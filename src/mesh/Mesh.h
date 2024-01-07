#ifndef MESH_H
#define MESH_H

#include <GL/gl3w.h>

#include <stdio.h>
#include <vector>

#include "../utils/Shader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
};

class Mesh {
private:
    GLuint VBO, EBO, VAO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    void setupMesh();
    int modelLocation;
    glm::vec3 worldPosition = glm::vec3(0.0f, 0.0f, 0.0f);
public:
    Mesh() {};
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    void Draw(Shader &shader);
    void clearMesh();
};

#endif