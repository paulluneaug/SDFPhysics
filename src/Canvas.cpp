#include "Canvas.h"

Canvas::Canvas(uint32_t width, uint32_t height) :
    m_vertexArray(sf::PrimitiveType::Points, width * height),
    m_size(width, height),
    m_changedSinceLastDraw(true)
{
    uint32_t index = 0;
    for (uint32_t x = 0u; x < width; ++x)
    {
        for (uint32_t y = 0u; y < height; ++y)
        {
            m_vertexArray[index++].position = sf::Vector2f{ static_cast<float>(x), static_cast<float>(y) };
        }
    }
}

Canvas::Canvas(sf::Vector2u size) : Canvas(size.x, size.y)
{
}

void Canvas::SetPointColor(uint32_t x, uint32_t y, sf::Color color)
{
    const uint32_t index = x * m_size.y + y;
    m_vertexArray[index].color = color;

    m_changedSinceLastDraw = true;
}

void Canvas::DrawOnTexture(sf::RenderTexture& texture)
{
    if (!m_changedSinceLastDraw)
    {
        return;
    }

    texture.draw(m_vertexArray);
    texture.display();

    m_changedSinceLastDraw = false;
}
