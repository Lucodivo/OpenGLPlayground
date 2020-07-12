#define GLFW_INCLUDE_NONE // ensure GLFW doesn't load OpenGL headers

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "main.h"
#include "scenes/NessCube/NessCubesScene.h"
#include "scenes/InfiniteCube/InfiniteCubeScene.h"
#include "scenes/ReflectRefract/ReflectRefractScene.h"
#include "scenes/AsteroidBelt/AsteroidBeltScene.h"
#include "scenes/Moon/MoonScene.h"
#include "scenes/Room/RoomScene.h"
#include "common/Input.h"
#include "scenes/GUIScene/GUIScene.h"
#include "scenes/InfiniteCapsules/InfiniteCapsulesScene.h"
#include "scenes/Mandelbrot/MandelbrotScene.h"
#include "scenes/MengerSponge/MengerSpongeScene.h"
#include "scenes/RayTracingSphere/RayTracingSphereScene.h"
#include "scenes/Pixel2D/Pixel2DScene.h"

#define MULTI_SAMPLING_ON true

void initImgui(GLFWwindow* window);

int main()
{
  loadGLFW();
  GLFWwindow* window = createWindow();
  initializeGLAD();
  loadXInput();
  initImgui(window);

  Scene* scene = new MengerSpongeScene(window, VIEWPORT_INIT_HEIGHT, VIEWPORT_INIT_WIDTH);
  scene->runScene();

  glfwTerminate(); // clean up gl resources
  return 0;
}

void initImgui(GLFWwindow* window)
{
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(NULL);
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