#pragma once
#include "MinOperator.h"

#include <algorithm>


template <typename TFloatType>
MinOperator<TFloatType>::MinOperator(const SDF<TFloatType>& sdf0, const SDF<TFloatType>& sdf1) : 
    m_sdf0(sdf0),
    m_sdf1(sdf1)
{
}

template <typename TFloatType>
TFloatType MinOperator<TFloatType>::Evaluate(const sf::Vector2<TFloatType> &position) const
{
    return std::min(m_sdf0.Evaluate(position), m_sdf1.Evaluate(position));
}