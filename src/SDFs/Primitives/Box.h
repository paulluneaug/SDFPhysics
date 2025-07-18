#pragma once
#include "Primitive.h"

#include <algorithm>

#include "../../Math.h"

template<typename TFloatType>
class Box : public Primitive<TFloatType>
{
    typedef Primitive<TFloatType> Base;

private:
    sf::Vector2<TFloatType> m_size;

public:
    Box(const sf::Vector2<TFloatType>& position, sf::Vector2<TFloatType>& size) :
        Base(position),
        m_size(size)
    {
    }

protected:
    virtual TFloatType Evaluate_Impl(const sf::Vector2<TFloatType>& position) const override
    {
        sf::Vector2<TFloatType> d = Math::Abs(position) - m_size;
        return Math::Max(d, Math::Vector2Zero<TFloatType>).length() + std::min(std::max(d.x, d.y), 0.0f);
    }


    virtual bool DisplayTreeNode() const override
    {
        return ImGui::TreeNode("Box");
    }

    virtual void DrawDebug_Impl(int& elementID) override
    {
        Base::DrawDebug_Impl(elementID);

        TFloatType size[2] = { m_size.x, m_size.y };
        if (ImGui::DragFloat2("Size", size))
        {
            m_size = { size[0], size[1] };
        }
    }
};
