#pragma once
#include "Events.h"
#include <imgui-SFML.h>

void Events::ProcessEvents(sf::RenderWindow& window, Application& application)
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
			ProcessKeyPress(window, keyPressed, application);
		}

		if (const sf::Event::KeyReleased* keyReleased = event->getIf<sf::Event::KeyReleased>())
		{
			ProcessKeyReleased(window, keyReleased, application);
		}

		if (const sf::Event::MouseMoved* mouseMoved = event->getIf<sf::Event::MouseMoved>())
		{
			ProcessMouseMoved(window, mouseMoved, application);
		}

		if (const sf::Event::MouseButtonPressed* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
		{
			ProcessMouseButtonPressed(window, mouseButtonPressed, application);
		}
	}
}

void Events::ProcessKeyPress(sf::RenderWindow& window, const sf::Event::KeyPressed* keyPressedEvent, Application& application)
{
	switch (keyPressedEvent->code) 
	{
		case sf::Keyboard::Key::Escape:
			window.close();
			break;
	}
}

void Events::ProcessKeyReleased(sf::RenderWindow& window, const sf::Event::KeyReleased* keyPressedEvent, Application& application)
{
}

void Events::ProcessMouseMoved(sf::RenderWindow& window, const sf::Event::MouseMoved* mouseMovedEvent, Application& application)
{
	//application.MousePosition = mouseMovedEvent->position;
}

void Events::ProcessMouseButtonPressed(sf::RenderWindow& window, const sf::Event::MouseButtonPressed* mouseButtonPressedEvent, Application& application)
{
	application.MousePosition = mouseButtonPressedEvent->position;
}
