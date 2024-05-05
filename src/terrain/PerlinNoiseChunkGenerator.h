
#ifndef PERLIN_NOISE_CHUNK_GENERATOR_H
#define PERLIN_NOISE_CHUNK_GENERATOR_H

#include "../model/Mesh.h"
#include <vector>

class PerlinNoiseChunkGenerator {
public:
    PerlinNoiseChunkGenerator();
    std::vector<int> calculateIndices();
    std::vector<float> generateNoiseMap(int xOffset, int zOffset);
    std::vector<float> generateVertices(const std::vector<float> &noiseMap);
    std::vector<float> generateNormals(const std::vector<int> &indices, const std::vector<float> &vertices);
    

    void generateMapChunk(uint32_t &VAO, int xOffset, int zOffset);

    void renderChunk(int xChunk, int zChunk);

    int getChunkWidth() {return chunkWidth; };
    int getChunkHeight() {return chunkHeight; };
    float getWaterHeight() { return waterHeight; };
    float getMeshHeight() { return meshHeight; };
    void generateAllChunks();

    // Noise parameters
    int octaves = 8;
    float meshHeight = 64;
    float noiseScale = 128;
    float persistence = 0.5;
    float lacunarity = 2;

private:


    // Map parameters
    float waterHeight = 0.1f;
    int chunkRenderDistance = 3;
    int xMapChunks = 1;
    int zMapChunks = 1;
    int chunkWidth = 256;
    int chunkHeight = 256;
    int gridPosX = 0;
    int gridPosZ = 0;
    float originX = (chunkWidth  * xMapChunks) / 2 - chunkWidth / 2;
    float originZ = (chunkHeight  * zMapChunks) / 2 - chunkHeight / 2;

    std::vector<uint32_t> chunks;

};

#endif // PERLIN_NOISE_CHUNK_GENERATOR_H