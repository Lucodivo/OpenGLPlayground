#include "FirstPersonScene.h"

FirstPersonScene::FirstPersonScene() : Scene(){}

void FirstPersonScene::init(uint32 windowWidth, uint32 windowHeight)
{
  Scene::init(windowWidth, windowHeight);
}

void FirstPersonScene::deinit() {
  Scene::deinit();
}

void FirstPersonScene::inputStatesUpdated()
{
  Scene::inputStatesUpdated();

  if(keyboardCameraMovementEnabled)
  {
    // Camera movement speed
    if(hotPress(KeyboardInput_Shift_Left)) {
      camera.MovementSpeed = CAMERA_SPEED * 2;
    } else if(hotRelease(KeyboardInput_Shift_Left)) {
      camera.MovementSpeed = CAMERA_SPEED;
    }

    // Camera movement direction
    if(isActive(KeyboardInput_W)){
      camera.ProcessInput(FORWARD);
    }
    if(isActive(KeyboardInput_A)){
      camera.ProcessInput(LEFT);
    }
    if(isActive(KeyboardInput_S)){
      camera.ProcessInput(BACKWARD);
    }
    if(isActive(KeyboardInput_D)){
      camera.ProcessInput(RIGHT);
    }

    // Camera jump
    if(isActive(KeyboardInput_Space)){
      camera.ProcessInput(JUMP);
    }
  }

  if(mouseCameraMovementEnabled)
  {
    // Camera orientation
    if(isActive(MouseInput_Movement)) {
      MouseCoord mouseMovementDelta = getMouseDelta();
      // TODO: This needs to be better documented. Since the origin of our screen is the upper left
      // TODO: moving the mouse down means that the vertical mouse delta is actually positive
      // TODO: potentially handle it in the camera class?
      camera.ProcessMouseMovement(mouseMovementDelta.x, -mouseMovementDelta.y);
    }

    if(isActive(MouseInput_Scroll)) {
      camera.ProcessMouseScroll(getMouseScrollY());
    }
  }
}

void FirstPersonScene::enableDefaultMouseCameraMovement(bool enable)
{
  mouseCameraMovementEnabled = enable;
}

void FirstPersonScene::enableDefaultKeyboardCameraMovement(bool enable)
{
  mouseCameraMovementEnabled = enable;
}

//// TODO: handle Xinput
//// +++ CONTROLLER CONSUMER IMPLEMETNATION - START +++
//void FirstPersonScene::leftAnalog(int16 stickX, int16 stickY)
//{
//  camera.ProcessLeftAnalog(stickX, stickY);
//}
//
//void FirstPersonScene::rightAnalog(int16 stickX, int16 stickY)
//{
//  camera.ProcessRightAnalog(stickX, stickY);
//}
//
//void FirstPersonScene::button_A_pressed() {
//  camera.ProcessInput(JUMP);
//}
//
//void FirstPersonScene::button_B_pressed()
//{
//  camera.MovementSpeed = CAMERA_SPEED * 2;
//}
//
//void FirstPersonScene::button_B_released()
//{
//  camera.MovementSpeed = CAMERA_SPEED;
//}
//
//void FirstPersonScene::button_dPadUp_pressed()
//{
//  camera.ProcessInput(FORWARD);
//}
//
//void FirstPersonScene::button_dPadDown_pressed()
//{
//  camera.ProcessInput(BACKWARD);
//}
//
//void FirstPersonScene::button_dPadLeft_pressed()
//{
//  camera.ProcessInput(LEFT);
//}
//
//void FirstPersonScene::button_dPadRight_pressed()
//{
//  camera.ProcessInput(RIGHT);
//}
//

