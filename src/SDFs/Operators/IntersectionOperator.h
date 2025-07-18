#pragma once
#include "../SDF.h"

#include "../../Alias.h"

using namespace Alias;

template<typename TFloatType>
class IntersectionOperator : public SDF<TFloatType>
{
    typedef SDF<TFloatType> Base;
private:
    SDF_Ptr<TFloatType> m_sdf0;
    SDF_Ptr<TFloatType> m_sdf1;

public:
    IntersectionOperator(SDF_Ptr<TFloatType> sdf0, SDF_Ptr<TFloatType> sdf1) :
        m_sdf0(std::move(sdf0)),
        m_sdf1(std::move(sdf1))
    {
    }

protected:
    TFloatType Evaluate_Impl(const sf::Vector2<TFloatType>& position) const override
    {
        return std::max(m_sdf0->Evaluate(position), m_sdf1->Evaluate(position));
    }

    virtual bool DisplayTreeNode() const override
    {
        return ImGui::TreeNode("Intersection");
    }

    virtual void DrawDebug_Impl(int& elementID) override
    {
        Base::DrawDebug_Impl(elementID);
        m_sdf0->DrawDebug(elementID);
        m_sdf1->DrawDebug(elementID);
    }
};