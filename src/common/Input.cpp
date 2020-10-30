#include "Input.h"

#include <Windows.h>
#include <Xinput.h>
#include <iostream>

template<typename T>
struct Node
{
  T* data;
  Node<T>* next;
};

template<typename T>
void addNode(Node<T>** headptr, T* newData)
{
  // add to front of linked list
  Node<T>* newNode = new Node<T>{newData, *headptr};
  *headptr = newNode;
}

// removes all nodes with specified data
template<typename T>
bool removeNode(Node<T>** headptr, T* removeData)
{
  bool nodeRemoved = false;
  if (*headptr != NULL)
  {
    if (*headptr->data == removeData)
    { // head node special case
      Node<T>* removeHeadNode = *headptr;
      Node<T>* removeTailNode = removeHeadNode;
      while (removeTailNode->next != NULL && removeTailNode->next->data == removeData)
      {
        removeTailNode = removeTailNode->next; // move it to the last instance of a series of removeData nodes
      }

      *headptr = removeTailNode->next; // disconnect link

      while (removeHeadNode != removeTailNode->next)
      {
        delete removeHeadNode; // deallocate
        removeHeadNode = removeHeadNode->next;
      }

      nodeRemoved = true;
    }

    if (*headptr != NULL)
    {
      Node<T>* traversalNode = *headptr;
      while (traversalNode->next != NULL)
      {
        if (traversalNode->next->data == removeData)
        {
          Node<T>* removeNode = traversalNode->next;
          traversalNode->next = removeNode->next; // disconnect link
          delete removeNode; // deallocate
          nodeRemoved = true;
        } else
        {
          traversalNode = traversalNode->next;
        }
      }
    }
  }
  return nodeRemoved;
}

// NOTE: This file does not support multiple windows
struct GLFWwindowUser
{
  Node<MouseMovementConsumer>* mouseMovementConsumer;
  Node<MouseScrollConsumer>* mouseScrollConsumer;
  Node<FrameBufferSizeConsumer>* framebufferSizeConsumer;
};

// NOTE: Casey Muratori's efficient way of handling function pointers, Handmade Hero episode 6 @ 22:06 & 1:00:21
// NOTE: Allows us to quickly change the function parameters & return type in one place and cascade throughout the rest
// NOTE: of the code if need be.
#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE *pState) // succinct way to define function of this type in future
typedef X_INPUT_GET_STATE(x_input_get_state); // succinct way to define function pointer of type above in the future
X_INPUT_GET_STATE(XInputGetStateStub) // create stub function of type above
{
  return (ERROR_DEVICE_NOT_CONNECTED);
}
file_accessible x_input_get_state* XInputGetState_ = XInputGetStateStub; // Create a function pointer of type above to point to stub
#define XInputGetState XInputGetState_ // Allow us to use XInputGetState method name without conflicting with definition in Xinput.h

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

  local_persist bool EWasDown = false;
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
  {
    if (!EWasDown)
    {
      EWasDown = true;
      consumer->key_E_pressed();
    }
  } else if (EWasDown)
  {
    EWasDown = false;
    consumer->key_E_released();
  }

  local_persist bool QWasDown = false;
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
  {
    if (!QWasDown)
    {
      QWasDown = true;
      consumer->key_Q_pressed();
    }
  } else if (QWasDown)
  {
    QWasDown = false;
    consumer->key_Q_released();
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
      float64 cursorPosX, cursorPosY;
      glfwGetCursorPos(window, &cursorPosX, &cursorPosY);
      consumer->key_LeftMouseButton_pressed((float32)cursorPosX, (float32)cursorPosY);
    }
  } else if (leftMouseButtonWasDown)
  {
    leftMouseButtonWasDown = false;
    float64 cursorPosX, cursorPosY;
    glfwGetCursorPos(window, &cursorPosX, &cursorPosY);
    consumer->key_LeftMouseButton_released((float32)cursorPosX, (float32)cursorPosY);
  }

  local_persist bool rightMouseButtonWasDown = false;
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
  {
    if (!rightMouseButtonWasDown)
    {
      rightMouseButtonWasDown = true;
      float64 cursorPosX, cursorPosY;
      glfwGetCursorPos(window, &cursorPosX, &cursorPosY);
      consumer->key_RightMouseButton_pressed((float32)cursorPosX, (float32)cursorPosY);
    }
  } else if (rightMouseButtonWasDown)
  {
    rightMouseButtonWasDown = false;
    float64 cursorPosX, cursorPosY;
    glfwGetCursorPos(window, &cursorPosX, &cursorPosY);
    consumer->key_RightMouseButton_released((float32)cursorPosX, (float32)cursorPosY);
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

  local_persist bool tabWasDown = false;
  if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
  {
    if (!tabWasDown)
    {
      tabWasDown = true;
      consumer->key_Tab_pressed();
    }
  } else if (tabWasDown)
  {
    tabWasDown = false;
    consumer->key_Tab_released();
  }
}

void setupWindowUser(GLFWwindow* window)
{
  local_persist bool windowUserSet = false;
  local_persist GLFWwindowUser windowUser = {NULL, NULL, NULL};
  if (!windowUserSet)
  {
    glfwSetWindowUserPointer(window, &windowUser);
    windowUserSet = true;
  }
}

void subscribeMouseMovement(GLFWwindow* window, MouseMovementConsumer* consumer)
{
  setupWindowUser(window);
  GLFWwindowUser* windowUser = (GLFWwindowUser*) glfwGetWindowUserPointer(window);
  addNode(&windowUser->mouseMovementConsumer, consumer);
  glfwSetCursorPosCallback(window, mouse_callback);
}

void subscribeMouseScroll(GLFWwindow* window, MouseScrollConsumer* consumer)
{
  setupWindowUser(window);
  GLFWwindowUser* windowUser = (GLFWwindowUser*) glfwGetWindowUserPointer(window);
  addNode(&windowUser->mouseScrollConsumer, consumer);
  glfwSetScrollCallback(window, scroll_callback);
}

void subscribeFrameBufferSize(GLFWwindow* window, FrameBufferSizeConsumer* consumer)
{
  setupWindowUser(window);
  GLFWwindowUser* windowUser = (GLFWwindowUser*) glfwGetWindowUserPointer(window);
  addNode(&windowUser->framebufferSizeConsumer, consumer);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

// Callback function for when user moves mouse
void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
  local_persist float32 lastXPos = (float32) xPos;
  local_persist float32 lastYPos = (float32) yPos;
  local_persist bool firstMouse = true;

  float32 xOffset = (float32) xPos - lastXPos;
  float32 yOffset = lastYPos - (float32) yPos; // reversed since y-coordinates go from bottom to top

  lastXPos = (float32) xPos;
  lastYPos = (float32) yPos;

  GLFWwindowUser* windowUser = (GLFWwindowUser*) glfwGetWindowUserPointer(window);
  for (Node<MouseMovementConsumer>* movementConsumerNode = windowUser->mouseMovementConsumer;
       movementConsumerNode != NULL; movementConsumerNode = movementConsumerNode->next)
  {
    movementConsumerNode->data->mouseMovement(xOffset, yOffset);
  }
}

// Callback function for when user scrolls with mouse wheel
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
  GLFWwindowUser* windowUser = (GLFWwindowUser*) glfwGetWindowUserPointer(window);
  for (Node<MouseScrollConsumer>* mouseScrollConsumer = windowUser->mouseScrollConsumer;
       mouseScrollConsumer != NULL; mouseScrollConsumer = mouseScrollConsumer->next)
  {
    mouseScrollConsumer->data->mouseScroll((float32) yOffset);
  }
}

// Callback for when screen changes size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  GLFWwindowUser* windowUser = (GLFWwindowUser*) glfwGetWindowUserPointer(window);
  for (Node<FrameBufferSizeConsumer>* framebufferSizeConsumer = windowUser->framebufferSizeConsumer;
       framebufferSizeConsumer != NULL; framebufferSizeConsumer = framebufferSizeConsumer->next)
  {
    framebufferSizeConsumer->data->frameBufferSize((uint32) width, (uint32) height);
  }
}