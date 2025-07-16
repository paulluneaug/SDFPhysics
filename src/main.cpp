#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include "Configuration.h"
#include "Events.h"

#include "Application.h"

int main()
{
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(Configuration::WINDOW_SIZE), "SDF Physics");
    window.setFramerateLimit(Configuration::MAX_FRAMERATE);
    if (!ImGui::SFML::Init(window))
        return -1;

    Application application = Application{ window };

    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Time deltaTime = clock.restart();

        Events::ProcessEvents(window);

        application.Update(deltaTime);

        ImGui::SFML::Update(window, deltaTime);

        ImGui::Begin("Debug");
        application.DrawImGui(deltaTime);
        ImGui::End();

        window.clear();

        application.Draw();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}
