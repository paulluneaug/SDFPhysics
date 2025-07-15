#pragma once
#include "SDF.h"

template<typename TFloatType>
class TranslateOperator : public SDF<TFloatType>
{
private:
    const SDF<TFloatType>& m_sdf;
    const sf::Vector2<TFloatType>& m_offset;

public:
    TranslateOperator(const SDF<TFloatType>& sdf, const sf::Vector2<TFloatType>& sdf1);

    TFloatType Evaluate(const sf::Vector2<TFloatType>& position) const override;
};
