#pragma once
#include "SDF_Primitive.h"

#include <algorithm>

#include "../../Math.h"
#include "../../Utils/ImGuiExtension.h"

template<typename TFloatType>
class SDF_RegularStar : public PrimitiveSDF<TFloatType>
{
    typedef PrimitiveSDF<TFloatType> Base;

private:
    TFloatType m_radius;
    int m_branches;
    TFloatType m_length;

public:
    SDF_RegularStar(const sf::Vector2<TFloatType>& position, TFloatType radius, int branches, TFloatType length) :
        Base(position),
        m_radius(radius),
        m_branches(branches),
        m_length(length)
    {
    }

protected:
    virtual TFloatType Evaluate_Impl(const sf::Vector2<TFloatType>& position) const override
    {
        constexpr TFloatType zero = static_cast<TFloatType>(0.0);
        constexpr TFloatType two = static_cast<TFloatType>(2.0);

        sf::Vector2<TFloatType> p = position;

        TFloatType m = m_branches + m_length * (two - m_branches);

        TFloatType an = Math::PI<TFloatType> / m_branches;
        TFloatType en = Math::PI<TFloatType> / m;
        sf::Vector2<TFloatType> racs = sf::Vector2<TFloatType>(std::cos(an) * m_radius, std::sin(an) * m_radius);
        sf::Vector2<TFloatType> ecs = sf::Vector2<TFloatType>(std::cos(en), std::sin(en));

        p.x = std::abs(p.x);
        p.y = std::abs(p.y);

        TFloatType bn = std::fmod(std::atan(p.x / p.y), two * an) - an;
        p = p.length() * sf::Vector2<TFloatType>(std::cos(bn), std::abs(std::sin(bn)));

        p -= racs;
        p += ecs * Math::Clamp(-(p.dot(ecs)), zero, racs.y / ecs.y);
        return p.length() * Math::Sign(p.x);
    }


    virtual bool DisplayTreeNode() const override
    {
        return ImGui::TreeNode("Regular Star");
    }

    virtual void DrawDebug_Impl(int& elementID) override
    {
        Base::DrawDebug_Impl(elementID);

        bool changed = false;
        changed |= ImGuiExtension::GenericDragFloat("Radius", &m_radius);
        changed |= ImGui::DragInt("Branches", &m_branches, 1.0f, 2, 40);
        changed |= ImGuiExtension::GenericDragFloat("Length", &m_length, 0.05f, 0.0f, 1.0f);

        if (changed)
        {
            OnSDFChanged();
        }
    }
};
