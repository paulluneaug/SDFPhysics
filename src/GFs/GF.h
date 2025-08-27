#pragma once
#include <SFML/Graphics.hpp>
#include <imgui.h>

template<typename TFloatType>
class GF
{
private:
    bool m_changed;

public:

    GF() :
        m_changed(true)
    {

    }

    virtual sf::Vector2<TFloatType> Evaluate(const sf::Vector2<TFloatType>& samplePosition) const 
    {
        return Evaluate_Impl(samplePosition);
    }

    void DrawDebug(int& elementID)
    {
        ImGui::PushID(elementID++);
        if (DisplayTreeNode())
        {
            DrawDebug_Impl(elementID);
            ImGui::TreePop();
        }
        ImGui::PopID();
    }

    virtual bool RequireReevaluation() const
    {
        return m_changed;
    }

    virtual void OnGFReevaluated()
    {
        m_changed = false;
    }

protected:
    virtual sf::Vector2<TFloatType> Evaluate_Impl(const sf::Vector2<TFloatType>& samplePosition) const = 0;

    virtual bool DisplayTreeNode() const = 0;
    virtual void DrawDebug_Impl(int& elementID)
    {
    }

    void OnGFChanged() 
    {
        m_changed = true;
    }
};