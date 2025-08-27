#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include "Configuration.h"
#include "Events.h"

#include "Application.h"
#include "Utils/Profiler.h"

int main()
{
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(Configuration::WINDOW_SIZE), "SDF Physics");
    window.setFramerateLimit(Configuration::MAX_FRAMERATE);
    if (!ImGui::SFML::Init(window))
        return -1;

    Profiler profiler = Profiler{};
    Application application = Application{ window, profiler };

    sf::Clock clock;

    const float fixedDelaTime = 1.0f / static_cast<float>(Configuration::FIXED_FRAMERATE);
    float accumulatedFixedTime = 0.0f;

    while (window.isOpen())
    {
        profiler.Reset();

        sf::Time deltaTime = clock.restart();
        accumulatedFixedTime += deltaTime.asSeconds();

        profiler.StartEvent("Event Processing");
        Events::ProcessEvents(window);
        profiler.EndEvent();

        profiler.StartEvent("Application Update");
        application.Update(deltaTime);
        profiler.EndEvent();

        profiler.StartEvent("Application Fixed Update");
        while (accumulatedFixedTime > fixedDelaTime) 
        {
            accumulatedFixedTime -= fixedDelaTime;
            application.FixedUpdate(fixedDelaTime);
        }
        profiler.EndEvent();

        profiler.StartEvent("Window Clear");
        window.clear();
        profiler.EndEvent();

        profiler.StartEvent("Application Draw");
        application.Draw();
        profiler.EndEvent();

        // ImGui
        ImGui::SFML::Update(window, deltaTime);

        ImGui::Begin("Debug");
        application.DrawImGui(deltaTime);

        profiler.DrawImGui();
        ImGui::End();
        ImGui::SFML::Render(window);

        window.display();
    }

    ImGui::SFML::Shutdown();
}
