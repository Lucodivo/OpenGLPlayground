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
  MouseInput_Left, MouseInput_Right, MouseInput_Middle,
  MouseInput_Last, //TODO: MouseInput_Next ?
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

class InputConsumer_
{
public:
  class_access void initializeInput(GLFWwindow* window, Extent2D windowExtent);
  // TODO: deinit input?

  class_access void loadInputStateForFrame(GLFWwindow* window);
  virtual void inputStatesUpdated() = 0;

protected:

  class_access bool hotPress(InputType key); // returns true if key was just pressed
  class_access bool hotRelease(InputType key); // returns true if key was just released
  class_access bool isActive(InputType key); // returns true if key is pressed or held down

  class_access InputState getInputState(InputType key);
  class_access MouseCoord getMousePosition();
  class_access MouseCoord getMouseDelta();
  class_access float32 getMouseScrollY();
  class_access Extent2D getWindowExtent();

private:
  class_access void setKeyState(GLFWwindow* window, uint32 glfwKey, InputType keyboardInput);
  class_access void setMouseState(GLFWwindow* window, uint32 glfwKey, InputType mouseInput);

  class_access std::map<InputType, InputState> inputState;
  class_access MouseCoord mousePosition;
  class_access MouseCoord mouseDelta;
  class_access float32 mouseScrollY;
};

void mouse_scroll_callback(GLFWwindow* window, float64 xOffset, float64 yOffset);
void window_size_callback(GLFWwindow* window, int32 width, int32 height);






// TODO: remove below


class KeyboardConsumer
{
public:
  virtual void key_LeftShift_pressed() {};
  virtual void key_LeftShift_released() {};
  virtual void key_Esc(){};
  virtual void key_W() {};
  virtual void key_S() {};
  virtual void key_A() {};
  virtual void key_D() {};
  virtual void key_Q_pressed() {};
  virtual void key_Q_released() {};
  virtual void key_E_pressed() {};
  virtual void key_E_released() {};
  virtual void key_O_pressed() {};
  virtual void key_O_released() {};
  virtual void key_P_pressed() {};
  virtual void key_P_released() {};
  virtual void key_Space() {};
  virtual void key_Up() {};
  virtual void key_Down() {};
  virtual void key_Left() {};
  virtual void key_Right() {};
  virtual void key_AltEnter_pressed() {};
  virtual void key_AltEnter_released() {};
  virtual void key_Tab_pressed() {};
  virtual void key_Tab_released() {};
};

class MouseConsumer
{
public:
  virtual void mouseMovement(float32 xOffset, float32 yOffset) {};
  virtual void mouseScroll(float32 yOffset) {};
  virtual void key_LeftMouseButton_pressed(float32 xPos, float32 yPos) {};
  virtual void key_LeftMouseButton_released(float32 xPos, float32 yPos) {};
  virtual void key_RightMouseButton_pressed(float32 xPos, float32 yPos) {};
  virtual void key_RightMouseButton_released(float32 xPos, float32 yPos) {};
};

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

class WindowSizeConsumer
{
public:
  virtual void windowSizeChanged(uint32 width, uint32 height) {};
};

void initializeInput(GLFWwindow* window);
void processInput(GLFWwindow* window);
void processXInput();
void subscribeXInput(ControllerConsumer* consumer);
bool unsubscribeXInput(ControllerConsumer* consumer);