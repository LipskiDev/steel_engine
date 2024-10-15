#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include <random>

#include "../utils/shader.h"
#include "../rendering/mesh.h"

class BaseTerrain {
public:
    virtual void fillMesh() = 0;
    virtual void Render() = 0;
    void setShader(Shader sh) { 
        //this->sh = sh; 
    };

protected: 
    Mesh m;
    Shader sh;
};

class DiamondSquareTerrain : protected BaseTerrain{
public:
    DiamondSquareTerrain(int _terrainSize, float worldScale, Shader bt);

    float max = -INFINITY;
    float min = INFINITY;

    void initTerrain();
    void Render();
    float getHeight(int x, int y);
    float getWorldScale();
    void diamondSquare(int size);
    void generateTerrain();
    void fillMesh();
    void updateHeight(float scale);

private:    
    int terrainSize = 0;
    float worldScale = 1.0f;
    std::vector<std::vector<float>> heightMap;
    float height = 20.0f;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;

    int maxLocation = -1;
    int minLocation = -1;

    void diamondStep(int half, int step);
    void squareStep(int half, int step);

};

#endif