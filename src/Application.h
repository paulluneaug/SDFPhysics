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

using namespace Alias;
using namespace Configuration;

class Application
{
private:
    sf::RenderWindow& m_window;

    Canvas m_canvas;
    sf::RenderTexture m_renderTexture;

    SDF_UPtr<Configuration::FloatType> m_sceneSDF;
    GF_UPtr<Configuration::FloatType> m_sceneGF;

    ThreadPool m_threadPool;

    VerletSolver<Configuration::FloatType> m_physicsSolver;


    std::array<double, 60> m_deltaTimes;
    int m_currentDTIndex = 0;
    float m_captureAverageDeltaTime;

    Profiler& m_profiler;

    float m_directionSinFactor = 3.0f;

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
    SDF_UPtr<Configuration::FloatType> CreateSceneSDF();

    void ComputeSceneGF(bool forceReevaluation = false);
    GF_UPtr<Configuration::FloatType> CreateSceneGF();
};