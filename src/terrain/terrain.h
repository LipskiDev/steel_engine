#include <vector>

class BaseTerrain {
public:
    BaseTerrain();

    void initTerrain(float _worldScale);
    void Render();
    float getHeight(int x, int y);
    float getWorldScale();

private:
    int terrainSize = 0;
    float worldScale = 1.0f;
    std::vector<std::vector<float>> heightMap;
};