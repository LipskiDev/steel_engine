#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>

#include "../Shader.h"
#include "../Mesh.h"

class BaseTerrain {
public:
    BaseTerrain(int _terrainSize, float worldScale);

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
    float height = 7.f;

    void diamondStep(int x, int z, int step);
    void squareStep(int x, int z, int step);

};

#endif