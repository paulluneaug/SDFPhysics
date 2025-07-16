#pragma once
#include "../SDF.h"

template<typename TFloatType>
class Primitive : public SDF<TFloatType>
{
    typedef SDF<TFloatType> Base;
private:
    sf::Vector2<TFloatType> m_position;

public:
    Primitive(const sf::Vector2<TFloatType>& position) :
        m_position(position)
    {

    }

    virtual TFloatType Evaluate(const sf::Vector2<TFloatType>& samplePosition) const override
    {
        return Evaluate_Impl(samplePosition - m_position);
    }

protected:

    virtual void DrawDebug_Impl(int& elementID)
    {
        Base::DrawDebug_Impl(elementID);

        TFloatType position[2] = { m_position.x, m_position.y };
        if (ImGui::DragFloat2("Position", position))
        {
            m_position = { position[0], position[1] };
        }
    }
};

