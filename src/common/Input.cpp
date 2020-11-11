#include "Input.h"

#include <Windows.h>
#include <Xinput.h>
#include <iostream>
#include <vector>

file_access void setKeyState(GLFWwindow* window, uint32 glfwKey, InputType keyboardInput);
file_access void setMouseState(GLFWwindow* window, uint32 glfwKey, InputType mouseInput);

void mouse_scroll_callback(GLFWwindow* window, float64 xOffset, float64 yOffset);
void window_size_callback(GLFWwindow* window, int32 width, int32 height);

file_access bool globalWindowSizeChange;
file_access bool cursorModeChange;
file_access Extent2D globalWindowExtent;
file_access MouseCoord globalMouseScroll;
file_access MouseCoord mousePosition = {0.0f, 0.0f };
file_access MouseCoord mouseDelta = {0.0f, 0.0f };
file_access float32 mouseScrollY = 0.0f;
file_access std::map<InputType, InputState>* inputState = NULL;

void initializeInput(GLFWwindow* window, Extent2D windowExtent)
{
  globalMouseScroll = MouseCoord{0.0f, 0.0f};
  globalWindowSizeChange = true;
  cursorModeChange = false;
  globalWindowExtent = windowExtent;
  inputState = new std::map<InputType, InputState>();
  glfwSetScrollCallback(window, mouse_scroll_callback);
  glfwSetFramebufferSizeCallback(window, window_size_callback);
}

void deinitializeInput(GLFWwindow* window)
{
  delete inputState;
  glfwSetScrollCallback(window, NULL);
  glfwSetFramebufferSizeCallback(window, NULL);
}

InputState getInputState(InputType key) {
  std::map<InputType, InputState>::iterator inputSearch = inputState->find(key);
  return inputSearch != inputState->end() ? inputSearch->second : INPUT_INACTIVE;
}

bool hotPress(InputType key) {
  return getInputState(key) & INPUT_HOT_PRESS;
}

bool hotRelease(InputType key) {
  return getInputState(key) & INPUT_HOT_RELEASE;
}

bool isActive(InputType key) {
  return getInputState(key) & (INPUT_HOT_PRESS | INPUT_ACTIVE);
}

MouseCoord getMousePosition() {
  return mousePosition;
}

MouseCoord getMouseDelta() {
  return mouseDelta;
}

float32 getMouseScrollY() {
  return mouseScrollY;
}

Extent2D getWindowExtent() {
  return globalWindowExtent;
}

void setKeyState(GLFWwindow* window, uint32 glfwKey, InputType keyboardInput)
{
  std::map<InputType, InputState>::iterator keyIterator = inputState->find(keyboardInput);
  InputState oldKeyState = keyIterator != inputState->end() ? keyIterator->second : INPUT_INACTIVE;
  if (glfwGetKey(window, glfwKey) == GLFW_PRESS)
  {
    if(oldKeyState & INPUT_HOT_PRESS) {
      (*inputState)[keyboardInput] = INPUT_ACTIVE;
    } else if(oldKeyState ^ INPUT_ACTIVE) {
      (*inputState)[keyboardInput] = INPUT_HOT_PRESS;
    }
  } else if(oldKeyState & (INPUT_HOT_PRESS | INPUT_ACTIVE)) {
    (*inputState)[keyboardInput] = INPUT_HOT_RELEASE;
  } else if(oldKeyState & INPUT_HOT_RELEASE) { // only erase if there is something to be erased
    inputState->erase(keyIterator);
  }
}

void setMouseState(GLFWwindow* window, uint32 glfwKey, InputType mouseInput)
{
  std::map<InputType, InputState>::iterator mouseInputIterator = inputState->find(mouseInput);
  InputState oldMouseInputState = mouseInputIterator != inputState->end() ? mouseInputIterator->second : INPUT_INACTIVE;
  if (glfwGetMouseButton(window, glfwKey) == GLFW_PRESS)
  {
    if(oldMouseInputState & INPUT_HOT_PRESS) {
      (*inputState)[mouseInput] = INPUT_ACTIVE;
    } else if(oldMouseInputState ^ INPUT_ACTIVE) {
      (*inputState)[mouseInput] = INPUT_HOT_PRESS;
    }
  } else if(oldMouseInputState & (INPUT_HOT_PRESS | INPUT_ACTIVE)) {
    (*inputState)[mouseInput] = INPUT_HOT_RELEASE;
  } else if(oldMouseInputState & INPUT_HOT_RELEASE) { // only erase if there is something to be erased
    inputState->erase(mouseInputIterator);
  }
}

void loadInputStateForFrame(GLFWwindow* window) {
  // keyboard state
  {
    setKeyState(window, GLFW_KEY_Q, KeyboardInput_Q);
    setKeyState(window, GLFW_KEY_W, KeyboardInput_W);
    setKeyState(window, GLFW_KEY_E, KeyboardInput_E);
    setKeyState(window, GLFW_KEY_R, KeyboardInput_R);
    setKeyState(window, GLFW_KEY_A, KeyboardInput_A);
    setKeyState(window, GLFW_KEY_S, KeyboardInput_S);
    setKeyState(window, GLFW_KEY_D, KeyboardInput_D);
    setKeyState(window, GLFW_KEY_F, KeyboardInput_F);
    setKeyState(window, GLFW_KEY_J, KeyboardInput_J);
    setKeyState(window, GLFW_KEY_K, KeyboardInput_K);
    setKeyState(window, GLFW_KEY_L, KeyboardInput_L);
    setKeyState(window, GLFW_KEY_SEMICOLON, KeyboardInput_Semicolon);
    setKeyState(window, GLFW_KEY_LEFT_SHIFT, KeyboardInput_Shift_Left);
    setKeyState(window, GLFW_KEY_LEFT_CONTROL, KeyboardInput_Ctrl_Left);
    setKeyState(window, GLFW_KEY_LEFT_ALT, KeyboardInput_Alt_Left);
    setKeyState(window, GLFW_KEY_TAB, KeyboardInput_Tab);
    setKeyState(window, GLFW_KEY_RIGHT_SHIFT, KeyboardInput_Shift_Right);
    setKeyState(window, GLFW_KEY_RIGHT_CONTROL, KeyboardInput_Ctrl_Right);
    setKeyState(window, GLFW_KEY_RIGHT_ALT, KeyboardInput_Alt_Right);
    setKeyState(window, GLFW_KEY_ENTER, KeyboardInput_Enter);
    setKeyState(window, GLFW_KEY_ESCAPE, KeyboardInput_Esc);
    setKeyState(window, GLFW_KEY_GRAVE_ACCENT, KeyboardInput_Backtick);
    setKeyState(window, GLFW_KEY_1, KeyboardInput_1);
    setKeyState(window, GLFW_KEY_2, KeyboardInput_2);
    setKeyState(window, GLFW_KEY_3, KeyboardInput_3);
    setKeyState(window, GLFW_KEY_UP, KeyboardInput_Up);
    setKeyState(window, GLFW_KEY_DOWN, KeyboardInput_Down);
    setKeyState(window, GLFW_KEY_LEFT, KeyboardInput_Left);
    setKeyState(window, GLFW_KEY_RIGHT, KeyboardInput_Right);
    setKeyState(window, GLFW_KEY_SPACE, KeyboardInput_Space);
  }

  // mouse state
  {
    setMouseState(window, GLFW_MOUSE_BUTTON_LEFT, MouseInput_Left);
    setMouseState(window, GLFW_MOUSE_BUTTON_RIGHT, MouseInput_Right);
    setMouseState(window, GLFW_MOUSE_BUTTON_MIDDLE, MouseInput_Middle);
    setMouseState(window, GLFW_MOUSE_BUTTON_4, MouseInput_Back);
    setMouseState(window, GLFW_MOUSE_BUTTON_5, MouseInput_Forward);

    // mouse movement state management
    {
      MouseCoord newMouseCoord;

      glfwGetCursorPos(window, &newMouseCoord.x, &newMouseCoord.y);

      // NOTE: We do not consume mouse input on window size changes as it results in unwanted values
      mouseDelta = (globalWindowSizeChange || cursorModeChange) ? MouseCoord{0.0f, 0.0f} : MouseCoord{newMouseCoord.x - mousePosition.x, newMouseCoord.y - mousePosition.y};
      cursorModeChange = false; // The above essentially consumers the
      mousePosition = newMouseCoord;

      std::map<InputType, InputState>::iterator movementIterator = inputState->find(MouseInput_Movement);
      bool movementWasActive = movementIterator != inputState->end();
      if (mouseDelta.x != 0.0f || mouseDelta.y != 0.0f)
      {
        if(!movementWasActive) {
          (*inputState)[MouseInput_Movement] = INPUT_ACTIVE;
        }
      } else if (movementWasActive) // scroll no longer active
      {
        inputState->erase(movementIterator);
      }
    }

    // mouse scroll state management
    {
      std::map<InputType, InputState>::iterator scrollIterator = inputState->find(MouseInput_Scroll);
      bool scrollWasActive = scrollIterator != inputState->end();
      mouseScrollY = (float32)globalMouseScroll.y;
      globalMouseScroll.y = 0.0f; // NOTE: Set to 0.0f to signify that the result has been consumed
      if (mouseScrollY != 0.0f && !scrollWasActive)
      {
        (*inputState)[MouseInput_Scroll] = INPUT_ACTIVE;
      } else if (scrollWasActive) // scroll no longer active
      {
        inputState->erase(scrollIterator);
      }
    }

    // window size change management
    {
      local_access bool windowInputSizeChangeReportedLastFrame = false;
      if(globalWindowSizeChange)
      {
        (*inputState)[WindowInput_SizeChange] = INPUT_ACTIVE;
        globalWindowSizeChange = false; // NOTE: Set to false to signify that the result has been consumed
        windowInputSizeChangeReportedLastFrame = true;
      } else if(windowInputSizeChangeReportedLastFrame)
      {
        inputState->erase(WindowInput_SizeChange);
        windowInputSizeChangeReportedLastFrame = false;
      }
    }
  }
}

// Callback function for when user scrolls with mouse wheel
void mouse_scroll_callback(GLFWwindow* window, float64 xOffset, float64 yOffset)
{
  globalMouseScroll.y = yOffset;
}

// NOTE: returns (0,0) when no longer on screen
void window_size_callback(GLFWwindow* window, int32 width, int32 height)
{
  globalWindowSizeChange = true;
  globalWindowExtent = { width, height };
}

void enableCursor(GLFWwindow* window, bool enable)
{
  glfwSetInputMode(window, GLFW_CURSOR, enable ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
  cursorModeChange = true;
}

bool isCursorEnabled(GLFWwindow* window)
{
  return glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL;
}






// TODO: reintroduce xinput controller logic to new input code

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

void initializeXInput()
{
  loadXInput();
}

void processXInput(ControllerConsumer** consumers, uint32 consumerCount)
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