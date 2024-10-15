
#ifndef PERLIN_NOISE_CHUNK_GENERATOR_H
#define PERLIN_NOISE_CHUNK_GENERATOR_H

#include "../rendering/mesh.h"
#include <vector>

class PerlinNoiseChunkGenerator {
public:
    PerlinNoiseChunkGenerator();
    std::vector<int> CalculateIndices();
    std::vector<float> GenerateNoiseMap(int xOffset, int zOffset);
    std::vector<float> GenerateVertices(const std::vector<float> &noiseMap);
    std::vector<float> GenerateNormals(const std::vector<int> &indices, const std::vector<float> &vertices);
    

    void GenerateMapChunk(uint32_t &VAO, int xOffset, int zOffset);

    void RenderChunk(int xChunk, int zChunk);

    int GetChunkWidth() {return chunk_width_; };
    int GetChunkHeight() {return chunk_height_; };
    float GetWaterHeight() { return water_height_; };
    float GetMeshHeight() { return mesh_height_; };
    void GenerateAllChunks();

    // Noise parameters
    int octaves_ = 8;
    float mesh_height_ = 64;
    float noise_scale_ = 128;
    float persistence_ = 0.5;
    float lacunarity_ = 2;

private:


    // Map parameters
    float water_height_ = 0.1f;
    int chunk_render_distance_ = 3;
    int x_map_chunks_ = 1;
    int z_map_chunks_ = 1;
    int chunk_width_ = 256;
    int chunk_height_ = 256;
    int grid_pos_x_ = 0;
    int grid_pos_z_ = 0;
    float origin_x_ = (chunk_width_ * x_map_chunks_) / 2 - chunk_width_ / 2;
    float origin_z_ = (chunk_height_ * z_map_chunks_) / 2 - chunk_height_ / 2;

    std::vector<uint32_t> chunks_;

};

#endif // PERLIN_NOISE_CHUNK_GENERATOR_H