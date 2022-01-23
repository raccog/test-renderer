/*
 *  Derived from https://learnopengl.com/Getting-started/Camera
 */
#pragma once

#include <glm/glm.hpp>

// Movements in a 3D environment
enum class Movement {
    Forward,
    Backward,
    Left,
    Right
};

// Camera constants
constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 2.5f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM = 45.0f;

// A viewpoint in a 3D environment
class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;
    float zoomOffset;

    Camera(glm::vec3 position = glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3 up = glm::vec3{0.0f, 1.0f, 0.0f}, float yaw = YAW, float pitch = PITCH);

    glm::mat4 getViewMatrix() const;

    void move(Movement movement, float deltaTime);

    void rotate(float xOffset, float yOffset, bool constrainPitch = true);

    void zoom(float offset);

private:
    void updateVectors();
};

