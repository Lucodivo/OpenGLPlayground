#include "Input.h"

#include <windows.h>
#include <xinput.h>
#include <iostream>

MouseMovementConsumer* movementConsumer;
MouseScrollConsumer* scrollConsumer;
FrameBufferSizeConsumer* frameBufferConsumer;

float32 lastX = VIEWPORT_INIT_WIDTH / 2;
float32 lastY = VIEWPORT_INIT_HEIGHT / 2;


#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE *pState)

typedef X_INPUT_GET_STATE(x_input_get_state);

X_INPUT_GET_STATE(XInputGetStateStub)
{
  return (ERROR_DEVICE_NOT_CONNECTED);
}

global_variable x_input_get_state* XInputGetState_ = XInputGetStateStub;
#define XInputGetState XInputGetState_

void loadXInput()
{
  HMODULE XInputLibrary = LoadLibraryA("xinput1_4.dll");
  if (!XInputLibrary)
  {
    XInputLibrary = LoadLibraryA("xinput9_1_0.dll");
  }
  if (!XInputLibrary)
  {
    XInputLibrary = LoadLibraryA("xinput1_3.dll");
  }
  if (XInputLibrary)
  {
    XInputGetState = (x_input_get_state*)GetProcAddress(XInputLibrary, "XInputGetState");
    if (!XInputGetState)
    {
      XInputGetState = XInputGetStateStub;
    }
  } else
  {
    std::cout << "Failed to load XInput" << std::endl;
    exit(-1);
  }
}

void processXInput(ControllerConsumer* consumer)
{
  for (DWORD controllerIndex = 0; controllerIndex < XUSER_MAX_COUNT; ++controllerIndex)
  {
    XINPUT_STATE controllerState;
    if (XInputGetState(controllerIndex, &controllerState) == ERROR_SUCCESS)
    {
      // the controller is plugged in
      XINPUT_GAMEPAD* pad = &controllerState.Gamepad;

      bool dPadUp = (pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
      bool dPadDown = (pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
      bool dPadLeft = (pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
      bool dPadRight = (pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
      bool leftShoulder = (pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
      bool rightShoulder = (pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
      bool start = (pad->wButtons & XINPUT_GAMEPAD_START);
      bool select = (pad->wButtons & XINPUT_GAMEPAD_BACK);
      bool a = (pad->wButtons & XINPUT_GAMEPAD_A);
      bool b = (pad->wButtons & XINPUT_GAMEPAD_B);
      bool x = (pad->wButtons & XINPUT_GAMEPAD_X);
      bool y = (pad->wButtons & XINPUT_GAMEPAD_Y);

      if (dPadUp)
      {
        consumer->button_dPadUp_pressed();
      }

      if (dPadDown)
      {
        consumer->button_dPadDown_pressed();
      }

      if (dPadLeft)
      {
        consumer->button_dPadLeft_pressed();
      }

      if (dPadRight)
      {
        consumer->button_dPadRight_pressed();
      }

      if (leftShoulder)
      {
        consumer->button_leftShoulder_pressed();
      }

      if (rightShoulder)
      {
        consumer->button_rightShoulder_pressed();
      }

      if (start)
      {
        consumer->button_start_pressed();
      }


      local_persist bool selectWasDown = false;
      if (select)
      {
        if (!selectWasDown)
        {
          selectWasDown = true;
          consumer->button_select_pressed();
        }
      } else if (selectWasDown)
      {
        selectWasDown = false;
        consumer->button_select_released();
      }

      local_persist bool aWasDown = false;
      if (a)
      {
        if (!aWasDown)
        {
          aWasDown = true;
          consumer->button_A_pressed();
        }
      } else if (aWasDown)
      {
        aWasDown = false;
        consumer->button_A_released();
      }

      local_persist bool bWasDown = false;
      if (b)
      {
        if (!bWasDown)
        {
          bWasDown = true;
          consumer->button_B_pressed();
        }
      } else if (bWasDown)
      {
        bWasDown = false;
        consumer->button_B_released();
      }

      local_persist bool xWasDown = false;
      if (x)
      {
        if (!xWasDown)
        {
          xWasDown = true;
          consumer->button_X_pressed();
        }
      } else if (xWasDown)
      {
        xWasDown = false;
        consumer->button_X_released();
      }

      local_persist bool yWasDown = false;
      if (y)
      {
        if (!yWasDown)
        {
          yWasDown = true;
          consumer->button_Y_pressed();
        }
      } else if (yWasDown)
      {
        yWasDown = false;
        consumer->button_Y_released();
      }

      int16 leftStickX = pad->sThumbLX;
      int16 leftStickY = pad->sThumbLY;

      if (leftStickX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
          leftStickX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
      {
        leftStickX = 0;
      }
      if (leftStickY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
          leftStickY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
      {
        leftStickY = 0;
      }
      if (leftStickX + leftStickY != 0)
      {
        consumer->leftAnalog(leftStickX, leftStickY);
      }

      int16 rightStickX = pad->sThumbRX;
      int16 rightStickY = pad->sThumbRY;

      if (rightStickX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
          rightStickX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
      {
        rightStickX = 0;
      }
      if (rightStickY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
          rightStickY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
      {
        rightStickY = 0;
      }
      if ((rightStickX + rightStickY) != 0)
      {
        consumer->rightAnalog(rightStickX, rightStickY);
      }
    } else
    {
      // the controller is not available
    }
  }
}

void processKeyboardInput(GLFWwindow* window, KeyboardConsumer* consumer)
{
  double currentTime = glfwGetTime();

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }

  local_persist bool leftShiftWasDown = false;
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
  {
    if (!leftShiftWasDown)
    {
      leftShiftWasDown = true;
      consumer->key_LeftShift_pressed();
    }
  } else if (leftShiftWasDown)
  {
    leftShiftWasDown = false;
    consumer->key_LeftShift_released();
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    consumer->key_W();
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    consumer->key_S();
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    consumer->key_A();
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    consumer->key_D();
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    consumer->key_Space();

  local_persist bool leftMouseButtonWasDown = false;
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
  {
    if (!leftMouseButtonWasDown)
    {
      leftMouseButtonWasDown = true;
      consumer->key_LeftMouseButton_pressed();
    }
  } else if (leftMouseButtonWasDown)
  {
    leftMouseButtonWasDown = false;
    consumer->key_LeftMouseButton_released();
  }

  local_persist bool altEnterWasDown = false;
  if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS &&
      glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
  {
    if (!altEnterWasDown)
    {
      altEnterWasDown = true;
      consumer->key_AltEnter_pressed();
    }
  } else if (altEnterWasDown)
  {
    altEnterWasDown = false;
    consumer->key_AltEnter_released();
  }

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
  {
    consumer->key_Up();
  }

  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
  {
    consumer->key_Down();
  }

  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
  {
    consumer->key_Left();
  }

  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
  {
    consumer->key_Right();
  }
}

void subscribeMouseMovement(GLFWwindow* window, MouseMovementConsumer* consumer)
{
  movementConsumer = consumer;
  local_persist bool hasSetCallback = false;
  if (!hasSetCallback)
  {
    glfwSetCursorPosCallback(window, mouse_callback);
  }
}

void subscribeMouseScroll(GLFWwindow* window, MouseScrollConsumer* consumer)
{
  scrollConsumer = consumer;
  local_persist bool hasSetCallback = false;
  if (!hasSetCallback)
  {
    glfwSetScrollCallback(window, scroll_callback);
  }
}

void subscribeFrameBufferSize(GLFWwindow* window, FrameBufferSizeConsumer* consumer)
{
  frameBufferConsumer = consumer;
  local_persist bool hasSetCallback = false;
  if (!hasSetCallback)
  {
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  }
}

// Callback function for when user moves mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
  local_persist bool firstMouse = true;
  if (firstMouse)
  {
    lastX = (float32)xpos;
    lastY = (float32)ypos;
    firstMouse = false;
  }

  float32 xOffset = (float32)xpos - lastX;
  float32 yOffset = lastY - (float32)ypos; // reversed since y-coordinates go from bottom to top

  lastX = (float32)xpos;
  lastY = (float32)ypos;

  movementConsumer->mouseMovement(xOffset, yOffset);
}

// Callback function for when user scrolls with mouse wheel
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
  scrollConsumer->mouseScroll((float32)yOffset);
}

// Callback for when screen changes size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  frameBufferConsumer->frameBufferSize((uint32)width, (uint32)height);
}