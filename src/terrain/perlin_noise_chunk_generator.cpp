#include "perlin_noise_chunk_generator.h"
#include "../math/noise.h"

PerlinNoiseChunkGenerator::PerlinNoiseChunkGenerator()
{
    chunks_ = std::vector<uint32_t>(x_map_chunks_ * z_map_chunks_);

    GenerateAllChunks();
}

void PerlinNoiseChunkGenerator::GenerateAllChunks()
{
    for(int z = 0; z < z_map_chunks_; z++) {
        for(int x = 0; x < x_map_chunks_; x++) {
            GenerateMapChunk(chunks_[x + z * x_map_chunks_], x, z);
        }
    }
}

void PerlinNoiseChunkGenerator::GenerateMapChunk(uint32_t &vao, int x_offset, int z_offset)
{
    std::vector<int> indices;
    std::vector<float> noise_map;
    std::vector<float> vertices;
    std::vector<float> normals;

    indices = CalculateIndices();
    noise_map = GenerateNoiseMap(x_offset, z_offset);
    vertices = GenerateVertices(noise_map);
    normals = GenerateNormals(indices, vertices);

    uint32_t vbo[2], ebo;

    glGenBuffers(2, vbo);
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    // Vertex Position Attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(1);
}

std::vector<int> PerlinNoiseChunkGenerator::CalculateIndices()
{
    std::vector<int> indices;
    
    for (int z = 0; z < chunk_height_; z++)
        for (int x = 0; x < chunk_width_; x++) {
            int pos = x + z * chunk_width_;
            
            if (x == chunk_width_ - 1 || z == chunk_height_ - 1) {
                // Don't create indices for right or top edge
                continue;
            } else {
                // Top left triangle of square
                indices.push_back(pos + chunk_width_);
                indices.push_back(pos);
                indices.push_back(pos + chunk_width_ + 1);
                // Bottom right triangle of square
                indices.push_back(pos + 1);
                indices.push_back(pos + 1 + chunk_width_);
                indices.push_back(pos);
            }
        }

    return indices;
}

std::vector<float> PerlinNoiseChunkGenerator::GenerateNoiseMap(int x_offset, int z_offset)
{
    std::vector<float> noise_values;
    std::vector<float> normalized_noise_values; 
    std::vector<int> p = GetPermutationVector();

    float amplitude = 1.0f;
    float frequency = 1.0f;
    float maximum_height = 0.f;

    for(int i = 0; i < octaves_; i++) {
        maximum_height += amplitude;
        amplitude *= persistence_;
    }

    for(int z = 0; z < chunk_height_; z++) {
        for(int x = 0; x < chunk_width_; x++) {
            amplitude = 1.f;
            frequency = 1.f;

            float height = 0.f;

            for(int i = 0; i < octaves_; i++) {
                float x_sample = (x + x_offset * (chunk_width_ - 1)) / noise_scale_ * frequency;
                float z_sample = (z + z_offset * (chunk_height_ - 1)) / noise_scale_ * frequency;

                float perlin_value = PerlinNoise(x_sample, z_sample, p);
                height += perlin_value * amplitude;

                amplitude *= persistence_;
                frequency *= lacunarity_;
            }

            noise_values.push_back(height);
        }
    }

    for(int z = 0; z < chunk_height_; z++) {
        for(int x = 0; x < chunk_width_; x++) {
            normalized_noise_values.push_back((noise_values[x + z * chunk_width_] + 1) / maximum_height); 
        }
    }

    return normalized_noise_values;
}

std::vector<float> PerlinNoiseChunkGenerator::GenerateVertices(const std::vector<float> &noise_map)
{
    std::vector<float> v;
    
    for (int z = 0; z < chunk_height_; z++) {
        for (int x = 0; x < chunk_width_; x++) {
            v.push_back(x);

            float eased_noise = std::pow(noise_map[x + z * chunk_width_] * 1.1, 3);
            v.push_back(std::fmax(eased_noise * mesh_height_, water_height_ * 0.5 * mesh_height_));
            v.push_back(z);
        }
    }
    return v;
}

std::vector<float> PerlinNoiseChunkGenerator::GenerateNormals(const std::vector<int> &indices, const std::vector<float> &vertices)
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

        glm::vec3 U = verts[i + 1] - verts[i]; // 
        glm::vec3 V = verts[i + 2] - verts[i];

        normal = glm::normalize(-glm::cross(U, V));
        normals.push_back(normal.x);
        normals.push_back(normal.y);
        normals.push_back(normal.z);
    }

    return normals;
}

void PerlinNoiseChunkGenerator::RenderChunk(int x_chunk, int z_chunk)
{
    
    glBindVertexArray(chunks_[x_chunk + z_chunk * x_map_chunks_]);
    glDrawElements(GL_TRIANGLES, chunk_width_ * chunk_height_ * 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


