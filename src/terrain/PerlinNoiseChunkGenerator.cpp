#include "PerlinNoiseChunkGenerator.h"
#include "../math/noise.h"

PerlinNoiseChunkGenerator::PerlinNoiseChunkGenerator()
{
    chunks = std::vector<uint32_t>(xMapChunks * zMapChunks);

    for(int z = 0; z < zMapChunks; z++) {
        for(int x = 0; x < xMapChunks; x++) {
            generateMapChunk(chunks[x + z * xMapChunks], x, z);
        }
    }
}

void PerlinNoiseChunkGenerator::generateMapChunk(uint32_t &VAO, int xOffset, int zOffset)
{
    std::vector<int> indices;
    std::vector<float> noise_map;
    std::vector<float> vertices;
    std::vector<float> normals;

    indices = calculateIndices();
    noise_map = generateNoiseMap(xOffset, zOffset);
    vertices = generateVertices(noise_map);
    normals = generateNormals(indices, vertices);

    uint32_t VBO[2], EBO;

    glGenBuffers(2, VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    // Vertex Position Attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(1);
}

std::vector<int> PerlinNoiseChunkGenerator::calculateIndices()
{
    std::vector<int> indices;
    
    for (int z = 0; z < chunkHeight; z++)
        for (int x = 0; x < chunkWidth; x++) {
            int pos = x + z * chunkWidth;
            
            if (x == chunkWidth - 1 || z == chunkHeight - 1) {
                // Don't create indices for right or top edge
                continue;
            } else {
                // Top left triangle of square
                indices.push_back(pos + chunkWidth);
                indices.push_back(pos);
                indices.push_back(pos + chunkWidth + 1);
                // Bottom right triangle of square
                indices.push_back(pos + 1);
                indices.push_back(pos + 1 + chunkWidth);
                indices.push_back(pos);
            }
        }

    return indices;
}

std::vector<float> PerlinNoiseChunkGenerator::generateNoiseMap(int xOffset, int zOffset)
{
    std::vector<float> noiseValues;
    std::vector<float> normalizedNoiseValues;
    std::vector<int> p = getPermutationVector();

    float amplitude = 1.0f;
    float frequency = 1.0f;
    float maximumHeight = 0.f;

    for(int i = 0; i < octaves; i++) {
        maximumHeight += amplitude;
        amplitude *= persistence;
    }

    for(int z = 0; z < chunkHeight; z++) {
        for(int x = 0; x < chunkWidth; x++) {
            amplitude = 1.f;
            frequency = 1.f;

            float height = 0.f;

            for(int i = 0; i < octaves; i++) {
                float xSample = (x + xOffset * (chunkWidth - 1)) / noiseScale * frequency;
                float zSample = (z + zOffset * (chunkHeight - 1)) / noiseScale * frequency;

                float perlinValue = perlinNoise(xSample, zSample, p);
                height += perlinValue * amplitude;

                amplitude *= persistence;
                frequency *= lacunarity;
            }

            noiseValues.push_back(height);
        }
    }

    for(int z = 0; z < chunkHeight; z++) {
        for(int x = 0; x < chunkWidth; x++) {
            normalizedNoiseValues.push_back((noiseValues[x + z * chunkWidth] + 1) / maximumHeight);
        }
    }

    return normalizedNoiseValues;
}

std::vector<float> PerlinNoiseChunkGenerator::generateVertices(const std::vector<float> &noiseMap)
{
    std::vector<float> v;
    
    for (int z = 0; z < chunkHeight; z++) {
        for (int x = 0; x < chunkWidth; x++) {
            v.push_back(x);

            float easedNoise = std::pow(noiseMap[x + z * chunkWidth] * 1.1, 3);
            v.push_back(std::fmax(easedNoise * meshHeight, waterHeight * 0.5 * meshHeight));
            v.push_back(z);
        }
    }
    return v;
}

std::vector<float> PerlinNoiseChunkGenerator::generateNormals(const std::vector<int> &indices, const std::vector<float> &vertices)
{
    int pos;
    glm::vec3 normal;
    std::vector<float> normals;
    std::vector<glm::vec3> verts;

    for(int i = 0; i < indices.size(); i += 3) {
        for(int j = 0; j < 3; j++) {
            pos = indices[i + j] * 3;
            verts.push_back(glm::vec3(vertices[pos], vertices[pos + 1], vertices[pos + 2]));
        }

            glm::vec3 U = verts[i + 1] - verts[i];
            glm::vec3 V = verts[i + 2] - verts[i];

            normal = glm::normalize(-glm::cross(U, V));
            normals.push_back(normal.x);
            normals.push_back(normal.y);
            normals.push_back(normal.z);
    }

    return normals;
}

void PerlinNoiseChunkGenerator::renderChunk(int xChunk, int zChunk)
{
    
    glBindVertexArray(chunks[xChunk + zChunk * xMapChunks]);
    glDrawElements(GL_TRIANGLES, chunkWidth * chunkHeight * 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
