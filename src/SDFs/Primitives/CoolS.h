#pragma once
#include "Primitive.h"

#include <algorithm>

#include "../../Math.h"

template<typename TFloatType>
class CoolS : public Primitive<TFloatType>
{
    typedef Primitive<TFloatType> Base;

public:
    CoolS(const sf::Vector2<TFloatType>& position) :
        Base(position)
    {
    }

protected:
    virtual TFloatType Evaluate_Impl(const sf::Vector2<TFloatType>& position) const override
    {
        sf::Vector2<TFloatType> p = position / 100.0f;
        float six = (p.y < 0.0f) ? -p.x : p.x;
        p.x = abs(p.x);
        p.y = abs(p.y) - 0.2f;
        float rex = p.x - std::min(std::round(p.x / 0.4f), 0.4f);

        float aby = std::abs(p.y - 0.2f) - 0.6f;

        float f0 = std::clamp(0.5f * (six - p.y), 0.0f, 0.2f);
        sf::Vector2f seg0 = { six - f0, -p.y - f0 };
        float d = seg0.dot(seg0);

        float f1 = std::clamp(0.5f * (p.x - aby), 0.0f, 0.4f);
        sf::Vector2f seg1 = { p.x - f1, -aby - f1 };

        d = std::min(d, seg1.dot(seg1));

        sf::Vector2f seg2 = { rex, p.y - std::clamp(p.y, 0.0f, 0.4f) };
        d = std::min(d, seg2.dot(seg2));

        float s = 2.0f * p.x + aby + std::abs(aby + 0.4f) - 0.4f;
        return std::sqrt(d) * Math::Sign(s);
    }


    virtual bool DisplayTreeNode() const override
    {
        return ImGui::TreeNode("Cool S");
    }

    virtual void DrawDebug_Impl(int& elementID) override
    {
        Base::DrawDebug_Impl(elementID);
    }
};
