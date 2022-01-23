#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) 
        : front{glm::vec3{0.0f, 0.0f, -1.0f}}, movementSpeed{SPEED}, mouseSensitivity{SENSITIVITY}, zoom{ZOOM} {
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    updateVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::movePosition(Movement movement, float deltaTime) {
    const float velocity = movementSpeed * deltaTime;
    switch (movement) {
        case Movement::Forward:
            position += front * velocity;
            break;
        case Movement::Backward:
            position -= front * velocity;
            break;
        case Movement::Left:
            position -= right * velocity;
            break;
        case Movement::Right:
            position += right * velocity;
            break;
    }
}

void Camera::rotateCamera(float xOffset, float yOffset, bool constrainPitch) {
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if (constrainPitch) {
        if (pitch > 89.0f) {
            pitch = 89.0f;
        } else if (pitch < -89.0f) {
            pitch = -89.0f;
        }
    }

    updateVectors();
}

void Camera::updateVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->front = glm::normalize(front);
    right = glm::normalize(glm::cross(this->front, worldUp));
    up = glm::normalize(glm::cross(right, this->front));
}

void Camera::zoomCamera(float offset) {
    zoom -= offset;
    if (zoom < 1.0f) {
        zoom = 1.0f;
    } else if (zoom > 45.0f) {
        zoom = 45.0f;
    }
}
