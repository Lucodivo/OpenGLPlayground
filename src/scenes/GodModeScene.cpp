#include "GodModeScene.h"

#define GODMODE_CAMERA_SPEED (CAMERA_SPEED * 4)
#define GODMOD_CAMERA_SPEED_FAST (GODMODE_CAMERA_SPEED * 2)

// TODO: Replace GodModeScene by simply using a different camera for FirstPersonScene
GodModeScene::GodModeScene() : FirstPersonScene()
{
  camera.groundedMovement = false;
  camera.MovementSpeed = GODMODE_CAMERA_SPEED;
}

// +++ CONTROLLER CONSUMER IMPLEMETNATION - START +++
void GodModeScene::button_B_pressed()
{
  camera.MovementSpeed = GODMOD_CAMERA_SPEED_FAST;
}

void GodModeScene::button_B_released()
{
  camera.MovementSpeed = GODMODE_CAMERA_SPEED;
}
// +++ CONTROLLER CONSUMER OVERRIDES - END +++

// +++ KEYBOARD CONSUMER OVERRIDES - START +++
void GodModeScene::key_LeftShift_pressed()
{
  camera.MovementSpeed = GODMOD_CAMERA_SPEED_FAST;
}

void GodModeScene::key_LeftShift_released()
{
  camera.MovementSpeed = GODMODE_CAMERA_SPEED;
}
// +++ KEYBOARD CONSUMER OVERRIDES - START +++