#pragma once
#include "SDF_Primitive.h"

#include <algorithm>

#include "../../Math.h"
#include "../../Utils/ImGuiExtension.h"

template<typename TFloatType>
class SDF_Box : public PrimitiveSDF<TFloatType>
{
    typedef PrimitiveSDF<TFloatType> Base;

private:
    sf::Vector2<TFloatType> m_size;

public:
    SDF_Box(const sf::Vector2<TFloatType>& position, sf::Vector2<TFloatType>& size) :
        Base(position),
        m_size(size)
    {
    }

protected:
    virtual TFloatType Evaluate_Impl(const sf::Vector2<TFloatType>& position) const override
    {
        sf::Vector2<TFloatType> d = Math::Abs(position) - m_size;
        return Math::Max(d, Math::Vector2Zero<TFloatType>).length() + Math::Min(Math::Max(d.x, d.y), static_cast<TFloatType>(0));
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
            OnSDFChanged();
        }
    }
};
