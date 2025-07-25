#pragma once
#include "../SDF.h"

#include "../../Alias.h"
#include "../../Math.h"

using namespace Alias;

template<typename TFloatType>
class SmoothUnionOperator : public SDF<TFloatType>
{
    typedef SDF<TFloatType> Base;
private:
    SDF_Ptr<TFloatType> m_sdf0;
    SDF_Ptr<TFloatType> m_sdf1;

    TFloatType m_smoothFactor;

public:
    SmoothUnionOperator(SDF_Ptr<TFloatType> sdf0, SDF_Ptr<TFloatType> sdf1, TFloatType smoothFactor) :
        m_sdf0(std::move(sdf0)),
        m_sdf1(std::move(sdf1)),
        m_smoothFactor(smoothFactor)
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
        return Math::SmoothMin(m_sdf0->Evaluate(position), m_sdf1->Evaluate(position), m_smoothFactor);
    }

    virtual bool DisplayTreeNode() const override
    {
        return ImGui::TreeNode("Smooth Union");
    }

    virtual void DrawDebug_Impl(int& elementID) override
    {
        Base::DrawDebug_Impl(elementID);
        if (ImGui::DragFloat("Smooth Factor", &m_smoothFactor)) 
        {
            OnSDFChanged();
        }
        m_sdf0->DrawDebug(elementID);
        m_sdf1->DrawDebug(elementID);
    }
};
