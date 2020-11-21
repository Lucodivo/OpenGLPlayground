#include "FirstPersonScene.h"
#include "../common/Input.h"

#define INIT_CAMERA_SPEED 1.0

void FirstPersonScene::inputStatesUpdated()
{
  Scene::inputStatesUpdated();

  if(keyboardCameraMovementEnabled)
  {
    // Camera movement speed
    InputState leftShiftState = getInputState(KeyboardInput_Shift_Left);
    if(leftShiftState == INPUT_HOT_PRESS) {
      cameraMovementSpeed = 2.0;
    } else if(leftShiftState == INPUT_HOT_RELEASE) {
      cameraMovementSpeed = 1.0;
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
      camera.ProcessMouseMovement(mouseMovementDelta.x, mouseMovementDelta.y, true);
    }

    if(isActive(MouseInput_Scroll)) {
      camera.ProcessMouseScroll(getMouseScrollY());
    }
  }

  if(controllerCameraMovementEnabled)
  {
    if(hotPress(Controller1Input_A))
    {
      camera.ProcessInput(JUMP);
    }

    InputState bState = getInputState(Controller1Input_B);
    if(bState == INPUT_HOT_PRESS)
    {
      cameraMovementSpeed = INIT_CAMERA_SPEED * 2;
    } else if(bState == INPUT_HOT_RELEASE) {
      cameraMovementSpeed = INIT_CAMERA_SPEED;
    }

    if(isActive(Controller1Input_Analog_Left))
    {
      ControllerAnalogStick leftAnalogStick = getControllerAnalogStickLeft();
      camera.ProcessLeftAnalog(leftAnalogStick.x, leftAnalogStick.y);
    }

    if(isActive(Controller1Input_Analog_Right))
    {
      ControllerAnalogStick rightAnalogStick = getControllerAnalogStickRight();
      camera.ProcessRightAnalog(rightAnalogStick.x, rightAnalogStick.y);
    }

    if(isActive(Controller1Input_DPad_Up))
    {
      camera.ProcessInput(FORWARD);
    }

    if(isActive(Controller1Input_DPad_Down))
    {
      camera.ProcessInput(BACKWARD);
    }

    if(isActive(Controller1Input_DPad_Left))
    {
      camera.ProcessInput(LEFT);
    }

    if(isActive(Controller1Input_DPad_Right))
    {
      camera.ProcessInput(RIGHT);
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

void FirstPersonScene::enableDefaultControllerCameraMovement(bool enable)
{
  controllerCameraMovementEnabled = enable;
}