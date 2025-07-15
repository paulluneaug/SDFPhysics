#include "Box.h"

#include <algorithm>

template <typename TFloatType>
Box<TFloatType>::Box(sf::Vector2<TFloatType>& size) :
    m_size(size)
{
}

template <typename TFloatType>
TFloatType Box<TFloatType>::Evaluate(const sf::Vector2<TFloatType> &position) const
{
    sf::Vector2<TFloatType> d = std::abs(p) - m_size;
    return (max(d, 0.0)) + min(max(d.x, d.y), 0.0);
}