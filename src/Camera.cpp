//
// Created by Connor on 11/14/2019.
//

#include "Camera.h"

// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, float32 yaw, float32 pitch)
: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(CAMERA_SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
  Position = position;
  WorldUp = up;
  Yaw = yaw;
  Pitch = pitch;
  updateCameraVectors();
}

// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix(float32 deltaTime)
{
  changePositioning(deltaTime);
  return lookAt();
}

glm::mat4 Camera::lookAt()
{
  glm::vec3 target = Position + Front;

  // Calculate cameraDirection
  glm::vec3 zaxis = glm::normalize(Position - target);
  // Get positive right axis vector
  glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(Up), zaxis));
  // Calculate camera up vector
  glm::vec3 yaxis = glm::cross(zaxis, xaxis);

  // In glm we access elements as mat[col][row] due to column-major layout
  glm::mat4 translation = glm::mat4(
          1.0f, 0.0f, 0.0f, 0.0f,
          0.0f, 1.0f, 0.0f, 0.0f,
          0.0f, 0.0f, 1.0f, 0.0f,
          -Position.x, -Position.y, -Position.z, 1.0f);

  glm::mat4 rotation = glm::mat4(
          xaxis.x, yaxis.x, zaxis.x, 0.0f,
          xaxis.y, yaxis.y, zaxis.y, 0.0f,
          xaxis.z, yaxis.z, zaxis.z, 0.0f,
          0.0f, 0.0f, 0.0f, 1.0f);

  // Return lookAt matrix as combination of translation and rotation matrix
  return rotation * translation; // Remember to read from right to left (first translation then rotation)
}

void Camera::changePositioning(float32 deltaTime)
{
  if (jumping)
  {
    jumpVal += JUMP_SPEED * deltaTime * 6;
    float32 verticalOffset = sin(jumpVal) / 1.3f;
    if (verticalOffset < 0.0f)
    {
      Position.y = 0.0f;
      jumpVal = 0.0f;
      jumping = false;
    } else
    {
      Position.y = verticalOffset;
    }
  }

  // multiplying a vec3(0,0,0) by small fractions may lead to NAN values
  if (deltaPosition.x != 0.0f || deltaPosition.y != 0.0f || deltaPosition.z != 0.0f)
  {
    // normalizing the deltaPosition helps:
    // - accommodate for slower movement when looking up or down
    //      due to the front.xz values creating a < 1 magnitude vector
    float32 velocity = MovementSpeed * deltaTime;
    Position += glm::normalize(deltaPosition) * velocity;
  }
  deltaPosition = glm::vec3(0.0f);
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessInput(Camera_Movement direction)
{
  switch (direction)
  {
    case FORWARD:
      deltaPosition += groundedMovement ? glm::vec3(Front.x, 0.0f, Front.z) : Front;
      break;
    case BACKWARD:
      deltaPosition -= groundedMovement ? glm::vec3(Front.x, 0.0f, Front.z) : Front;
      break;
    case LEFT:
      deltaPosition -= Right;
      break;
    case RIGHT:
      deltaPosition += Right;
      break;
    case JUMP:
      jumping = groundedMovement;
      break;
  }
}

void Camera::ProcessLeftAnalog(int16 stickX, int16 stickY, GLboolean constrainPitch)
{
  if (stickY > 0) ProcessInput(FORWARD);
  else if (stickY < 0) ProcessInput(BACKWARD);
  if (stickX > 0) ProcessInput(RIGHT);
  else if (stickX < 0) ProcessInput(LEFT);
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float32 xoffset, float32 yoffset, GLboolean constrainPitch)
{
  xoffset *= MouseSensitivity;
  yoffset *= MouseSensitivity;

  Yaw += xoffset;
  Pitch += yoffset;

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrainPitch)
  {
    if (Pitch > 89.0f) Pitch = 89.0f;
    if (Pitch < -89.0f) Pitch = -89.0f;
  }

  // Update Front, Right and Up Vectors using the updated Eular angles
  updateCameraVectors();
}

void Camera::ProcessRightAnalog(int16 stickX, int16 stickY, GLboolean constrainPitch)
{
  Yaw += (float32)stickX * stickSensitivity;;
  Pitch += (float32)stickY * stickSensitivity;

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrainPitch)
  {
    if (Pitch > 89.0f) Pitch = 89.0f;
    if (Pitch < -89.0f) Pitch = -89.0f;
  }

  // Update Front, Right and Up Vectors using the updated Eular angles
  updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float32 yoffset)
{
  Zoom -= yoffset;
  if (Zoom < 1.0f) Zoom = 1.0f;
  if (Zoom > 45.0f) Zoom = 45.0f;
}

// Calculates the front vector from the Camera's (updated) Eular Angles
void Camera::updateCameraVectors()
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