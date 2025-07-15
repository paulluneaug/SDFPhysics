#pragma once
#include <SFML/Graphics.hpp>

template<typename TFloatType>
class SDF
{
    TFloatType Evaluate(const sf::Vector2<TFloatType>& position) const = 0;
};