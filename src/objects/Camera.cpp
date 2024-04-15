#include "Camera.h"

#include <iostream>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float fov, float ratio, float nearZ, float farZ) {
    Front = glm::vec3(glm::vec3(0.0f, 0.0f, -1.0f));
    Speed = SPEED;
    MouseSensitivity = SENSITIVITY;
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    this->fov = fov;
    this->aspectRatio = ratio;
    this->nearZ = nearZ;
    this->farZ = farZ;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, float fov, float ratio, float nearZ, float farZ) {
    Front = glm::vec3(glm::vec3(0.0f, 0.0f, -1.0f));
    Speed = SPEED;
    MouseSensitivity = SENSITIVITY;
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    this->fov = fov;
    this->aspectRatio = ratio;
    this->nearZ = nearZ;
    this->farZ = farZ;
    updateCameraVectors();
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = Speed * deltaTime * 100.f;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
}

void Camera::ProcessMouseInput(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if(constrainPitch) {
        if (Pitch > 89.0f) Pitch = 89.0f;
        if (Pitch < -89.0f) Pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up    = glm::normalize(glm::cross(Right, Front));
}