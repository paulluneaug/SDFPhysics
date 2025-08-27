#pragma once
#include "GF_Primitive.h"

#include "../../Utils/ImGuiUtils.h"

template<typename TFloatType>
class GF_Circle : public GF_Primitive<TFloatType>
{
    typedef GF_Primitive<TFloatType> Base;
private:
    TFloatType m_radius;

public:
    GF_Circle(const sf::Vector2<TFloatType>& position, TFloatType radius) :
        Base(position),
        m_radius(radius)
    {
    }
   
protected:
    virtual sf::Vector2<TFloatType> Evaluate_Impl(const sf::Vector2<TFloatType>& position) const override
    {
        if (position == Math::Vector2Zero<TFloatType>) 
        {
            return sf::Vector2<TFloatType>{static_cast<TFloatType>(0.0), -m_radius};
        }
        return position.normalized() * m_radius - position;
    }


    virtual bool DisplayTreeNode() const override
    {
        return ImGui::TreeNode("Circle");
    }

    virtual void DrawDebug_Impl(int& elementID) override
    {
        Base::DrawDebug_Impl(elementID);
        if (ImGuiExtension::GenericDragFloat("Radius", &m_radius, 1.0f, 0.0f))
        {
            OnGFChanged();
        }
    }
};
