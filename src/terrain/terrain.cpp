#include "terrain.h"

void BaseTerrain::initTerrain(float _worldScale)
{
    
}

void BaseTerrain::Render()
{
    
}

float BaseTerrain::getHeight(int x, int y)
{
    return heightMap[x][y];
}

float BaseTerrain::getWorldScale()
{
    return worldScale;
}
