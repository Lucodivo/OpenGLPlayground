#pragma once

#include "FirstPersonScene.h"
#include "../Camera.h"

class GodModeScene : public FirstPersonScene
{
public:
  GodModeScene(GLFWwindow* window);

  // ControllerConsumer implementation
  void button_B_pressed();
  void button_B_released();

  // KeyboardConsumer overrides
  void key_LeftShift_pressed();
  void key_LeftShift_released();
};