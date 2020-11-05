#include "FirstPersonScene.h"

FirstPersonScene::FirstPersonScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
        : Scene(window, initScreenHeight, initScreenWidth){}

void FirstPersonScene::init() {
  subscribeMouseMovement(this);
  subscribeKeyboardInput(this);
  subscribeXInput(this);
}

void FirstPersonScene::deinit() {
  unsubscribeMouseMovement(this);
  unsubscribeKeyboardInput(this);
  unsubscribeXInput(this);
}

// +++ CONTROLLER CONSUMER IMPLEMETNATION - START +++
void FirstPersonScene::leftAnalog(int16 stickX, int16 stickY)
{
  camera.ProcessLeftAnalog(stickX, stickY);
}

void FirstPersonScene::rightAnalog(int16 stickX, int16 stickY)
{
  camera.ProcessRightAnalog(stickX, stickY);
}

void FirstPersonScene::button_A_pressed() {
  camera.ProcessInput(JUMP);
}

void FirstPersonScene::button_B_pressed()
{
  camera.MovementSpeed = CAMERA_SPEED * 2;
}

void FirstPersonScene::button_B_released()
{
  camera.MovementSpeed = CAMERA_SPEED;
}

void FirstPersonScene::button_dPadUp_pressed()
{
  camera.ProcessInput(FORWARD);
}

void FirstPersonScene::button_dPadDown_pressed()
{
  camera.ProcessInput(BACKWARD);
}

void FirstPersonScene::button_dPadLeft_pressed()
{
  camera.ProcessInput(LEFT);
}

void FirstPersonScene::button_dPadRight_pressed()
{
  camera.ProcessInput(RIGHT);
}

void FirstPersonScene::button_start_pressed()
{
  // close window
}

void FirstPersonScene::button_select_pressed()
{
  adjustWindowSize();
}

// +++ KEYBOARD CONSUMER IMPLEMENTATION - START +++
void FirstPersonScene::key_LeftShift_pressed()
{
  camera.MovementSpeed = CAMERA_SPEED * 2;
}

void FirstPersonScene::key_LeftShift_released()
{
  camera.MovementSpeed = CAMERA_SPEED;
}

void FirstPersonScene::key_W()
{
  camera.ProcessInput(FORWARD);
}

void FirstPersonScene::key_S()
{
  camera.ProcessInput(BACKWARD);
}

void FirstPersonScene::key_A()
{
  camera.ProcessInput(LEFT);
}

void FirstPersonScene::key_D()
{
  camera.ProcessInput(RIGHT);
}

void FirstPersonScene::key_Space()
{
  camera.ProcessInput(JUMP);
}

void FirstPersonScene::key_AltEnter_pressed()
{
  adjustWindowSize();
}
// +++ INPUT CONSUMER IMPLEMENTATION - END +++

// +++ MOUSE MOVEMENT CONSUMER IMPLEMENTATION - START +++
void FirstPersonScene::mouseMovement(float32 xOffset, float32 yOffset)
{
  if(screenSizeChangedSkipNextTrashMouseMove) {
    screenSizeChangedSkipNextTrashMouseMove = false;
    return;
  }
  camera.ProcessMouseMovement(xOffset, yOffset);
}
// +++ MOUSE MOVEMENT IMPLEMENTATION - END +++

// +++ MOUSE SCROLL CONSUMER IMPLEMENTATION - START +++
void FirstPersonScene::mouseScroll(float32 yOffset)
{
  camera.ProcessMouseScroll(yOffset);
}
// +++ MOUSE SCROLL CONSUMER IMPLEMENTATION - END +++


void FirstPersonScene::frameBufferSize(uint32 width, uint32 height) {
  Scene::frameBufferSize(width, height);
  screenSizeChangedSkipNextTrashMouseMove = true;
}