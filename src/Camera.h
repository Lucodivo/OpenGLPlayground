#pragma once

// Directly pulled from: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/camera.h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "LearnOpenGLPlatform.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CameraMovement
{
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  JUMP
};

const float64 PITCH = 0.0f;
const float64 YAW = -90.0f;
const float32 CAMERA_SPEED = 2.5f;
const float32 SENSITIVITY = 0.1f;
const float32 ZOOM = 45.0f;
const float32 JUMP_SPEED = 1;
const float32 stickSensitivity = 0.00007f;

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
  float64 Yaw;
  float64 Pitch;

  // Camera options
  float32 MovementSpeed;
  float32 MouseSensitivity;
  float32 Zoom;
  glm::vec3 deltaPosition = glm::vec3(0.0f, 0.0f, 0.0f);

  // Jump values
  bool jumping = false;
  float32 jumpVal = 0.0f;
  bool groundedMovement = true;

  // Constructor with vectors
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
          glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
          float64 yaw = YAW,
          float64 pitch = PITCH);

  // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
  glm::mat4 GetViewMatrix(float32 deltaTime);
  glm::mat4 lookAt();
  // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
  void ProcessInput(CameraMovement direction);
  void ProcessLeftAnalog(int16 stickX, int16 stickY, GLboolean constrainPitch = true);
  // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
  void ProcessMouseMovement(float64 xoffset, float64 yoffset, GLboolean constrainPitch = true);
  void ProcessRightAnalog(int16 stickX, int16 stickY, GLboolean constrainPitch = true);
  // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
  void ProcessMouseScroll(float32 yoffset);

private:
  // Calculates the front vector from the Camera's (updated) Eular Angles
  void updateCameraVectors();
  void changePositioning(float32 deltaTime);
};