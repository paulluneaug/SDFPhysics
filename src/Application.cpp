#include "Application.h"

#include <imgui.h>
#include <numeric>

#include "SDFs/AllSDFs.h"
#include "SDFs/SDFUtils.h"

#include "GFs/AllGFs.h"

#include "Utils/ImGuiExtension.h"

Application::Application(sf::RenderWindow& window, Profiler& profiler) :
	m_window(window),
	m_canvas(window.getSize() / Configuration::PIXEL_SIZE),
	m_renderTexture(window.getSize() / Configuration::PIXEL_SIZE),
	m_sceneSDF(std::move(CreateSceneSDF())),
	m_sceneGF(std::move(CreateSceneGF())),
	m_profiler(profiler),
	m_threadPool(16u),
	m_physicsSolver(m_sceneSDF.get()),
	m_currentRenderOption(RenderOption::InOut),
	m_triangle({ 100.0, 100.0 }, { 200.0, 100.0 }, { 100.0, 200.0 })
{
    m_renderTexture.setSmooth(false);
	ComputeSceneSDF();
	ComputeSceneGF();

	m_physicsSolver.AddObject(sf::Vector2<FloatType>{700.0, 200.0}, 5.0f);
}

void Application::Update(const sf::Time &deltaTime)
{
	float deltaTimeSeconds = deltaTime.asSeconds();
	m_currentDTIndex++;
	if (m_currentDTIndex >= m_deltaTimes.size()) {
		m_currentDTIndex = 0;
	}
	m_deltaTimes[m_currentDTIndex] = deltaTimeSeconds;
	
	m_profiler.StartEvent("Scene SDF Computation");
	ComputeSceneSDF();
	m_profiler.EndEvent();
	/*
	m_profiler.StartEvent("Scene GF Computation");
	ComputeSceneGF();
	m_profiler.EndEvent();
	*/
}

void Application::FixedUpdate(float fixedDeltaTime)
{
	m_physicsSolver.Update(fixedDeltaTime);
}

void Application::Draw()
{
	m_canvas.DrawOnTexture(m_renderTexture);

    sf::Sprite sprite(m_renderTexture.getTexture());
    const sf::FloatRect bounds = sprite.getGlobalBounds();
	const sf::Vector2f origin = bounds.size * 0.5f;
	const sf::Vector2f position = { origin.x * Configuration::PIXEL_SIZE, origin.y * Configuration::PIXEL_SIZE };
    sprite.setOrigin(origin);
    sprite.setPosition(position);
	sprite.setScale({ Configuration::PIXEL_SIZE, Configuration::PIXEL_SIZE });

    m_window.draw(sprite);


	sf::CircleShape circle{ 1.0f };
	circle.setPointCount(32);
	circle.setOrigin({ 1.0f, 1.0f });
	circle.setFillColor(sf::Color::White);
	const std::vector<VerletObject<FloatType>>& objects = m_physicsSolver.GetObjects();
	for (const VerletObject<FloatType>& obj : objects)
	{
		circle.setPosition({ static_cast<float>(obj.Position.x), static_cast<float>(obj.Position.y) });
		circle.setScale({ static_cast<float>(obj.Radius), static_cast<float>(obj.Radius) });
		m_window.draw(circle);
	}

	m_triangle.Draw(m_window, sf::Color(255, 255, 255, 180));
	std::vector<sf::Vector2<FloatType>> pointDescent = SDFUtils<FloatType>::FindClosestPointOfTriangle(m_sceneSDF.get(), m_triangle);

	sf::Color pointColor = sf::Color(255, 0, 0, 100);
	for (sf::Vector2<FloatType> point : pointDescent)
	{
		circle.setPosition({ static_cast<float>(point.x), static_cast<float>(point.y) });
		circle.setScale({ 5.0f, 5.0f });
		circle.setFillColor(pointColor);

		m_window.draw(circle);
	}

	/*
	sf::Vector2<FloatType> closestPointFromMouse = m_triangle.GetClosestPoint({ static_cast<FloatType>(MousePosition.x), static_cast<FloatType>(MousePosition.y) });
	circle.setPosition({ static_cast<float>(closestPointFromMouse.x), static_cast<float>(closestPointFromMouse.y) });
	circle.setScale({ 5.0f, 5.0f });
	circle.setFillColor(sf::Color::Green);
	m_window.draw(circle); 
	
	circle.setPosition({ static_cast<float>(MousePosition.x), static_cast<float>(MousePosition.y) });
	circle.setScale({ 5.0f, 5.0f });
	circle.setFillColor(sf::Color::Yellow);
	m_window.draw(circle);
	*/
}

void Application::DrawImGui(const sf::Time &deltaTime)
{
	// General stats
	float deltaTimeSeconds = deltaTime.asSeconds();
	if (ImGui::CollapsingHeader("General statistics", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen)) 
	{
		float averageDeltaTime = std::accumulate(m_deltaTimes.begin(), m_deltaTimes.end(), 0.0f) / m_deltaTimes.size();
		ImGui::Value("Update Time		", deltaTimeSeconds, "%0.6f");
		ImGui::Value("FPS				", 1.0 / deltaTimeSeconds, "%0.6f");

		if (m_currentDTIndex == 0) {
			m_captureAverageDeltaTime = averageDeltaTime;
		}
		ImGui::Value("Avg Update Time	", m_captureAverageDeltaTime, "%0.6f");
		ImGui::Value("Avg FPS			", 1.0f / m_captureAverageDeltaTime, "%0.6f");
	}

	// Scene SDF
	if (ImGui::CollapsingHeader("Scene settings", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen)) 
	{
		bool changed = false;

		if (ImGui::BeginCombo("Render Option", ToString(m_currentRenderOption)))
		{
			for (RenderOption option : m_allRenderOptions)
			{
				const bool selected = m_currentRenderOption == option;
				if (ImGui::Selectable(ToString(option), selected))
				{
					m_currentRenderOption = option;
					changed = true;
				}

				if (selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		if (m_currentRenderOption == RenderOption::Gradient)
		{
			changed |= ImGui::DragFloat("Sin Factor", &m_directionSinFactor);
			changed |= ImGui::Checkbox("Red", &m_redChannel);
			changed |= ImGui::Checkbox("Green", &m_greenChannel);
			changed |= ImGui::Checkbox("Blue", &m_blueChannel);
		}

		//changed |= ImGuiExtension::GenericDragFloat("Gradient evaluation offset", &SDFUtils<FloatType>::EvalOffset, 0.1f, 0.0f, 1.0f);

		if (changed)
		{
			ComputeSceneSDF(true);
		}

		int id = 0;
		m_sceneSDF->DrawDebug(id);

		if (ImGui::TreeNode("Triangle"))
		{
			m_triangle.DrawImGui();
			ImGui::TreePop();
		}
	}
}

void Application::ComputeSceneSDF(bool forceReevaluation)
{
	if ((!m_sceneSDF->RequireReevaluation() && !forceReevaluation) || !m_threadPool.IsDone())
	{
		return;
	}

	const sf::Vector2u size = m_canvas.GetSize();	
	const uint32_t threadCount = m_threadPool.ThreadCount();
	const uint32_t sliceHeight = size.y / threadCount;

	for (uint32_t k{ 0 }; k < threadCount; ++k) 
	{
		m_threadPool.AddTask([=] 
		{
			for (uint32_t x = 0u; x < size.x; ++x) 
			{
				for (uint32_t y{ k * sliceHeight }; y < (k + 1) * sliceHeight; ++y) 
				{
					m_canvas.SetPointColor(x, y, GetPointColorSDF(x, y));
				}
			}
		});
	}

	m_sceneSDF->OnSDFReevaluated();
}

sf::Color Application::GetPointColorSDF(uint32_t x, uint32_t y)
{
	const sf::Vector2<FloatType> samplePosition = { static_cast<FloatType>(x), static_cast<FloatType>(y) };
	const FloatType sdfValueAtPosition = m_sceneSDF->Evaluate(samplePosition);
	const float inverseFactor = 1.0f / m_directionSinFactor;

	switch (m_currentRenderOption)
	{
	case RenderOption::InOut:
		return sdfValueAtPosition < 0.0f ? sf::Color::White : sf::Color::Black;

	case RenderOption::Distance:
	{
		const FloatType multiplierMin = 0.2;
		const FloatType multiplierMax = 0.8;

		if (std::abs(sdfValueAtPosition) < 1.0f)
		{
			return sf::Color::White;
		}

		const bool inside = sdfValueAtPosition < 0;

		FloatType distanceMultiplier = 0.5f + std::sin(sdfValueAtPosition * inverseFactor) / 2.0f;
		distanceMultiplier = multiplierMin + (multiplierMax - multiplierMin) * distanceMultiplier;
		const sf::Color baseColor = inside ? sf::Color(51u, 80u, 190u) : sf::Color(190u, 120u, 0u);
		return sf::Color(
			static_cast<uint8_t>(baseColor.r * distanceMultiplier),
			static_cast<uint8_t>(baseColor.g * distanceMultiplier),
			static_cast<uint8_t>(baseColor.b * distanceMultiplier));
	}

	case RenderOption::Gradient:
		const sf::Vector2<FloatType> direction = SDFUtils<FloatType>::ComputeGradient(m_sceneSDF.get(), samplePosition);
		sf::Color pointColor =
		{
			static_cast<uint8_t>(m_redChannel ? ((direction.x / 2 + 0.5f) * 256u) : 0u),
			static_cast<uint8_t>(m_greenChannel ? ((direction.y / 2 + 0.5f) * 256u) : 0u),
			static_cast<uint8_t>(m_blueChannel ? ((1.0f + std::cos(sdfValueAtPosition * inverseFactor)) * 128u) : 0u),
			255u
		};
		return pointColor;
	}
	return sf::Color::Black;

}

SDF_UPtr<FloatType> Application::CreateSceneSDF()
{
	SDF_UPtr<FloatType> circle0 = std::make_unique<SDF_Circle<FloatType>>(sf::Vector2<FloatType>{1000.0, 400.0}, 200.0);
	SDF_UPtr<FloatType> circle1 = std::make_unique<SDF_Circle<FloatType>>(sf::Vector2<FloatType>{550.0, 500.0}, 200.0);

	SDF_UPtr<FloatType> box = std::make_unique<SDF_Box<FloatType>>(sf::Vector2<FloatType>{800.0, 700.0}, sf::Vector2<FloatType>{ 200.0, 100.0 });
	
	SDF_UPtr<FloatType> smoothMin = std::make_unique<SDF_SmoothUnionOperator<FloatType>>(std::move(circle1), std::move(circle0), 0.0001f);
	
	//SDF_UPtr<FloatType> cools = std::make_unique<SDF_CoolS<FloatType>>(sf::Vector2<FloatType>{500.0f, 700.0f}, 100.0f);




	//SDF_UPtr<FloatType> scene = std::make_unique<SDF_SmoothUnionOperator<FloatType>>(std::move(smoothMin), std::move(box), 0.0001f);

	SDF_UPtr<FloatType> star = std::make_unique<SDF_RegularStar<FloatType>>(sf::Vector2<FloatType>{1000.0, 550.0}, 300.0, 12, 0.8f);
	SDF_UPtr<FloatType> circle = std::make_unique<SDF_Circle<FloatType>>(sf::Vector2<FloatType>{750.0, 400.0}, 200.0);


	return std::make_unique<SDF_SmoothUnionOperator<FloatType>>(std::move(star), std::move(circle), 1.0f);
}

void Application::ComputeSceneGF(bool forceReevaluation)
{
	if ((!m_sceneGF->RequireReevaluation() && !forceReevaluation) || !m_threadPool.IsDone())
	{
		return;
	}

	const sf::Vector2u size = m_canvas.GetSize();
	const uint32_t threadCount = m_threadPool.ThreadCount();
	const uint32_t sliceHeight = size.y / threadCount;

	const float inverseFactor = 1.0f / m_directionSinFactor;

	for (uint32_t k{ 0 }; k < threadCount; ++k)
	{
		m_threadPool.AddTask([=]
			{
				for (uint32_t x = 0u; x < size.x; ++x)
				{
					for (uint32_t y{ k * sliceHeight }; y < (k + 1) * sliceHeight; ++y)
					{
						const sf::Vector2<FloatType> direction = m_sceneGF->Evaluate({ static_cast<FloatType>(x), static_cast<FloatType>(y) });
						const sf::Vector2<FloatType> normalizedDirection = direction.lengthSquared() == 0 ? Math::Vector2Zero<FloatType> : direction.normalized();
						sf::Color pointColor = 
						{ 
							static_cast<uint8_t>(m_redChannel ? ((normalizedDirection.x / 2 + 0.5f) * 256u) : 0u),
							static_cast<uint8_t>(m_greenChannel ? ((normalizedDirection.y / 2 + 0.5f) * 256u) : 0u),
							static_cast<uint8_t>(m_blueChannel ? ((1.0f + std::cos(direction.length() * inverseFactor)) * 128u) : 0u),
							255u 
						};
						m_canvas.SetPointColor(x, y, pointColor);
					}
				}
			});
	}
	m_threadPool.WaitForCompletion();
	m_sceneGF->OnGFReevaluated();
}

GF_UPtr<Configuration::FloatType> Application::CreateSceneGF()
{
	GF_UPtr<FloatType> circle0 = std::make_unique<GF_Circle<FloatType>>(sf::Vector2<FloatType>{200.0f, 100.0f}, 200.0f);
	GF_UPtr<FloatType> cools = std::make_unique<GF_Circle<FloatType>>(sf::Vector2<FloatType>{500.0f, 700.0f}, 100.0f);

	GF_UPtr<FloatType> circleMin = std::make_unique<GF_UnionOperator<FloatType>>(std::move(circle0), std::move(cools));

	//GF_UPtr<FloatType> box = std::make_unique<GF_Box<FloatType>>(sf::Vector2<FloatType>{800.0f, 900.0f}, sf::Vector2<FloatType>{ 200.0f, 100.0f });


	//GF_UPtr<FloatType> scene = std::make_unique<GF_SmoothUnionOperator<FloatType>>(std::move(circleMin), std::move(box), 0.5f);
	return circleMin;
}

const char* Application::ToString(RenderOption option)
{
	switch (option)
	{
	case RenderOption::InOut:
		return "InOut";
	case RenderOption::Distance:
		return "Distance";
	case RenderOption::Gradient:
		return "Gradient";
	}
}
