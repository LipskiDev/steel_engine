#ifndef CAMERA_H
#define CAMERA_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../utils/shader.h"


enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float kSpeed       =  5.f;
const float kSensitivity =  0.1f;
const float kZoom        =  45.0f;

class Camera {
public:
    // Camera Position + Directions
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;
    glm::vec3 world_up_;

    // Camera Angles
    float yaw_;
    float pitch_;

    // Camera Stats
    float speed_;
    float mouse_sensitivity_;

    float fov_;
    float aspect_ratio_;   
    float near_z_;
    float far_z_; 

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.f, float pitch = 0.0f, float fov = 45.f, float ratio = 16.f / 9.f, float nearZ = 0.1f, float farZ = 100.f);

    Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch, float fov, float ratio, float near_z, float far_z);

    glm::mat4 GetViewMat() {
        return glm::lookAt(
            position_,
            position_ + front_,
            up_
        );
    }

    void ProcessKeyboard(CameraMovement direction, float delta_time);

    void ProcessMouseInput(float x_offset, float y_offset, bool constrain_pitch);

    void UpdateShader(std::unique_ptr<ShaderProgram> &shader);


private:
    void UpdateCameraVectors();
};

#endif