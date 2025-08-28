#pragma once
#include "SDF_Primitive.h"

#include <algorithm>

#include "../../Math.h"
#include "../../Utils/ImGuiExtension.h"

template<typename TFloatType>
class SDF_CoolS : public PrimitiveSDF<TFloatType>
{
    typedef PrimitiveSDF<TFloatType> Base;

private:
    TFloatType m_scale;

public:
    SDF_CoolS(const sf::Vector2<TFloatType>& position, TFloatType scale) :
        Base(position),
        m_scale(scale)
    {
    }

protected:
    virtual TFloatType Evaluate_Impl(const sf::Vector2<TFloatType>& position) const override
    {
        const TFloatType zero = static_cast<TFloatType>(0.0);
        const TFloatType point2 = static_cast<TFloatType>(0.2);
        const TFloatType point4 = static_cast<TFloatType>(0.4);
        const TFloatType point5 = static_cast<TFloatType>(0.5);
        const TFloatType point6 = static_cast<TFloatType>(0.6);
        const TFloatType point7 = static_cast<TFloatType>(0.7);


        sf::Vector2<TFloatType> p = position / m_scale;
        TFloatType six = (p.y < zero) ? -p.x : p.x;
        p.x = std::abs(p.x);
        p.y = std::abs(p.y) - point2;
        TFloatType rex = p.x - std::min(std::round(p.x / point4), point4);

        TFloatType aby = std::abs(p.y - point2) - point6;

        TFloatType f0 = std::clamp(point5 * (six - p.y), zero, point2);
        sf::Vector2<TFloatType> seg0 = { six - f0, -p.y - f0 };
        TFloatType d = seg0.dot(seg0);

        TFloatType f1 = std::clamp(point5 * (p.x - aby), zero, point4);
        sf::Vector2<TFloatType> seg1 = { p.x - f1, -aby - f1 };

        d = std::min(d, seg1.dot(seg1));

        sf::Vector2<TFloatType> seg2 = { rex, p.y - std::clamp(p.y, zero, point4) };
        d = std::min(d, seg2.dot(seg2));

        TFloatType s = 2.0f * p.x + aby + std::abs(aby + point4) - point4;
        return std::sqrt(d) * Math::Sign(s);
    }


    virtual bool DisplayTreeNode() const override
    {
        return ImGui::TreeNode("Cool S");
    }

    virtual void DrawDebug_Impl(int& elementID) override
    {
        Base::DrawDebug_Impl(elementID);
        if (ImGuiExtension::GenericDragFloat("Scale", &m_scale)) 
        {
            OnSDFChanged();
        }
    }
};
