#pragma once
#include "GF_Primitive.h"

#include <algorithm>

#include "../../Math.h"

#include "../../Utils/ImGuiUtils.h"

template<typename TFloatType>
class GF_Box : public GF_Primitive<TFloatType>
{
    typedef GF_Primitive<TFloatType> Base;

private:
    sf::Vector2<TFloatType> m_size;

public:
    GF_Box(const sf::Vector2<TFloatType>& position, sf::Vector2<TFloatType>& size) :
        Base(position),
        m_size(size)
    {
    }

protected:
    virtual sf::Vector2<TFloatType> Evaluate_Impl(const sf::Vector2<TFloatType>& position) const override
    {
        const TFloatType zero = static_cast<TFloatType>(0.0);

        const sf::Vector2<TFloatType> absPositition = Math::Abs(position);
        const sf::Vector2<TFloatType> delta = absPositition - m_size;

        if (delta.x > zero && delta.y > zero)
        {
            return sf::Vector2<TFloatType>
            {
                delta.x * Math::Sign(position.x),
                delta.y * Math::Sign(position.y)
            };
        }

        if (delta.x > delta.y)
        {
            return sf::Vector2<TFloatType>
            {
                (m_size.x - absPositition.x) * Math::Sign(position.x),
                zero
            };
        }


        return sf::Vector2<TFloatType>
        {
            zero,
            (m_size.y - absPositition.y)* Math::Sign(position.y)
        };
    }


    virtual bool DisplayTreeNode() const override
    {
        return ImGui::TreeNode("Box");
    }

    virtual void DrawDebug_Impl(int& elementID) override
    {
        Base::DrawDebug_Impl(elementID);

        if (ImGuiExtension::GenericDragFloat2("Size", &m_size))
        {
            OnGFChanged();
        }
    }
};
