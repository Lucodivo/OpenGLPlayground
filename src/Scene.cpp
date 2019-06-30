#include "Scene.h"

Scene::Scene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
  : window(window), viewportHeight(initScreenHeight), viewportWidth(initScreenWidth)
{
  subscribeFrameBufferSize(window, this);
}

// Callback function for when user resizes our window
void Scene::frameBufferSize(uint32 width, uint32 height)
{
  glViewport(0, 0, width, height);
  viewportHeight = height;
  viewportWidth = width;
}

void Scene::adjustWindowSize()
{
  local_persist bool windowMode = true;
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  if(windowMode)
  {
    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
  } else
  {
    glfwSetWindowMonitor(window, NULL, (mode->width / 4), (mode->height / 4), VIEWPORT_INIT_WIDTH, VIEWPORT_INIT_HEIGHT, GLFW_DONT_CARE);
  }
  windowMode = !windowMode;
}

void Scene::closeWindow()
{
  glfwSetWindowShouldClose(window, GL_TRUE);
}

FirstPersonScene::FirstPersonScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
  : Scene(window, initScreenHeight, initScreenWidth)
{
  subscribeMouseMovement(window, this);
  subscribeMouseScroll(window, this);
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

void FirstPersonScene::button_A_pressed() { camera.ProcessKeyboard(JUMP); }

void FirstPersonScene::button_A_released() {}

void FirstPersonScene::button_B_pressed()
{
  camera.MovementSpeed = SPEED * 2;
}

void FirstPersonScene::button_B_released()
{
  camera.MovementSpeed = SPEED;
}

void FirstPersonScene::button_X_pressed() {}

void FirstPersonScene::button_X_released() {}

void FirstPersonScene::button_Y_pressed() {}

void FirstPersonScene::button_Y_released() {}

void FirstPersonScene::button_dPadUp_pressed()
{
  camera.ProcessKeyboard(FORWARD);
}

void FirstPersonScene::button_dPadDown_pressed()
{
  camera.ProcessKeyboard(BACKWARD);
}

void FirstPersonScene::button_dPadLeft_pressed()
{
  camera.ProcessKeyboard(LEFT);
}

void FirstPersonScene::button_dPadRight_pressed()
{
  camera.ProcessKeyboard(RIGHT);
}

void FirstPersonScene::button_leftShoulder_pressed() {}

void FirstPersonScene::button_rightShoulder_pressed() {}

void FirstPersonScene::button_start_pressed()
{
  closeWindow();
}

void FirstPersonScene::button_select_pressed()
{
  adjustWindowSize();
}

void FirstPersonScene::button_select_released() {}
// +++ CONTROLLER CONSUMER IMPLEMETNATION - START +++

// +++ KEYBOARD CONSUMER IMPLEMENTATION - START +++
void FirstPersonScene::key_LeftShift_pressed()
{
  camera.MovementSpeed = SPEED * 2;
}

void FirstPersonScene::key_LeftShift_released()
{
  camera.MovementSpeed = SPEED;
}

void FirstPersonScene::key_W()
{
  camera.ProcessKeyboard(FORWARD);
}

void FirstPersonScene::key_S()
{
  camera.ProcessKeyboard(BACKWARD);
}

void FirstPersonScene::key_A()
{
  camera.ProcessKeyboard(LEFT);
}

void FirstPersonScene::key_D()
{
  camera.ProcessKeyboard(RIGHT);
}

void FirstPersonScene::key_Space()
{
  camera.ProcessKeyboard(JUMP);
}

void FirstPersonScene::key_LeftMouseButton_pressed()
{
  // Do nothing
}

void FirstPersonScene::key_LeftMouseButton_released()
{
  // Do nothing
}

void FirstPersonScene::key_Up()
{
  // Do nothing
}

void FirstPersonScene::key_Down()
{
  // Do nothing
}

void FirstPersonScene::key_Left()
{
  // Do nothing
}

void FirstPersonScene::key_Right()
{
  // Do nothing
}

void FirstPersonScene::key_AltEnter_pressed()
{
  adjustWindowSize();
}

void FirstPersonScene::key_AltEnter_released()
{
  // Do nothing
}
// +++ INPUT CONSUMER IMPLEMENTATION - END +++

// +++ MOUSE MOVEMENT CONSUMER IMPLEMENTATION - START +++
void FirstPersonScene::mouseMovement(float32 xOffset, float32 yOffset)
{
  camera.ProcessMouseMovement(xOffset, yOffset);
}
// +++ MOUSE MOVEMENT IMPLEMENTATION - END +++

// +++ MOUSE SCROLL CONSUMER IMPLEMENTATION - START +++
void FirstPersonScene::mouseScroll(float32 yOffset)
{
  camera.ProcessMouseScroll(yOffset);
}
// +++ MOUSE SCROLL CONSUMER IMPLEMENTATION - END +++