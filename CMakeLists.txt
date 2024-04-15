cmake_minimum_required(VERSION 3.20)
project(steel_engine VERSION 0.1.0)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_STANDARD 17)
set(IMGUI_PATH "dependencies/imgui")

file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp)


add_subdirectory("dependencies/glfw")
add_subdirectory("dependencies/glm")
add_subdirectory("dependencies/gl3w")

set(ROOT_DIR ${CMAKE_SOURCE_DIR})

configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h @ONLY)

add_library("ImGui" STATIC ${IMGUI_SOURCES})

target_include_directories("ImGui" PUBLIC ${IMGUI_PATH})

add_executable(steel_engine 
    src/GatesOfHell.cpp
    src/ui/Display.h src/ui/Display.cpp
    src/math/noise.h
    src/math/Vector.h
    src/objects/Camera.h src/objects/Camera.cpp
    src/terrain/terrain.h src/terrain/terrain.cpp
    src/utils/SceneManager.h src/utils/SceneManager.cpp
    src/input/InputHandler.h src/input/InputHandler.cpp
    src/terrain/PerlinNoiseChunkGenerator.h src/terrain/PerlinNoiseChunkGenerator.cpp
    src/mesh/Mesh.h src/mesh/Mesh.cpp
    src/utils/Shader.h src/utils/Shader.cpp
    src/utils/utils.h src/utils/utils.cpp
    src/objects/DirectionalLight.h src/objects/DirectionalLight.cpp
    lib/stb_image.h lib/stb_image.cpp
    ${IMGUI_PATH}/backends/imgui_impl_opengl3.h ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_PATH}/backends/imgui_impl_glfw.h ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
)

target_link_libraries(steel_engine PUBLIC glfw glm gl3w ImGui)
