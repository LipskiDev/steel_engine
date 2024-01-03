#include "terrain.h"

#include <cmath>
#include <ostream>
#include <iostream>
#include <iomanip>



BaseTerrain::BaseTerrain(int n, float worldScale)
{
    gen = std::mt19937(rd());
    dis = std::uniform_real_distribution(-height, height);
    this->worldScale = worldScale;
    // Grid must be 2^n + 1 for Diamond-Square Algorithm
    terrainSize = pow(2, n) + 1;
    heightMap = std::vector<std::vector<float>>(terrainSize);

    for(int i = 0; i < terrainSize; i++) {
        heightMap[i] = std::vector<float>(terrainSize);
    }

    heightMap[0][0];
    initTerrain();
    std::vector<Vertex> verts;
    std::vector<unsigned int> indices;

    for(int z = 0; z < terrainSize; z++) {
        for(int x = 0; x < terrainSize; x++) {
            glm::vec3 normal(0.f);
            Vertex v;
            if (x > 0 && x < terrainSize - 1 && z > 0 && z < terrainSize - 1) {
                glm::vec3 v1 = glm::vec3(0, heightMap[x - 1][z] - heightMap[x + 1][z], 0);
                glm::vec3 v2 = glm::vec3(0, heightMap[x][z - 1] - heightMap[x][z + 1], 0);
                normal = glm::normalize(glm::cross(v1, v2));
            }
            v.Position = glm::vec3(worldScale * x, getHeight(x, z), worldScale * z);
            v.Normal = normal;

            verts.push_back(v);
            if(x < terrainSize - 1 && z < terrainSize - 1) {
                int ind = x + terrainSize * z;
                // first triangle
                indices.push_back(ind);
                indices.push_back(ind + 1);
                indices.push_back(ind + terrainSize);

                // second triangle
                indices.push_back(ind + 1);
                indices.push_back(ind + terrainSize);
                indices.push_back(ind + terrainSize + 1);
            }
        }
    }

    m = Mesh(verts, indices);
}

// Using Diamond-Square Algorithm
void BaseTerrain::initTerrain()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-height, height);

    // Init Corners
    heightMap[0][0] = dis(gen);
    heightMap[terrainSize - 1][0] = dis(gen);
    heightMap[0][terrainSize - 1] = dis(gen);
    heightMap[terrainSize - 1][terrainSize - 1] = dis(gen);

    diamondSquare(terrainSize);


}

void BaseTerrain::diamondSquare(int size) {
    int step = size;

    while(step > 1) {
        int half = step / 2;

        diamondStep(half, step);
        squareStep(half, step);

        step /= 2;
        height /= 2;

    }
}

void BaseTerrain::diamondStep(int half, int step) {
    for(int x = half; x < terrainSize; x += step) {
        for(int z = half; z < terrainSize; z += step) {
            float cornerSum = 0.f;
            cornerSum += heightMap[x - half][z - half];
            cornerSum += heightMap[x + half][z - half];
            cornerSum += heightMap[x - half][z + half];
            cornerSum += heightMap[x + half][z + half];

            cornerSum /= 4;
            cornerSum += dis(gen) * height;
            heightMap[x][z] = cornerSum;
            if(cornerSum > max) max = cornerSum;
            if(cornerSum < min) min = cornerSum;
        }
    }
    
}

void BaseTerrain::squareStep(int half, int step)
{
    for (int x = 0; x < terrainSize; x += half) {
	    for (int z = (x + half) % step; z < terrainSize; z += step) {
            float cornerSum = 0.f;
            int count = 0;

		    if (x - half >= 0) {
		    	cornerSum += heightMap[x - half][z];
		    	count++;
		    }
		    
            if (x + half < terrainSize) {
		    	cornerSum += heightMap[x + half][z];
		    	count++;
		    }
		    
            if (z - half >= 0) {
		    	cornerSum += heightMap[x][z - half];
		    	count++;
		    }
		    
            if (z + half < terrainSize) {
		    	cornerSum += heightMap[x][z + half];
		    	count++;
		    }

            cornerSum /= count;
            cornerSum += dis(gen) * height;
            heightMap[x][z] = cornerSum;
            if(cornerSum > max) max = cornerSum;
            if(cornerSum < min) min = cornerSum;
        }
    }
}

void BaseTerrain::Render()
{
    m.Draw(sh);
}

float BaseTerrain::getHeight(int x, int z)
{
    return heightMap[x][z];
}

float BaseTerrain::getWorldScale()
{
    return worldScale;
}



