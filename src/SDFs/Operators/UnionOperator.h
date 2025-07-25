#pragma once
#include "../SDF.h"

#include "../../Alias.h"

using namespace Alias;

template<typename TFloatType>
class UnionOperator : public SDF<TFloatType>
{
    typedef SDF<TFloatType> Base;
private:
    SDF_Ptr<TFloatType> m_sdf0;
    SDF_Ptr<TFloatType> m_sdf1;

public:
    UnionOperator(SDF_Ptr<TFloatType> sdf0, SDF_Ptr<TFloatType> sdf1) :
        m_sdf0(std::move(sdf0)),
        m_sdf1(std::move(sdf1))
    {
    }

    virtual bool RequireReevaluation() const override
    {
        return Base::RequireReevaluation()
            || m_sdf0->RequireReevaluation()
            || m_sdf1->RequireReevaluation();
    }

    virtual void OnSDFReevaluated() override
    {
        Base::OnSDFReevaluated();
        m_sdf0->OnSDFReevaluated();
        m_sdf1->OnSDFReevaluated();
    }

protected:
    TFloatType Evaluate_Impl(const sf::Vector2<TFloatType>& position) const override
    {
        return std::min(m_sdf0->Evaluate(position), m_sdf1->Evaluate(position));
    }

    virtual bool DisplayTreeNode() const override
    {
        return ImGui::TreeNode("Union");
    }

    virtual void DrawDebug_Impl(int& elementID) override
    {
        Base::DrawDebug_Impl(elementID);
        m_sdf0->DrawDebug(elementID);
        m_sdf1->DrawDebug(elementID);
    }
};
