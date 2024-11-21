cmake_minimum_required(VERSION 3.16)

project(Poker VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_WAYLAND OFF)
add_subdirectory(thirdparty/glfw)
add_subdirectory(thirdparty/glad)

file(GLOB imgui_sources thirdparty/imgui/*.cpp)
file(GLOB imgui_headers thirdparty/imgui/*.h)
add_library(imgui
    ${imgui_sources}
    ${imgui_headers}
    thirdparty/imgui/backends/imgui_impl_glfw.cpp
    thirdparty/imgui/backends/imgui_impl_glfw.h
    thirdparty/imgui/backends/imgui_impl_opengl3.cpp
    thirdparty/imgui/backends/imgui_impl_opengl3.h
)
target_include_directories(imgui PUBLIC thirdparty/imgui thirdparty/glfw/include)

add_executable(poker src/main.cpp 
    src/tester.cpp
    src/pokerlib.cpp
    src/Controller.cpp
    src/5cardHandEvaluator/5cardHandEvaluator.cpp
    src/7cardHandEvaluator/7cardHandEvaluator.cpp
    src/naivePreflop/naive_preflop.cpp)

target_include_directories(poker PUBLIC thirdparty PUBLIC thirdparty/imgui/backends PUBLIC include)

target_link_libraries(poker PUBLIC 
    glfw glad OpenGL::GL imgui
)
