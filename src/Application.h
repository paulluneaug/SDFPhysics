#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <memory>

#include "Canvas.h"
#include "SDFs/SDF.h"

#include "Alias.h"

#include "Utils/Profiler.h"

using namespace Alias;
using namespace Configuration;

class Application
{
private:
    sf::RenderWindow& m_window;

    Canvas m_canvas;
    sf::RenderTexture m_renderTexture;

    SDF_Ptr<Configuration::FloatType> m_sceneSDF;


    std::array<double, 60> m_deltaTimes;
    int m_currentDTIndex = 0;
    float m_captureAverageDeltaTime;

    Profiler& m_profiler;

public:
    Application(sf::RenderWindow& window, Profiler& profiler);

    void Update(const sf::Time& deltaTime);
    void Draw();
    void DrawImGui(const sf::Time& deltaTime);

private:
    void ComputeSceneSDF();
    SDF_Ptr<Configuration::FloatType> CreateSceneSDF();
};