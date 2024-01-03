#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include <random>

#include "../utils/Shader.h"
#include "../mesh/Mesh.h"

class BaseTerrain {
public:
    BaseTerrain(int _terrainSize, float worldScale);

    float max = -INFINITY;
    float min = INFINITY;

    void initTerrain();
    void Render();
    float getHeight(int x, int y);
    float getWorldScale();
    void diamondSquare(int size);
    Shader getShader() {return sh; };
    void setShader(Shader sh) { this->sh = sh; };

private:
    Mesh m;
    Shader sh;
    
    int terrainSize = 0;
    float worldScale = 1.0f;
    std::vector<std::vector<float>> heightMap;
    float height = 7.0f;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;

    void diamondStep(int half, int step);
    void squareStep(int half, int step);

};

#endif