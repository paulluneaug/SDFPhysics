#include "Circle.h"

template <typename TFloatType>
Circle<TFloatType>::Circle(TFloatType radius) :
    m_radius(radius)
{
}

template <typename TFloatType>
TFloatType Circle<TFloatType>::Evaluate(const sf::Vector2<TFloatType> &position) const
{
    return position.length() - m_radius;
}