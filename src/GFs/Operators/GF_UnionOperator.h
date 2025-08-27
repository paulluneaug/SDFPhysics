#pragma once
#include "../GF.h"

#include "../../Alias.h"

using namespace Alias;

template<typename TFloatType>
class GF_UnionOperator : public GF<TFloatType>
{
    typedef GF<TFloatType> Base;
private:
    GF_UPtr<TFloatType> m_gf0;
    GF_UPtr<TFloatType> m_gf1;

public:
    GF_UnionOperator(GF_UPtr<TFloatType> gf0, GF_UPtr<TFloatType> gf1) :
        m_gf0(std::move(gf0)),
        m_gf1(std::move(gf1))
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
        sf::Vector2<TFloatType> gf0Value = m_gf0->Evaluate(position);
        sf::Vector2<TFloatType> gf1Value = m_gf1->Evaluate(position);

        return gf0Value.lengthSquared() < gf1Value.lengthSquared() ? gf0Value : gf1Value;
    }

    virtual bool DisplayTreeNode() const override
    {
        return ImGui::TreeNode("Union");
    }

    virtual void DrawDebug_Impl(int& elementID) override
    {
        Base::DrawDebug_Impl(elementID);
        m_gf0->DrawDebug(elementID);
        m_gf1->DrawDebug(elementID);
    }
};
