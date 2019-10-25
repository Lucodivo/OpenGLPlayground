//
// Created by Connor on 10/24/2019.
//

#include "GUIScene.h"


GUIScene::GUIScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
      : FirstPersonScene(window, initScreenHeight, initScreenWidth)
{
  // TODO
}

void GUIScene::runScene()
{
  renderLoop();
}

void GUIScene::renderLoop()
{
  // background clear color
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

  // NOTE: render/game loop
  float32 startTime = (float32)glfwGetTime();
  while (glfwWindowShouldClose(window) == GL_FALSE)
  {
    // check for input
    processKeyboardInput(window, this);

    float32 t = (float32) glfwGetTime() - startTime;
    deltaTime = t - lastFrame;
    lastFrame = t;

    glfwSwapBuffers(window); // swaps double buffers
    glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
  }
}

void GUIScene::key_Tab_pressed()
{
  local_persist bool cursorEnabled = glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL;
  glfwSetInputMode(window, GLFW_CURSOR, cursorEnabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
  cursorEnabled = !cursorEnabled;
}