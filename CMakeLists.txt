cmake_minimum_required(VERSION 3.20)
project(steel_engine VERSION 0.1.0)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_STANDARD 20)
set(IMGUI_PATH "dependencies/imgui")
set(ASSIMP_PATH "dependencies/assimp")

file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp)


add_subdirectory("dependencies/glfw")
add_subdirectory("dependencies/glm")
add_subdirectory("dependencies/gl3w")
add_subdirectory("dependencies/assimp")

set(ROOT_DIR ${CMAKE_SOURCE_DIR})

configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h @ONLY)

add_library("ImGui" STATIC ${IMGUI_SOURCES})

target_include_directories("ImGui" PUBLIC ${IMGUI_PATH})


add_executable(steel_engine 
    src/gates_of_hell.cpp
    src/ui/display.h src/ui/display.cpp
    src/math/noise.h
    src/math/vector.h
    src/objects/camera.h src/objects/camera.cpp
    # src/terrain/terrain.h src/terrain/terrain.cpp
    src/utils/scene_manager.h src/utils/scene_manager.cpp
    src/input/input_handler.h src/input/input_handler.cpp
    src/terrain/perlin_noise_chunk_generator.h src/terrain/perlin_noise_chunk_generator.cpp
    src/model/mesh.h 
    src/model/model.h
    src/utils/shader.h src/utils/shader.cpp
    src/utils/utils.h src/utils/utils.cpp
    src/objects/directional_light.h src/objects/directional_light.cpp
    lib/stb_image.h lib/stb_image.cpp
    ${IMGUI_PATH}/backends/imgui_impl_opengl3.h ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_PATH}/backends/imgui_impl_glfw.h ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
)

target_link_libraries(steel_engine PUBLIC glfw glm gl3w ImGui assimp)
