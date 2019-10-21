#define GLFW_INCLUDE_NONE // ensure GLFW doesn't load OpenGL headers

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "main.h"
#include "scenes/NessCube/NessCubesScene.h"
#include "scenes/InfiniteCube/InfiniteCubeScene.h"
#include "scenes/ReflectRefract/ReflectRefractScene.h"
#include "scenes/AsteroidBelt/AsteroidBeltScene.h"
#include "scenes/Floor/FloorScene.h"
#include "scenes/Room/RoomScene.h"
#include "Input.h"

#include <iostream>

#define MULTI_SAMPLING_ON true

int main()
{
  loadGLFW();
  GLFWwindow* window = createWindow();
  initializeGLAD();
  loadXInput();

  Scene* scene = new FloorScene(window, VIEWPORT_INIT_HEIGHT, VIEWPORT_INIT_WIDTH);
  scene->runScene();

  glfwTerminate(); // clean up gl resources
  return 0;
}

void loadGLFW()
{
  int loadSucceeded = glfwInit();
  if (loadSucceeded == GLFW_FALSE)
  {
    std::cout << "Failed to load GLFW" << std::endl;
    exit(-1);
  }
}

void initializeGLAD()
{
  // intialize GLAD to help manage function pointers for OpenGL
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    exit(-1);
  }

#if MULTI_CAMPLING_ON
  glEnable(GL_MULTISAMPLE);
#endif
}

GLFWwindow* createWindow()
{
  // set what kind of window desired
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL version x._
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL version _.x
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

#if MULTI_CAMPLING_ON
  glfwWindowHint(GLFW_SAMPLES, 4);
#endif

  // create window
  GLFWwindow* window = glfwCreateWindow(VIEWPORT_INIT_WIDTH, // int Width
                                        VIEWPORT_INIT_HEIGHT, // int Height
                                        "LearnOpenGL", // const char* Title
                                        nullptr, // GLFWmonitor* Monitor: Specified for which monitor for fullscreen, NULL for windowed mode
                                        nullptr); // GLFWwindow* Share: window to share resources with
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(-1);
  }

  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  return window;
}