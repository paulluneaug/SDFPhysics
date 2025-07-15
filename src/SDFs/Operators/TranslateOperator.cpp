#pragma once
#include "TranslateOperator.h"


template <typename TFloatType>
TranslateOperator<TFloatType>::TranslateOperator(const SDF<TFloatType>& sdf, const sf::Vector2<TFloatType>& offset) : 
    m_sdf(sdf),
    m_offset(offset)
{
}

template <typename TFloatType>
TFloatType TranslateOperator<TFloatType>::Evaluate(const sf::Vector2<TFloatType> &position) const
{
    return m_sdf.Evaluate(position + m_offset);
}