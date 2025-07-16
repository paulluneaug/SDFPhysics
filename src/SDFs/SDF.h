#pragma once
#include <SFML/Graphics.hpp>
#include <imgui.h>

template<typename TFloatType>
class SDF
{

public:

    virtual TFloatType Evaluate(const sf::Vector2<TFloatType>& samplePosition) const 
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

protected:
    virtual TFloatType Evaluate_Impl(const sf::Vector2<TFloatType>& samplePosition) const = 0;

    virtual bool DisplayTreeNode() const = 0;
    virtual void DrawDebug_Impl(int& elementID)
    {
    }
};