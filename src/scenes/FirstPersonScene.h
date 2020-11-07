#pragma once

#include "Scene.h"
#include "../Camera.h"

class FirstPersonScene : public Scene, public ControllerConsumer, public MouseConsumer
{
public:
  FirstPersonScene(GLFWwindow* window);

  // Scene implementation required
  virtual void init(uint32 windowWidth, uint32 windowHeight);
  virtual void deinit();
  virtual void drawFrame(){};

  // ControllerConsumer implementation
  void leftAnalog(int16 stickX, int16 stickY);
  void rightAnalog(int16 stickX, int16 stickY);
  void button_A_pressed();
  void button_A_released() {}
  void button_B_pressed();
  void button_B_released();
  void button_X_pressed() {}
  void button_X_released() {}
  void button_Y_pressed() {}
  void button_Y_released() {}
  void button_dPadUp_pressed();
  void button_dPadDown_pressed();
  void button_dPadLeft_pressed();
  void button_dPadRight_pressed();
  void button_leftShoulder_pressed() {}
  void button_rightShoulder_pressed() {}
  void button_start_pressed() {};
  void button_select_pressed() {};
  void button_select_released() {}
  void key_Tab_pressed() {}
  void key_Tab_released() {}

  // KeyboardConsumer implementation
  void key_LeftShift_pressed();
  void key_LeftShift_released();
  void key_W();
  void key_S();
  void key_A();
  void key_D();
  void key_Space();
  void key_Up() {}
  void key_Down() {}
  void key_Left() {}
  void key_Right() {}
  void key_AltEnter_pressed() {};
  void key_AltEnter_released() {}

  // MouseConsumer implementation
  void mouseMovement(float32 xOffset, float32 yOffset);
  void mouseScroll(float32 yOffset);
  void key_LeftMouseButton_pressed(float32 xPos, float32 yPos) {}
  void key_LeftMouseButton_released(float32 xPos, float32 yPos) {}
  void key_RightMouseButton_pressed(float32 xPos, float32 yPos) {}
  void key_RightMouseButton_released(float32 xPos, float32 yPos) {}

  void framebufferSizeChange(uint32 width, uint32 height);

private:
  bool screenSizeChangedSkipNextTrashMouseMove;

protected:
  Camera camera = Camera();
};