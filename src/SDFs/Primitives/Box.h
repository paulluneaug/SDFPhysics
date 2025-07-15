#pragma once
#include "../SDF.h"

template<typename TFloatType>
class Box : public SDF<TFloatType>
{
private:
    sf::Vector2<TFloatType>& m_size;

public:
    Box(sf::Vector2<TFloatType>& size);
    
    TFloatType Evaluate(const sf::Vector2<TFloatType>& position) const override;
};
