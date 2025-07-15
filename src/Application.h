#pragma once
#include <SFML/Graphics.hpp>

class Application
{
private:
    sf::RenderWindow& m_window;

public:
    Application(sf::RenderWindow& window);

    void Update(const sf::Time& deltaTime);
    void Draw();
    void DrawImGui(const sf::Time& deltaTime);
};