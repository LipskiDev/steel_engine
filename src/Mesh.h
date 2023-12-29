#ifndef MESH_H
#define MESH_H

#include <stdio.h>
#include <vector>

#include "Shader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
};

class Mesh {
private:
    GLuint VBO, EBO, VAO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
        
        glEnableVertexAttribArray(1);	
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

        glBindVertexArray(0);
    }
public:
    Mesh() {};
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
        this->vertices = vertices;
        this->indices = indices;
        setupMesh();
    }
    void Draw(Shader &shader) {
        shader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

#endif