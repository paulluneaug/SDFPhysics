#pragma once
#include "../SDF.h"

#include "../../Utils/ImGuiExtension.h"

template<typename TFloatType>
class PrimitiveSDF : public SDF<TFloatType>
{
    typedef SDF<TFloatType> Base;
private:
    sf::Vector2<TFloatType> m_position;

public:
    PrimitiveSDF(const sf::Vector2<TFloatType>& position) :
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

        if (ImGuiExtension::GenericDragFloat2("Position", &m_position))
        {
            OnSDFChanged();
        }
    }
};

