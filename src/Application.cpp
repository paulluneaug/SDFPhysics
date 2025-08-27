#include "Application.h"

#include <imgui.h>
#include <numeric>

#include "SDFs/AllSDFs.h"
#include "SDFs/SDFUtils.h"

#include "GFs/AllGFs.h"

#include "Utils/ImGuiUtils.h"

Application::Application(sf::RenderWindow& window, Profiler& profiler) :
	m_window(window),
	m_canvas(window.getSize() / Configuration::PIXEL_SIZE),
	m_renderTexture(window.getSize() / Configuration::PIXEL_SIZE),
	m_sceneSDF(std::move(CreateSceneSDF())),
	m_sceneGF(std::move(CreateSceneGF())),
	m_profiler(profiler),
	m_threadPool(16u),
	m_physicsSolver(m_sceneSDF.get()),
	m_directionSinFactor(200.0f)
{
	//SDFUtils<FloatType>::EvalOffset = static_cast<FloatType>(0.01f);

    m_renderTexture.setSmooth(false);
	ComputeSceneSDF();
	ComputeSceneGF();

	
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
		bool changed = ImGui::DragFloat("Sin Factor", &m_directionSinFactor);
		changed |= ImGui::Checkbox("Red", &m_redChannel);
		changed |= ImGui::Checkbox("Green", &m_greenChannel);
		changed |= ImGui::Checkbox("Blue", &m_blueChannel);

		//changed |= ImGuiExtension::GenericDragFloat("Gradient evaluation offset", &SDFUtils<FloatType>::EvalOffset, 0.1f, 0.0f, 1.0f);

		if (changed)
		{
			ComputeSceneSDF(true);
		}

		int id = 0;
		m_sceneSDF->DrawDebug(id);
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

	const float inverseFactor = 1.0f / m_directionSinFactor;

	for (uint32_t k{ 0 }; k < threadCount; ++k) 
	{
		m_threadPool.AddTask([=] 
		{
			for (uint32_t x = 0u; x < size.x; ++x) 
			{
				for (uint32_t y{ k * sliceHeight }; y < (k + 1) * sliceHeight; ++y) 
				{
					//const FloatType distance = m_sceneSDF->Evaluate({ static_cast<FloatType>(x), static_cast<FloatType>(y) });
					//m_canvas.SetPointColor(x, y, distance < 0.0f ? sf::Color::White : sf::Color::Black);

					const sf::Vector2<FloatType> direction = SDFUtils<FloatType>::ComputeGradient(m_sceneSDF.get(), { static_cast<FloatType>(x), static_cast<FloatType>(y) });
					const sf::Vector2<FloatType> normalizedDirection = direction.lengthSquared() == 0 ? Math::Vector2Zero<FloatType> : direction.normalized();
					sf::Color pointColor =
					{
						static_cast<uint8_t>(m_redChannel ? ((normalizedDirection.x / 2 + 0.5f) * 256u) : 0u),
						static_cast<uint8_t>(m_greenChannel ? ((normalizedDirection.y / 2 + 0.5f) * 256u) : 0u),
						static_cast<uint8_t>(m_blueChannel ? ((1.0f + std::cos(m_sceneSDF->Evaluate({ static_cast<FloatType>(x), static_cast<FloatType>(y) }) * inverseFactor)) * 128u) : 0u),
						255u
					};
					m_canvas.SetPointColor(x, y, pointColor);
				}
			}
		});
	}

	m_sceneSDF->OnSDFReevaluated();
}

SDF_UPtr<FloatType> Application::CreateSceneSDF()
{
	SDF_UPtr<FloatType> circle0 = std::make_unique<SDF_Circle<FloatType>>(sf::Vector2<FloatType>{200.0f, 100.0f}, 200.0f);
	SDF_UPtr<FloatType> circle1 = std::make_unique<SDF_Circle<FloatType>>(sf::Vector2<FloatType>{400.0f, 500.0f}, 100.0f);

	//SDF_UPtr<FloatType> cools = std::make_unique<SDF_CoolS<FloatType>>(sf::Vector2<FloatType>{500.0f, 700.0f}, 100.0f);

	//SDF_UPtr<FloatType> circleMin = std::make_unique<SDF_UnionOperator<FloatType>>(std::move(circle0), std::move(cools));

	//SDF_UPtr<FloatType> box = std::make_unique<SDF_Box<FloatType>>(sf::Vector2<FloatType>{800.0f, 900.0f}, sf::Vector2<FloatType>{ 200.0f, 100.0f });


	//SDF_UPtr<FloatType> scene = std::make_unique<SDF_SmoothUnionOperator<FloatType>>(std::move(circle0), std::move(circle1), 0.5f);
	return circle0;
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
