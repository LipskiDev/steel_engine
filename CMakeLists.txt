cmake_minimum_required(VERSION 3.20)
project(steel_engine VERSION 0.1.0)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_STANDARD 17)

add_subdirectory("dependencies/glfw")
add_subdirectory("dependencies/glm")
add_subdirectory("dependencies/gl3w")

set(ROOT_DIR ${CMAKE_SOURCE_DIR})

configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h @ONLY)



add_executable(steel_engine 
    src/main.cpp
    src/objects/Camera.h src/objects/Camera.cpp
    src/terrain/terrain.h src/terrain/terrain.cpp
    src/mesh/Mesh.h src/mesh/Mesh.cpp
    src/utils/Shader.h src/utils/Shader.cpp
    src/utils/utils.h src/utils/utils.cpp
    src/objects/DirectionalLight.h src/objects/DirectionalLight.cpp
    lib/stb_image.h
    lib/stb_image.cpp
)

target_link_libraries(steel_engine PUBLIC glfw glm gl3w)
