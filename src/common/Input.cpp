#include "Input.h"

#include <Windows.h>
#include <Xinput.h>
#include <iostream>
#include <vector>

// NOTE: This file does not support multiple windows
#define MAX_NUMBER_OF_SUBSCRIBERS 10
file_access struct {
  uint32 keyboardCount = 0;
  KeyboardConsumer* keyboard[MAX_NUMBER_OF_SUBSCRIBERS]{};
  uint32 controllerCount = 0;
  ControllerConsumer* controller[MAX_NUMBER_OF_SUBSCRIBERS]{};
  uint32 mouseCount = 0;
  MouseConsumer* mouse[MAX_NUMBER_OF_SUBSCRIBERS]{};
  WindowSizeConsumer* windowSize = NULL;
} inputConsumers;

file_access bool skipMouseMovementFromWindowSizeChange = false;

// NOTE: Casey Muratori's efficient way of handling function pointers, Handmade Hero episode 6 @ 22:06 & 1:00:21
// NOTE: Allows us to quickly change the function parameters & return type in one place and cascade throughout the rest
// NOTE: of the code if need be.
#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE *pState) // succinct way to define function of this type in future
typedef X_INPUT_GET_STATE(x_input_get_state); // succinct way to define function pointer of type above in the future
X_INPUT_GET_STATE(XInputGetStateStub) // create stub function of type above
{
  return (ERROR_DEVICE_NOT_CONNECTED);
}
file_access x_input_get_state* XInputGetState_ = XInputGetStateStub; // Create a function pointer of type above to point to stub
#define XInputGetState XInputGetState_ // Allow us to use XInputGetState method name without conflicting with definition in Xinput.h

file_access void loadXInput()
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
    XInputGetState = (x_input_get_state*) GetProcAddress(XInputLibrary, "XInputGetState");
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

void initializeInput(GLFWwindow* window)
{
  loadXInput();
  glfwSetCursorPosCallback(window, mouse_movement_callback);
  glfwSetScrollCallback(window, mouse_scroll_callback);
  glfwSetFramebufferSizeCallback(window, window_size_callback);
}

void processInput(GLFWwindow* window) {
  processXInput();
  processKeyboardInput(window);
  processMouseInput(window);
}

void processXInput()
{
  if(inputConsumers.controllerCount == 0) return;
  ControllerConsumer** consumers = inputConsumers.controller;
  uint32 consumerCount = inputConsumers.controllerCount;

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
        for(uint32 i = 0; i < consumerCount; ++i) {
          consumers[i]->button_dPadUp_pressed();;
        }
      }

      if (dPadDown)
      {
        for(uint32 i = 0; i < consumerCount; ++i) {
          consumers[i]->button_dPadDown_pressed();;
        }
      }

      if (dPadLeft)
      {
        for(uint32 i = 0; i < consumerCount; ++i) {
          consumers[i]->button_dPadLeft_pressed();;
        }
      }

      if (dPadRight)
      {
        for(uint32 i = 0; i < consumerCount; ++i) {
          consumers[i]->button_dPadRight_pressed();;
        }
      }

      if (leftShoulder)
      {
        for(uint32 i = 0; i < consumerCount; ++i) {
          consumers[i]->button_leftShoulder_pressed();;
        }
      }

      if (rightShoulder)
      {
        for(uint32 i = 0; i < consumerCount; ++i) {
          consumers[i]->button_rightShoulder_pressed();;
        }
      }

      if (start)
      {
        for(uint32 i = 0; i < consumerCount; ++i) {
          consumers[i]->button_start_pressed();;
        }
      }


      local_access bool selectWasDown = false;
      if (select)
      {
        if (!selectWasDown)
        {
          selectWasDown = true;
          for(uint32 i = 0; i < consumerCount; ++i) {
            consumers[i]->button_select_pressed();;
          }
        }
      } else if (selectWasDown)
      {
        selectWasDown = false;
        for(uint32 i = 0; i < consumerCount; ++i) {
          consumers[i]->button_select_released();;
        }
      }

      local_access bool aWasDown = false;
      if (a)
      {
        if (!aWasDown)
        {
          aWasDown = true;
          for(uint32 i = 0; i < consumerCount; ++i) {
            consumers[i]->button_A_pressed();;
          }
        }
      } else if (aWasDown)
      {
        aWasDown = false;
        for(uint32 i = 0; i < consumerCount; ++i) {
          consumers[i]->button_A_released();;
        }
      }

      local_access bool bWasDown = false;
      if (b)
      {
        if (!bWasDown)
        {
          bWasDown = true;
          for(uint32 i = 0; i < consumerCount; ++i) {
            consumers[i]->button_B_pressed();;
          }
        }
      } else if (bWasDown)
      {
        bWasDown = false;
        for(uint32 i = 0; i < consumerCount; ++i) {
          consumers[i]->button_B_released();;
        }
      }

      local_access bool xWasDown = false;
      if (x)
      {
        if (!xWasDown)
        {
          xWasDown = true;
          for(uint32 i = 0; i < consumerCount; ++i) {
            consumers[i]->button_X_pressed();;
          }
        }
      } else if (xWasDown)
      {
        xWasDown = false;
        for(uint32 i = 0; i < consumerCount; ++i) {
          consumers[i]->button_X_released();;
        }
      }

      local_access bool yWasDown = false;
      if (y)
      {
        if (!yWasDown)
        {
          yWasDown = true;
          for(uint32 i = 0; i < consumerCount; ++i) {
            consumers[i]->button_Y_pressed();;
          }
        }
      } else if (yWasDown)
      {
        yWasDown = false;
        for(uint32 i = 0; i < consumerCount; ++i) {
          consumers[i]->button_Y_released();;
        }
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
      if (leftStickX != 0 && leftStickY != 0)
      {
        for(uint32 i = 0; i < consumerCount; ++i) {
          consumers[i]->leftAnalog(leftStickX, leftStickY);
        }
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
      if (rightStickX != 0 && rightStickY != 0)
      {
        for(uint32 i = 0; i < consumerCount; ++i) {
          consumers[i]->rightAnalog(rightStickX, rightStickY);
        }
      }
    } else
    {
      // the controller is not available
    }
  }
}

void processKeyboardInput(GLFWwindow* window)
{
  if(inputConsumers.keyboardCount == 0) return;
  KeyboardConsumer** consumers = inputConsumers.keyboard;
  uint32 consumerCount = inputConsumers.keyboardCount;

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    for(uint32 i = 0; i < consumerCount; ++i) {
      consumers[i]->key_Esc();
    }
  }

  local_access bool leftShiftWasDown = false;
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
  {
    if (!leftShiftWasDown)
    {
      leftShiftWasDown = true;
      for(uint32 i = 0; i < consumerCount; ++i) {
        consumers[i]->key_LeftShift_pressed();
      }
    }
  } else if (leftShiftWasDown)
  {
    leftShiftWasDown = false;
    for(uint32 i = 0; i < consumerCount; ++i) {
      consumers[i]->key_LeftShift_released();
    }
  }

  local_access bool EWasDown = false;
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
  {
    if (!EWasDown)
    {
      EWasDown = true;
      for(uint32 i = 0; i < consumerCount; ++i) {
        consumers[i]->key_E_pressed();
      }
    }
  } else if (EWasDown)
  {
    EWasDown = false;
    for(uint32 i = 0; i < consumerCount; ++i) {
      consumers[i]->key_E_released();
    }
  }

  local_access bool QWasDown = false;
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
  {
    if (!QWasDown)
    {
      QWasDown = true;
      for(uint32 i = 0; i < consumerCount; ++i) {
        consumers[i]->key_Q_pressed();
      }
    }
  } else if (QWasDown)
  {
    QWasDown = false;
    for(uint32 i = 0; i < consumerCount; ++i) {
      consumers[i]->key_Q_released();
    }
  }

  local_access bool OWasDown = false;
  if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
  {
    if (!OWasDown)
    {
      OWasDown = true;
      for(uint32 i = 0; i < consumerCount; ++i) {
        consumers[i]->key_O_pressed();
      }
    }
  } else if (OWasDown)
  {
    OWasDown = false;
    for(uint32 i = 0; i < consumerCount; ++i) {
      consumers[i]->key_O_released();
    }
  }

  local_access bool PWasDown = false;
  if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
  {
    if (!PWasDown)
    {
      PWasDown = true;
      for(uint32 i = 0; i < consumerCount; ++i) {
        consumers[i]->key_P_pressed();
      }
    }
  } else if (PWasDown)
  {
    PWasDown = false;
    for(uint32 i = 0; i < consumerCount; ++i) {
      consumers[i]->key_P_released();
    }
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    for(uint32 i = 0; i < consumerCount; ++i) {
      consumers[i]->key_W();
    }
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    for(uint32 i = 0; i < consumerCount; ++i) {
      consumers[i]->key_S();
    }
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    for(uint32 i = 0; i < consumerCount; ++i) {
      consumers[i]->key_A();
    }
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    for(uint32 i = 0; i < consumerCount; ++i) {
      consumers[i]->key_D();
    }
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
  {
    for(uint32 i = 0; i < consumerCount; ++i) {
      consumers[i]->key_Space();
    }
  }

  local_access bool altEnterWasDown = false;
  if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS &&
      glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
  {
    if (!altEnterWasDown)
    {
      altEnterWasDown = true;
      for(uint32 i = 0; i < consumerCount; ++i) {
        consumers[i]->key_AltEnter_pressed();
      }
    }
  } else if (altEnterWasDown)
  {
    altEnterWasDown = false;
    for(uint32 i = 0; i < consumerCount; ++i) {
      consumers[i]->key_AltEnter_released();
    }
  }

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
  {
    for(uint32 i = 0; i < consumerCount; ++i) {
      consumers[i]->key_Up();
    }
  }

  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
  {
    for(uint32 i = 0; i < consumerCount; ++i) {
      consumers[i]->key_Down();
    }
  }

  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
  {
    for(uint32 i = 0; i < consumerCount; ++i) {
      consumers[i]->key_Left();
    }
  }

  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
  {
    for(uint32 i = 0; i < consumerCount; ++i) {
      consumers[i]->key_Right();
    }
  }

  local_access bool tabWasDown = false;
  if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
  {
    if (!tabWasDown)
    {
      tabWasDown = true;
      for(uint32 i = 0; i < consumerCount; ++i) {
        consumers[i]->key_Tab_pressed();
      }
    }
  } else if (tabWasDown)
  {
    tabWasDown = false;
    for(uint32 i = 0; i < consumerCount; ++i) {
      consumers[i]->key_Tab_released();
    }
  }
}

void processMouseInput(GLFWwindow* window) {
  if(inputConsumers.mouseCount == 0) return;
  MouseConsumer** consumers = inputConsumers.mouse;
  uint32 consumerCount = inputConsumers.mouseCount;

  local_access bool leftMouseButtonWasDown = false;
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
  {
    if (!leftMouseButtonWasDown)
    {
      leftMouseButtonWasDown = true;
      float64 cursorPosX, cursorPosY;
      glfwGetCursorPos(window, &cursorPosX, &cursorPosY);
      for(uint32 i = 0; i < consumerCount; ++i) {
        consumers[i]->key_LeftMouseButton_pressed((float32)cursorPosX, (float32)cursorPosY);
      }
    }
  } else if (leftMouseButtonWasDown)
  {
    leftMouseButtonWasDown = false;
    float64 cursorPosX, cursorPosY;
    glfwGetCursorPos(window, &cursorPosX, &cursorPosY);
    for(uint32 i = 0; i < consumerCount; ++i) {
      consumers[i]->key_LeftMouseButton_released((float32)cursorPosX, (float32)cursorPosY);
    }
  }

  local_access bool rightMouseButtonWasDown = false;
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
  {
    if (!rightMouseButtonWasDown)
    {
      rightMouseButtonWasDown = true;
      float64 cursorPosX, cursorPosY;
      glfwGetCursorPos(window, &cursorPosX, &cursorPosY);
      for(uint32 i = 0; i < consumerCount; ++i) {
        consumers[i]->key_RightMouseButton_pressed((float32)cursorPosX, (float32)cursorPosY);
      }
    }
  } else if (rightMouseButtonWasDown)
  {
    rightMouseButtonWasDown = false;
    float64 cursorPosX, cursorPosY;
    glfwGetCursorPos(window, &cursorPosX, &cursorPosY);
    for(uint32 i = 0; i < consumerCount; ++i) {
      consumers[i]->key_RightMouseButton_released((float32)cursorPosX, (float32)cursorPosY);
    }
  }
}

void subscribeKeyboardInput(KeyboardConsumer* consumer)
{
  Assert(inputConsumers.keyboardCount < MAX_NUMBER_OF_SUBSCRIBERS);
  inputConsumers.keyboard[inputConsumers.keyboardCount++] = consumer;
}

void subscribeXInput(ControllerConsumer* consumer)
{
  Assert(inputConsumers.controllerCount < MAX_NUMBER_OF_SUBSCRIBERS);
  inputConsumers.controller[inputConsumers.controllerCount++] = consumer;
}

void subscribeMouseInput(MouseConsumer* consumer)
{
  Assert(inputConsumers.mouseCount < MAX_NUMBER_OF_SUBSCRIBERS);
  inputConsumers.mouse[inputConsumers.mouseCount++] = consumer;
}

void subscribeWindowSize(WindowSizeConsumer* consumer)
{
  inputConsumers.windowSize = consumer;
}

bool unsubscribeMouseInput(MouseConsumer* consumer)
{
  bool unsubscribed = false;
  for(uint32 i = 0; i < inputConsumers.mouseCount;)
  {
    if((inputConsumers.mouse[i]) == consumer) {
      inputConsumers.mouse[i] = inputConsumers.mouse[--inputConsumers.mouseCount];
      unsubscribed = true;
    } else { ++i; }
  }
  return unsubscribed;
}

bool unsubscribeKeyboardInput(KeyboardConsumer* consumer)
{
  bool unsubscribed = false;
  for(uint32 i = 0; i < inputConsumers.keyboardCount;)
  {
    if((inputConsumers.keyboard[i]) == consumer) {
      inputConsumers.keyboard[i] = inputConsumers.keyboard[--inputConsumers.keyboardCount];
      unsubscribed = true;
    } else { ++i; }
  }
  return unsubscribed;
}

bool unsubscribeXInput(ControllerConsumer* consumer)
{
  bool unsubscribed = false;
  for(uint32 i = 0; i < inputConsumers.controllerCount;)
  {
    if((inputConsumers.controller[i]) == consumer) {
      inputConsumers.controller[i] = inputConsumers.controller[--inputConsumers.controllerCount];
      unsubscribed = true;
    } else { ++i; }
  }
  return unsubscribed;
}

void unsubscribeWindowSize()
{
  inputConsumers.windowSize = NULL;
}

// Callback function for when user moves mouse
void mouse_movement_callback(GLFWwindow* window, float64 xPos, float64 yPos)
{
  local_access float32 lastXPos = (float32) xPos;
  local_access float32 lastYPos = (float32) yPos;
  local_access bool firstMouse = true;

  if(skipMouseMovementFromWindowSizeChange)
  {
    skipMouseMovementFromWindowSizeChange = false;
    return;
  }

  float32 xOffset = (float32) xPos - lastXPos;
  float32 yOffset = lastYPos - (float32) yPos; // reversed since y-coordinates go from bottom to top

  lastXPos = (float32) xPos;
  lastYPos = (float32) yPos;

  if(inputConsumers.mouseCount == 0) return;
  MouseConsumer** consumers = inputConsumers.mouse;
  const uint32 consumerCount = inputConsumers.mouseCount;
  for(uint32 i = 0; i < consumerCount; ++i) {
    consumers[i]->mouseMovement(xOffset, yOffset);
  }
}

// Callback function for when user scrolls with mouse wheel
void mouse_scroll_callback(GLFWwindow* window, float64 xOffset, float64 yOffset)
{
  if(inputConsumers.mouseCount == 0) return;
  MouseConsumer** consumers = inputConsumers.mouse;
  const uint32 consumerCount = inputConsumers.mouseCount;
  for(uint32 i = 0; i < consumerCount; ++i) {
    consumers[i]->mouseScroll((float32)yOffset);
  }
}

// NOTE: returns (0,0) when no longer on screen
void window_size_callback(GLFWwindow* window, int width, int height) {
  skipMouseMovementFromWindowSizeChange = true; // TODO: find better way to skip next mouse movement callback?

  if(inputConsumers.windowSize == NULL) { return; }

  inputConsumers.windowSize->windowSizeChanged((uint32)width, (uint32)height);
}