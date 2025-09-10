#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <memory>

#include "Canvas.h"
#include "SDFs/SDF.h"
#include "GFs/GF.h"

#include "Alias.h"

#include "Utils/Profiler.h"
#include "Utils/ThreadPool.h"

#include "Physics/Verlet/VerletSolver.h"

#include "Triangle.h"

using namespace Alias;
using namespace Configuration;

class Application
{
    enum class RenderOption
    {
        InOut,
        Distance,
        Gradient,
    };

public:
    sf::Vector2i MousePosition;

private:
    sf::RenderWindow& m_window;

    Canvas m_canvas;
    sf::RenderTexture m_renderTexture;

    SDF_UPtr<Configuration::FloatType> m_sceneSDF;
    GF_UPtr<Configuration::FloatType> m_sceneGF;

    Triangle<Configuration::FloatType> m_triangle;

    ThreadPool m_threadPool;

    VerletSolver<Configuration::FloatType> m_physicsSolver;


    std::array<double, 60> m_deltaTimes;
    int m_currentDTIndex = 0;
    float m_captureAverageDeltaTime;

    Profiler& m_profiler;

    float m_directionSinFactor = 3.0f;

    RenderOption m_currentRenderOption;
    RenderOption m_allRenderOptions[3] = {RenderOption::InOut, RenderOption::Distance,  RenderOption::Gradient };
    bool m_redChannel = true;
    bool m_greenChannel = true;
    bool m_blueChannel = true;

public:
    Application(sf::RenderWindow& window, Profiler& profiler);

    void Update(const sf::Time& deltaTime);
    void FixedUpdate(float fixedDeltaTime);
    void Draw();
    void DrawImGui(const sf::Time& deltaTime);

private:
    void ComputeSceneSDF(bool forceReevaluation = false);
    sf::Color GetPointColorSDF(uint32_t x, uint32_t y);
    SDF_UPtr<Configuration::FloatType> CreateSceneSDF();

    void ComputeSceneGF(bool forceReevaluation = false);
    GF_UPtr<Configuration::FloatType> CreateSceneGF();

    const char* ToString(RenderOption option);
};