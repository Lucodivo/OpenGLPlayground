#include "GodModeScene.h"

#define CAMERA_SPEED (SPEED * 4)
#define CAMERA_SPEED_FAST (CAMERA_SPEED * 2)

GodModeScene::GodModeScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
  : FirstPersonScene(window, initScreenHeight, initScreenWidth)
{
  camera.groundedMovement = false;
  camera.MovementSpeed = CAMERA_SPEED;
}

// +++ CONTROLLER CONSUMER IMPLEMETNATION - START +++
void GodModeScene::button_B_pressed()
{
  camera.MovementSpeed = CAMERA_SPEED_FAST;
}

void GodModeScene::button_B_released()
{
  camera.MovementSpeed = CAMERA_SPEED;
}
// +++ CONTROLLER CONSUMER OVERRIDES - END +++

// +++ KEYBOARD CONSUMER OVERRIDES - START +++
void GodModeScene::key_LeftShift_pressed()
{
  camera.MovementSpeed = CAMERA_SPEED_FAST;
}

void GodModeScene::key_LeftShift_released()
{
  camera.MovementSpeed = CAMERA_SPEED;
}
// +++ KEYBOARD CONSUMER OVERRIDES - START +++