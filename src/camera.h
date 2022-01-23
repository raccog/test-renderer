#pragma once

#include <glm/glm.hpp>

enum class Movement {
    Forward,
    Backward,
    Left,
    Right
};

constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 2.5f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM = 45.0f;

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
    float zoom;

    Camera(glm::vec3 position = glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3 up = glm::vec3{0.0f, 1.0f, 0.0f}, float yaw = YAW, float pitch = PITCH);

    glm::mat4 getViewMatrix() const;

    void movePosition(Movement movement, float deltaTime);

    void rotateCamera(float xOffset, float yOffset, bool constrainPitch = true);

    void zoomCamera(float offset);

private:
    void updateVectors();
};

