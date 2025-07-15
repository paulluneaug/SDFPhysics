#pragma once
#include "../SDF.h"

template<typename TFloatType>
class Circle : public SDF<TFloatType>
{
private:
    TFloatType m_radius;

public:
    Circle(TFloatType radius);
    
    TFloatType Evaluate(const sf::Vector2<TFloatType>& position) const override;
};
