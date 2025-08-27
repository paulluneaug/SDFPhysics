#pragma once
#include "SDF_Primitive.h"
#include "../../Utils/ImGuiUtils.h"

template<typename TFloatType>
class SDF_Circle : public PrimitiveSDF<TFloatType>
{
    typedef PrimitiveSDF<TFloatType> Base;
private:
    TFloatType m_radius;

public:
    SDF_Circle(const sf::Vector2<TFloatType>& position, TFloatType radius) :
        Base(position),
        m_radius(radius)
    {
    }
   
protected:
    virtual TFloatType Evaluate_Impl(const sf::Vector2<TFloatType>& position) const override
    {
        return position.length() - m_radius;
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
            OnSDFChanged();
        }
    }
};
