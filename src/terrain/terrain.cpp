#include "terrain.h"

#include <cmath>
#include <ostream>
#include <iostream>
#include <iomanip>



BaseTerrain::BaseTerrain(int n, float worldScale)
{
    this->terrainSize = pow(2, n) + 1;
    this->worldScale = worldScale;

    gen = std::mt19937(rd());
    dis = std::uniform_real_distribution(-height, height);

    generateTerrain();
}

// Using Diamond-Square Algorithm
void BaseTerrain::initTerrain()
{
    min = INFINITY;
    max = -INFINITY;

    heightMap = std::vector<std::vector<float>>(terrainSize);

    for(int i = 0; i < terrainSize; i++) {
        heightMap[i] = std::vector<float>(terrainSize);
    }

    for(int x = 0; x < terrainSize; x++) {
        for(int z = 0; z < terrainSize; z++) {
            heightMap[x][z] = 0.f;
        }
    }
    
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

void BaseTerrain::generateTerrain()
{
    initTerrain();
    fillMesh();   
}

void BaseTerrain::fillMesh()
{
    m.clearMesh();
    std::vector<Vertex> verts;
    std::vector<unsigned int> indices;

    for(int z = 0; z < terrainSize; z++) {
        for(int x = 0; x < terrainSize; x++) {
            glm::vec3 normal(1.f);
            Vertex v;
            if (x > 0 && x < terrainSize - 1 && z > 0 && z < terrainSize - 1) {
                float left = heightMap[x - 1][z];
                float right = heightMap[x + 1][z];
                float up = heightMap[x][z + 1];
                float down = heightMap[x][z - 1];

                float gradient_x = (right - left) / 2.0f;
                float gradient_z = (up - down) / 2.0f;

                normal = glm::vec3(-gradient_x, 1.0f, -gradient_z);
                float magnitude = pow(pow(normal.x, 2) + pow(normal.y, 2) + pow(normal.z, 2), 0.5);
                normal = glm::vec3(normal.x / magnitude, normal.y / magnitude, normal.z / magnitude);
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



