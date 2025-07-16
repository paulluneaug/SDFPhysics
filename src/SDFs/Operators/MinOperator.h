#pragma once
#include "../SDF.h"

template<typename TFloatType>
class MinOperator : public SDF<TFloatType>
{
private:
    const SDF<TFloatType>& m_sdf0;
    const SDF<TFloatType>& m_sdf1;

public:
    MinOperator(const SDF<TFloatType>& sdf0, const SDF<TFloatType>& sdf1);

    TFloatType Evaluate(const sf::Vector2<TFloatType>& position) const override;
};
