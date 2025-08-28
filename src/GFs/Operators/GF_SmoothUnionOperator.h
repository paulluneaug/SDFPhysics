#pragma once
#include "../GF.h"

#include "../../Alias.h"
#include "../../Math.h"

#include "../../Utils/ImGuiExtension.h"

using namespace Alias;

template<typename TFloatType>
class GF_SmoothUnionOperator : public GF<TFloatType>
{
    typedef GF<TFloatType> Base;
private:
    GF_UPtr<TFloatType> m_gf0;
    GF_UPtr<TFloatType> m_gf1;

    TFloatType m_smoothFactor;

public:
    GF_SmoothUnionOperator(GF_UPtr<TFloatType> gf0, GF_UPtr<TFloatType> gf1, TFloatType smoothFactor) :
        m_gf0(std::move(gf0)),
        m_gf1(std::move(gf1)),
        m_smoothFactor(smoothFactor)
    {
    }

    virtual bool RequireReevaluation() const override
    {
        return Base::RequireReevaluation()
            || m_gf0->RequireReevaluation()
            || m_gf1->RequireReevaluation();
    }

    virtual void OnGFReevaluated() override
    {
        Base::OnGFReevaluated();
        m_gf0->OnGFReevaluated();
        m_gf1->OnGFReevaluated();
    }

protected:
    sf::Vector2<TFloatType> Evaluate_Impl(const sf::Vector2<TFloatType>& position) const override
    {
        const sf::Vector2<TFloatType> gf0Value = m_gf0->Evaluate(position);
        const sf::Vector2<TFloatType> gf1Value = m_gf1->Evaluate(position);

        const TFloatType gf0Length = gf0Value.length();
        const TFloatType gf1Length = gf1Value.length();

        const TFloatType smoothMin = Math::SmoothMin(gf0Length, gf1Length, m_smoothFactor);
        const TFloatType lerpFactor = Math::InverseLerp(smoothMin, gf0Length, gf1Length);

        return Math::Lerp(gf0Value, gf1Value, lerpFactor);
    }

    virtual bool DisplayTreeNode() const override
    {
        return ImGui::TreeNode("Smooth Union");
    }

    virtual void DrawDebug_Impl(int& elementID) override
    {
        Base::DrawDebug_Impl(elementID);
        if (ImGuiExtension::GenericDragFloat("Smooth Factor", &m_smoothFactor)) 
        {
            OnGFChanged();
        }
        m_gf0->DrawDebug(elementID);
        m_gf1->DrawDebug(elementID);
    }
};
