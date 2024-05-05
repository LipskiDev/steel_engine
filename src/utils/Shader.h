#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>
#include <unordered_map>
#include <string>
#include "../objects/DirectionalLight.h"
#include "../objects/Camera.h"

class Shader {
public:
    unsigned int shaderProgramID;

    Shader(){};
    Shader(const char *vertPath, const char *fragPath);

    int getUniformLocation(const char *uniformName) const;
    static void setMat4(int uniformLocation, glm::mat4 mat);
    static void setFloat(int uniformLocation, float value);
    static void setVec3(int uniformLocation, glm::vec3);
    static void setInt(int uniformLocation, int);

    void bind() const;
    static void unbind();
    void addDirectionalLight(DirectionalLight light);
    void addCamera(Camera *cam);
    void updateCamera();
private:
    static unsigned int createShader(const char *path, unsigned int type);
    Camera *camera;

    int viewLocation;
    int projectionLocation;
    int viewPosLocation;

};

#endif // SHADER_H