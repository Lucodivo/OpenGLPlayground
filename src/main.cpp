#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "main.h"
#include "NessCubesScene.h"
#include "InfiniteCubeScene.h"
#include "ReflectRefractScene.h"
#include "AsteroidBeltScene.h"

#include <iostream>

int main()
{
  loadGLFW();
  GLFWwindow* window = createWindow();
  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  loadXInput();

  initializeGLAD();

  Scene* scene = new ReflectRefractScene(window, VIEWPORT_INIT_HEIGHT, VIEWPORT_INIT_WIDTH);
  scene->runScene();

  glfwTerminate(); // clean up gl resources
  return 0;
}

void loadGLFW()
{
  int loadSucceeded = glfwInit();
  if(loadSucceeded == GLFW_FALSE)
  {
    std::cout << "Failed to load GLFW" << std::endl;
    exit(-1);
  }
}

void initializeGLAD()
{
  // intialize GLAD to help manage function pointers for OpenGL
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    exit(-1);
  }
}

GLFWwindow* createWindow()
{
  // set what kind of window desired
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL version x._
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL version _.x
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create window
  GLFWwindow* window = glfwCreateWindow(VIEWPORT_INIT_WIDTH, // int Width
                                        VIEWPORT_INIT_HEIGHT, // int Height
                                        "LearnOpenGL", // const char* Title
                                        NULL, // GLFWmonitor* Monitor: Specified for which monitor for fullscreen, NULL for windowed mode
                                        NULL); // GLFWwindow* Share: window to share resources with
  if(window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(-1);
  }

  return window;
}