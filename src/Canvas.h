#pragma once
#include <SFML/Graphics.hpp>

class Canvas
{
private:
    sf::VertexArray m_vertexArray;
    sf::Vector2u m_size;

    bool m_changedSinceLastDraw = true;

public:
    Canvas(uint32_t width, uint32_t height);
    Canvas(sf::Vector2u size);

    void SetPointColor(uint32_t x, uint32_t y, sf::Color color);
    void DrawOnTexture(sf::RenderTexture& texture);
};

