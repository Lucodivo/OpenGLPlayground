#pragma once

#define GLFW_INCLUDE_NONE // ensure GLFW doesn't load OpenGL headers

#include <GLFW/glfw3.h>

#include "LearnOpenGlPlatform.h"

class KeyboardConsumer
{
public:
  virtual void key_LeftShift_pressed() = 0;
  virtual void key_LeftShift_released() = 0;
  virtual void key_W() = 0;
  virtual void key_S() = 0;
  virtual void key_A() = 0;
  virtual void key_D() = 0;
  virtual void key_Space() = 0;
  virtual void key_LeftMouseButton_pressed() = 0;
  virtual void key_LeftMouseButton_released() = 0;
  virtual void key_Up() = 0;
  virtual void key_Down() = 0;
  virtual void key_Left() = 0;
  virtual void key_Right() = 0;
  virtual void key_AltEnter_pressed() = 0;
  virtual void key_AltEnter_released() = 0;
};

class MouseMovementConsumer
{
public:
  virtual void mouseMovement(float32 xOffset, float32 yOffset) = 0;
};

class MouseScrollConsumer
{
public:
  virtual void mouseScroll(float32 yOffset) = 0;
};

class ControllerConsumer
{
public:
  virtual void leftAnalog(int16 stickX, int16 stickY) = 0;
  virtual void rightAnalog(int16 stickX, int16 stickY) = 0;
  virtual void button_A_pressed() = 0;
  virtual void button_A_released() = 0;
  virtual void button_B_pressed() = 0;
  virtual void button_B_released() = 0;
  virtual void button_X_pressed() = 0;
  virtual void button_X_released() = 0;
  virtual void button_Y_pressed() = 0;
  virtual void button_Y_released() = 0;
  virtual void button_dPadUp_pressed() = 0;
  virtual void button_dPadDown_pressed() = 0;
  virtual void button_dPadLeft_pressed() = 0;
  virtual void button_dPadRight_pressed() = 0;
  virtual void button_leftShoulder_pressed() = 0;
  virtual void button_rightShoulder_pressed() = 0;
  virtual void button_start_pressed() = 0;
  virtual void button_select_pressed() = 0;
  virtual void button_select_released() = 0;
};

class FrameBufferSizeConsumer
{
public:
  virtual void frameBufferSize(uint32 width, uint32 height) = 0;
};

void loadXInput();

void processKeyboardInput(GLFWwindow* window, KeyboardConsumer* consumer);
void processXInput(ControllerConsumer* consumer);
void subscribeMouseMovement(GLFWwindow* window, MouseMovementConsumer* consumer);
void subscribeMouseScroll(GLFWwindow* window, MouseScrollConsumer* consumer);
void subscribeFrameBufferSize(GLFWwindow* window, FrameBufferSizeConsumer* consumer);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);