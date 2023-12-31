cmake_minimum_required(VERSION 3.0.0)
project(steel_engine VERSION 0.1.0)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory("dependencies/glfw")
add_subdirectory("dependencies/glm")
add_subdirectory("dependencies/gl3w")



add_executable(steel_engine 
    src/main.cpp
    src/Shader.h
    src/Shader.cpp
    src/terrain/terrain.h
    src/terrain/terrain.cpp
    src/Mesh.h
    src/Mesh.cpp
    lib/stb_image.h
    lib/stb_image.cpp
)

target_link_libraries(steel_engine PUBLIC glfw glm gl3w)
