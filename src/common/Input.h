#pragma once

#define GLFW_INCLUDE_NONE // ensure GLFW doesn't load OpenGL headers

#include <GLFW/glfw3.h>
#include <map>
#include <glm/glm.hpp>

#include "../LearnOpenGLPlatform.h"

struct MouseCoord
{
  float64 x;
  float64 y;
};

struct Extent2D
{
  int32 x;
  int32 y;
};

enum InputType
{
  KeyboardInput_Q, KeyboardInput_W, KeyboardInput_E, KeyboardInput_R,
  KeyboardInput_A, KeyboardInput_S, KeyboardInput_D, KeyboardInput_F,
  KeyboardInput_J, KeyboardInput_K, KeyboardInput_L, KeyboardInput_Semicolon,
  KeyboardInput_Shift_Left, KeyboardInput_Ctrl_Left, KeyboardInput_Alt_Left, KeyboardInput_Tab,
  KeyboardInput_Shift_Right, KeyboardInput_Ctrl_Right, KeyboardInput_Alt_Right, KeyboardInput_Enter,
  KeyboardInput_Esc, KeyboardInput_Backtick, KeyboardInput_1, KeyboardInput_2, KeyboardInput_3,
  KeyboardInput_Up, KeyboardInput_Down, KeyboardInput_Left, KeyboardInput_Right, KeyboardInput_Space,
  MouseInput_Left, MouseInput_Right, MouseInput_Middle, MouseInput_Back, MouseInput_Forward,
  MouseInput_Scroll, MouseInput_Movement,
  WindowInput_SizeChange
};

enum InputState
{
  INPUT_HOT_PRESS = 1 << 0,
  INPUT_ACTIVE = 1 << 1,
  INPUT_HOT_RELEASE = 1 << 2,
  INPUT_INACTIVE = 1 << 3
};

class InputConsumer
{
public:
  virtual void inputStatesUpdated() = 0;
};

void initializeInput(GLFWwindow* window, Extent2D windowExtent);
void deinitializeInput(GLFWwindow* window);
void loadInputStateForFrame(GLFWwindow* window);
bool hotPress(InputType key); // returns true if input was just activated
bool hotRelease(InputType key); // returns true if input was just deactivated
bool isActive(InputType key); // returns true if key is pressed or held down

InputState getInputState(InputType key); // Note: for special use cases (ex: double click), use hotPress/hotRelease/isActive in most cases
MouseCoord getMousePosition();
MouseCoord getMouseDelta();
float32 getMouseScrollY();
Extent2D getWindowExtent();

void enableCursor(GLFWwindow* window, bool enable);
bool isCursorEnabled(GLFWwindow* window);








// TODO: reintroduce xinput controller logic to new input code

class ControllerConsumer
{
public:
  virtual void leftAnalog(int16 stickX, int16 stickY) {};
  virtual void rightAnalog(int16 stickX, int16 stickY) {};
  virtual void button_A_pressed() {};
  virtual void button_A_released() {};
  virtual void button_B_pressed() {};
  virtual void button_B_released() {};
  virtual void button_X_pressed() {};
  virtual void button_X_released() {};
  virtual void button_Y_pressed() {};
  virtual void button_Y_released() {};
  virtual void button_dPadUp_pressed() {};
  virtual void button_dPadDown_pressed() {};
  virtual void button_dPadLeft_pressed() {};
  virtual void button_dPadRight_pressed() {};
  virtual void button_leftShoulder_pressed() {};
  virtual void button_rightShoulder_pressed() {};
  virtual void button_start_pressed() {};
  virtual void button_select_pressed() {};
  virtual void button_select_released() {};
};

void initializeXInput();
void processXInput(ControllerConsumer** consumers, uint32 consumerCount);