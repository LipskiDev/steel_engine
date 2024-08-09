#include "camera.h"

#include <iostream>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float fov, float ratio, float near_z, float far_z) {
    front_ = glm::vec3(glm::vec3(0.0f, 0.0f, -1.0f));
    speed_ = kSpeed;
    mouse_sensitivity_ = kSensitivity;
    position_ = position;
    world_up_ = up;
    yaw_ = yaw;
    pitch_ = pitch;
    fov_ = fov;
    aspect_ratio_ = ratio;
    near_z_ = near_z;
    far_z_ = far_z;
    UpdateCameraVectors();
}

Camera::Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch, float fov, float ratio, float near_z, float far_z) {
    front_ = glm::vec3(glm::vec3(0.0f, 0.0f, -1.0f));
    speed_ = kSpeed;
    mouse_sensitivity_ = kSensitivity;
    position_ = glm::vec3(pos_x, pos_y, pos_z);
    world_up_ = glm::vec3(up_x, up_y, up_z);
    yaw_ = yaw;
    pitch_ = pitch;
    fov_ = fov;
    aspect_ratio_ = ratio;
    near_z_ = near_z;
    far_z_ = far_z;
    UpdateCameraVectors();
}

void Camera::ProcessKeyboard(CameraMovement direction, float delta_time)
{
    float velocity = speed_ * delta_time * 100.f;
    if (direction == FORWARD)
        position_ += front_ * velocity;
    if (direction == BACKWARD)
        position_ -= front_ * velocity;
    if (direction == LEFT)
        position_ -= right_ * velocity;
    if (direction == RIGHT)
        position_ += right_ * velocity;
}

void Camera::ProcessMouseInput(float x_offset, float y_offset, bool constrain_pitch) {
    x_offset *= mouse_sensitivity_;
    y_offset *= mouse_sensitivity_;

    yaw_ += x_offset;
    pitch_ += y_offset;

    if(constrain_pitch) {
        if (pitch_ > 89.0f) pitch_ = 89.0f;
        if (pitch_ < -89.0f) pitch_ = -89.0f;
    }

    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front);
    // also re-calculate the Right and Up vector
    right_ = glm::normalize(glm::cross(front_, world_up_));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up_    = glm::normalize(glm::cross(right_, front_));
}


void Camera::UpdateShader(std::unique_ptr<ShaderProgram> &shader) {
    glm::mat4 view = GetViewMat();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect_ratio_, near_z_, far_z_);

    shader->SetMat4Uniform("projection", projection);
    shader->SetMat4Uniform("view", view);
    shader->SetVec3Uniform("viewPos", position_.x, position_.y, position_.z);
}