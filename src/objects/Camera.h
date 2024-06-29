#ifndef CAMERA_H
#define CAMERA_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../utils/Shader.h"


enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float SPEED       =  5.f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class Camera {
public:
    // Camera Position + Directions
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // Camera Angles
    float Yaw;
    float Pitch;

    // Camera Stats
    float Speed;
    float MouseSensitivity;

    float fov;
    float aspectRatio;   
    float nearZ;
    float farZ; 

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.f, float pitch = 0.0f, float fov = 45.f, float ratio = 16.f / 9.f, float nearZ = 0.1f, float farZ = 100.f);

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, float fov, float ratio, float nearZ, float farZ);

    glm::mat4 getViewMat() {
        return glm::lookAt(
            Position,
            Position + Front,
            Up
        );
    }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    void ProcessMouseInput(float xoffset, float yoffset, bool constrainPitch);

    void UpdateShader(std::unique_ptr<ShaderProgram> &shader);


private:
    void updateCameraVectors();
};

#endif