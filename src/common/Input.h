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

struct ControllerAnalogStick
{
  int16 x;
  int16 y;
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
  WindowInput_SizeChange,
  Controller1Input_A, Controller1Input_B, Controller1Input_X, Controller1Input_Y,
  Controller1Input_DPad_Up, Controller1Input_DPad_Down, Controller1Input_DPad_Left, Controller1Input_DPad_Right,
  Controller1Input_L1, Controller1Input_L2, Controller1Input_R1, Controller1Input_R2,
  Controller1Input_Start, Controller1Input_Select, Controller1Input_Analog_Left, Controller1Input_Analog_Right
};

enum InputState
{
  INPUT_HOT_PRESS = 1 << 0,
  INPUT_ACTIVE = 1 << 1,
  INPUT_HOT_RELEASE = 1 << 2,
  INPUT_INACTIVE = 1 << 3
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
ControllerAnalogStick getControllerAnalogStickLeft();
ControllerAnalogStick getControllerAnalogStickRight();

void enableCursor(GLFWwindow* window, bool enable);
bool isCursorEnabled(GLFWwindow* window);