cmake_minimum_required(VERSION 3.28)
project(SDFPhysics LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)
FetchContent_Declare(ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.91.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(ImGui)
FetchContent_GetProperties(ImGui SOURCE_DIR IMGUI_DIR)
set(IMGUI_SFML_FIND_SFML OFF)
FetchContent_Declare(ImGui-SFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    GIT_TAG v3.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(ImGui-SFML)

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/Configuration.h
    src/Events.h
    src/Events.cpp
    src/Application.h
    src/Application.cpp

    src/Math.h
    src/Math.cpp

    src/Utils/Profiler.h
    src/Utils/Profiler.cpp
    src/Utils/ThreadPool.h

    src/Canvas.h
    src/Canvas.cpp
    
    src/SDFs/SDF.h
    src/SDFs/AllSDFs.h

    src/SDFs/Operators/UnionOperator.h
    src/SDFs/Operators/SmoothUnionOperator.h
    src/SDFs/Operators/IntersectionOperator.h

    src/SDFs/Primitives/Circle.h
    src/SDFs/Primitives/Box.h
    src/SDFs/Primitives/CoolS.h
    
    src/Physics/Verlet/VerletSolver.h
    src/Physics/Verlet/VerletObject.h
    )

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics ImGui-SFML::ImGui-SFML)
