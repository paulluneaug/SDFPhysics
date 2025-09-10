#pragma once
#include <SFML/Graphics.hpp>
#include "Application.h"

class Events
{
public:
	static void ProcessEvents(sf::RenderWindow& window, Application& application);

private:
	static void ProcessKeyPress(sf::RenderWindow& window, const sf::Event::KeyPressed* keyPressedEvent, Application& application);
	static void ProcessKeyReleased(sf::RenderWindow& window, const sf::Event::KeyReleased* keyPressedEvent, Application& application);
	static void ProcessMouseMoved(sf::RenderWindow& window, const sf::Event::MouseMoved* mouseMovedEvent, Application& application);
	static void ProcessMouseButtonPressed(sf::RenderWindow& window, const sf::Event::MouseButtonPressed* mouseButtonPressedEvent, Application& application);
};

