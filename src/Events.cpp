#pragma once
#include "Events.h"
#include <imgui-SFML.h>

void Events::ProcessEvents(sf::RenderWindow& window)
{
	while (const std::optional event = window.pollEvent())
	{
		ImGui::SFML::ProcessEvent(window, *event);

		if (event->is<sf::Event::Closed>())
		{
			window.close();
		}

		if (const sf::Event::KeyPressed* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			ProcessKeyPress(window, keyPressed);
		}

		if (const sf::Event::KeyReleased* keyReleased = event->getIf<sf::Event::KeyReleased>())
		{
			ProcessKeyReleased(window, keyReleased);
		}
	}
}

void Events::ProcessKeyPress(sf::RenderWindow& window, const sf::Event::KeyPressed* keyPressedEvent)
{
	switch (keyPressedEvent->code) 
	{
		case sf::Keyboard::Key::Escape:
			window.close();
			break;
	}
}

void Events::ProcessKeyReleased(sf::RenderWindow& window, const sf::Event::KeyReleased* keyPressedEvent)
{
}
