#pragma once
#include <SFML/Graphics.hpp>

class Events
{
public:
	static void ProcessEvents(sf::RenderWindow& window);

private:
	static void ProcessKeyPress(sf::RenderWindow& window, const sf::Event::KeyPressed* keyPressedEvent);
	static void ProcessKeyReleased(sf::RenderWindow& window, const sf::Event::KeyReleased* keyPressedEvent);
};

