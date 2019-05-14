#pragma once

// Directly pulled from: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/camera.h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    JUMP
};

const float PITCH = 0.0f;
const float YAW = -90.0f;
const float SPEED = 2.5f;
const float SENSITIVTY = 0.1f;
const float ZOOM = 45.0f;
const float JUMP_SPEED = 1;


class Camera
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Eular Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    glm::vec3 deltaPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    // Jump values
    bool jumping = false;
    float jumpVal = 0.0f;

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix(float deltaTime)
    {
        changePositioning(deltaTime);

        return lookAt();
    }

    glm::mat4 lookAt() {
        glm::vec3 target = Position + Front;

        // Calculate cameraDirection
        glm::vec3 zaxis = glm::normalize(Position - target);
        // Get positive right axis vector
        glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(Up), zaxis));
        // Calculate camera up vector
        glm::vec3 yaxis = glm::cross(zaxis, xaxis);

        // In glm we access elements as mat[col][row] due to column-major layout
        auto translation = glm::mat4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            -Position.x, -Position.y, -Position.z, 1.0f);
        auto rotation = glm::mat4(
            xaxis.x, yaxis.x, zaxis.x, 0.0f,
            xaxis.y, yaxis.y, zaxis.y, 0.0f,
            xaxis.z, yaxis.z, zaxis.z, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);

        // Return lookAt matrix as combination of translation and rotation matrix
        return rotation * translation; // Remember to read from right to left (first translation then rotation)
    }

    void changePositioning(float deltaTime) {
        if (jumping) {
            jumpVal += JUMP_SPEED * 0.08f;
            float verticalOffset = sin(jumpVal) / 1.3f;
            if (verticalOffset < 0.0f) {
                Position.y = 0.0f;
                jumpVal = 0.0f;
                jumping = false;
            } else {
                Position.y = verticalOffset;
            }
        }

        // multiplying a vec3(0,0,0) by small fractions may lead to NAN values
        if (deltaPosition.x != 0.0f || deltaPosition.y != 0.0f || deltaPosition.z != 0.0f) {
            // normalizing the deltaPosition helps:
            // - accomodate for slower movement when looking up or down
            //      due to the front.xz values creating a < 1 magnitude vector
            float velocity = MovementSpeed * deltaTime;            
            Position += glm::normalize(deltaPosition) * velocity;
        }
        deltaPosition = glm::vec3(0.0f);
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction)
    {
        if (direction == FORWARD) deltaPosition += glm::vec3(Front.x, 0.0f, Front.z);
        if (direction == BACKWARD) deltaPosition -= glm::vec3(Front.x, 0.0f, Front.z);
        if (direction == LEFT) deltaPosition -= Right;
        if (direction == RIGHT) deltaPosition += Right;

        if (!jumping && direction == JUMP) jumping = true;
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch) {
            if (Pitch > 89.0f) Pitch = 89.0f;
            if (Pitch < -89.0f) Pitch = -89.0f;
        }

        // Update Front, Right and Up Vectors using the updated Eular angles
        updateCameraVectors();
    }

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        if (Zoom >= 1.0f && Zoom <= 45.0f) Zoom -= yoffset;
        if (Zoom <= 1.0f) Zoom = 1.0f;
        if (Zoom >= 45.0f) Zoom = 45.0f;
    }

private:
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};