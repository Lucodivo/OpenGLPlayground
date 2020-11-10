#pragma once

#include "FirstPersonScene.h"
#include "../Camera.h"

class GodModeScene : public FirstPersonScene
{
public:
  GodModeScene();

  virtual void inputStatesUpdated();

  // ControllerConsumer implementation
  void button_B_pressed();
  void button_B_released();
};