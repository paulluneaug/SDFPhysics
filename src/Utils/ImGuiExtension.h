#pragma once
#include <imgui.h>

class ImGuiExtension
{
public:
    template<typename TFloatType>
    static bool GenericDragFloat(const char* label, TFloatType* variable, float speed = 1.0f, float min = 0.0f, float max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0)
    {
        float castValue = static_cast<float>(*variable);
        if (ImGui::DragFloat(label, &castValue, speed, min, max, format, flags))
        {
            *variable = static_cast<TFloatType>(castValue);
            return true;
        }
        return false;
    }

    template<typename TFloatType>
    static bool GenericDragFloat2(const char* label, sf::Vector2<TFloatType>* variable, float speed = 1.0f, float min = 0.0f, float max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0)
    {
        float castValue[2] = { static_cast<float>(variable->x), static_cast<float>(variable->y) };
        if (ImGui::DragFloat2(label, castValue, speed, min, max, format, flags))
        {
            *variable = { static_cast<TFloatType>(castValue[0]), static_cast<TFloatType>(castValue[1]) };
            return true;
        }
        return false;
    }

};