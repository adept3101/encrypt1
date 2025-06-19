cmake_minimum_required(VERSION 3.10)
project(ImGuiProject)

set(CMAKE_CXX_STANDARD 11)

# GLFW
add_subdirectory(glfw)
include_directories(glfw/include)

# GLEW
add_definitions(-DGLEW_STATIC)
include_directories(glew-2.2.0/include)
link_directories(glew-2.2.0/lib/Release/x64)
#file(GLOB GLEW_SRC glew-2.2.0/src/glew.c)

# ImGui
include_directories(imgui)
file(GLOB IMGUI_SRC
    imgui/*.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

add_executable(main main.cpp encrypt.cpp decrypt.cpp ${IMGUI_SRC} ${GLEW_SRC})
target_link_libraries(main GLEW glfw OpenGL)
