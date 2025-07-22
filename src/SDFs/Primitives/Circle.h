#pragma once
#include "Primitive.h"

template<typename TFloatType>
class Circle : public Primitive<TFloatType>
{
    typedef Primitive<TFloatType> Base;
private:
    TFloatType m_radius;

public:
    Circle(const sf::Vector2<TFloatType>& position, TFloatType radius) :
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
        if (ImGui::DragFloat("Radius", &m_radius, 1.0f, 0.0f)) 
        {
            OnSDFChanged();
        }
    }
};
