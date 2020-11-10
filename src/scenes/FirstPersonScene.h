#pragma once

#include "Scene.h"
#include "../Camera.h"

class FirstPersonScene : public Scene, public ControllerConsumer
{
public:
  FirstPersonScene();

  // Scene implementation required
  virtual void init(uint32 windowWidth, uint32 windowHeight);
  virtual void deinit();
  virtual void drawFrame(){};
  virtual void inputStatesUpdated();

  void enableDefaultMouseCameraMovement(bool enable);
  void enableDefaultKeyboardCameraMovement(bool enable);

  void leftAnalog(int16 stickX, int16 stickY) {}
  void rightAnalog(int16 stickX, int16 stickY) {}
  void button_A_pressed() {}
  void button_A_released() {}
  void button_B_pressed() {}
  void button_B_released() {}
  void button_X_pressed() {}
  void button_X_released() {}
  void button_Y_pressed() {}
  void button_Y_released() {}
  void button_dPadUp_pressed() {}
  void button_dPadDown_pressed() {}
  void button_dPadLeft_pressed() {}
  void button_dPadRight_pressed() {}
  void button_leftShoulder_pressed() {}
  void button_rightShoulder_pressed() {}
  void button_start_pressed() {}
  void button_select_pressed() {}
  void button_select_released() {}
  void key_Tab_pressed() {}
  void key_Tab_released() {}

protected:
  Camera camera = Camera();
  bool mouseCameraMovementEnabled = true;
  bool keyboardCameraMovementEnabled = true;
};