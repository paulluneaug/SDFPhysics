#pragma once
#include "../GF.h"

#include "../../Utils/ImGuiUtils.h"

template<typename TFloatType>
class GF_Primitive : public GF<TFloatType>
{
    typedef GF<TFloatType> Base;
private:
    sf::Vector2<TFloatType> m_position;

public:
    GF_Primitive(const sf::Vector2<TFloatType>& position) :
        m_position(position)
    {

    }

    virtual sf::Vector2<TFloatType> Evaluate(const sf::Vector2<TFloatType>& samplePosition) const override
    {
        return Evaluate_Impl(samplePosition - m_position);
    }

protected:

    virtual void DrawDebug_Impl(int& elementID)
    {
        Base::DrawDebug_Impl(elementID);
        
        if (ImGuiExtension::GenericDragFloat2("Position", &m_position))
        {
            OnGFChanged();
        }
    }
};

