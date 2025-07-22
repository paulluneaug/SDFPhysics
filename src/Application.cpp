#include "Application.h"

#include <imgui.h>
#include <numeric>

#include "SDFs/AllSDFs.h"
#include "SDFs/Primitives/Circle.h"

Application::Application(sf::RenderWindow &window, Profiler& profiler) :
    m_window(window),
    m_canvas(window.getSize() / Configuration::PIXEL_SIZE),
    m_renderTexture(window.getSize() / Configuration::PIXEL_SIZE),
	m_sceneSDF(std::move(CreateSceneSDF())),
	m_profiler(profiler)
{
    m_renderTexture.setSmooth(true);
	ComputeSDF();
}

void Application::Update(const sf::Time &deltaTime)
{
	float deltaTimeSeconds = deltaTime.asSeconds();
	m_currentDTIndex++;
	if (m_currentDTIndex >= m_deltaTimes.size()) {
		m_currentDTIndex = 0;
	}
	m_deltaTimes[m_currentDTIndex] = deltaTimeSeconds;

	m_profiler.StartEvent("SDF Computation");
	ComputeSDF();
	m_profiler.EndEvent();
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
		int id = 0;
		m_sceneSDF->DrawDebug(id);
	}
}

void Application::ComputeSDF()
{
	const sf::Vector2u size = m_canvas.GetSize();
	for (uint32_t x = 0u; x < size.x; ++x) 
	{
		for (uint32_t y = 0u; y < size.y; ++y)
		{
			FloatType distance = m_sceneSDF->Evaluate({ static_cast<FloatType>(x), static_cast<FloatType>(y) });
			m_canvas.SetPointColor(x, y, distance < 0.0f ? sf::Color::White : sf::Color::Black);
		}
	}
}

SDF_Ptr<FloatType> Application::CreateSceneSDF()
{
	SDF_Ptr<FloatType> circle0 = std::make_unique<Circle<FloatType>>(sf::Vector2<FloatType>{200.0f, 100.0f}, 200.0f);
	SDF_Ptr<FloatType> circle1 = std::make_unique<Circle<FloatType>>(sf::Vector2<FloatType>{500.0f, 700.0f}, 100.0f);

	SDF_Ptr<FloatType> circleMin = std::make_unique<UnionOperator<FloatType>>(std::move(circle0), std::move(circle1));

	SDF_Ptr<FloatType> box = std::make_unique<Box<FloatType>>(sf::Vector2<FloatType>{800.0f, 900.0f}, sf::Vector2<FloatType>{ 200.0f, 100.0f });


	SDF_Ptr<FloatType> scene = std::make_unique<SmoothUnionOperator<FloatType>>(std::move(circleMin), std::move(box), 0.5f);
	return scene;
}
