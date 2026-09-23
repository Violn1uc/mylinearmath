cmake_minimum_required(VERSION 4.4.3)

project(neuralnetwork LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------------------------------------------------------------------------
# Neural network library
# ---------------------------------------------------------------------------

add_library(neuralnetwork STATIC
    src/NeuralNetwork.cpp
    src/Logger.cpp
)

target_include_directories(neuralnetwork
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ---------------------------------------------------------------------------
# Main application
# ---------------------------------------------------------------------------

add_executable(neuralnetwork_app
    src/main.cpp
)

target_link_libraries(neuralnetwork_app
    PRIVATE
        neuralnetwork
)

# ---------------------------------------------------------------------------
# Visual Studio debugger
# ---------------------------------------------------------------------------

set_target_properties(neuralnetwork_app PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY
        "${CMAKE_CURRENT_SOURCE_DIR}"
)