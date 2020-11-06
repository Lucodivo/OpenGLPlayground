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

#define VIEWPORT_INIT_WIDTH 1920
#define VIEWPORT_INIT_HEIGHT 1080

int main()
{
  loadGLFW();
  GLFWwindow* window = createWindow();
  initializeGLAD();
  initializeInput(window);
  initImgui(window);

  uint32 windowWidth = VIEWPORT_INIT_WIDTH;
  uint32 windowHeight = VIEWPORT_INIT_HEIGHT;

  uint32 sceneIndex = 0;
  NessCubesScene nessCubeScene = NessCubesScene(window);
  InfiniteCapsulesScene infiniteCapsulesScene = InfiniteCapsulesScene(window);
  Scene* scenes[] = { &nessCubeScene, &infiniteCapsulesScene };

  class InputConsumer_ : public KeyboardConsumer, public WindowSizeConsumer {
  public:
    InputConsumer_(GLFWwindow* window, Scene** scenes, uint32* sceneIndex, uint32 sceneCount, uint32* windowWidth, uint32* windowHeight){
      this->window = window;
      this->scenes = scenes;
      this->sceneIndex = sceneIndex;
      this->sceneCount = sceneCount;
      this->windowWidth = windowWidth;
      this->windowHeight = windowHeight;
    }
    GLFWwindow* window;
    Scene** scenes;
    uint32* sceneIndex;
    uint32 sceneCount;
    uint32* windowWidth;
    uint32* windowHeight;
    void key_O_pressed() {
      scenes[*sceneIndex]->deinit();
      if(++(*sceneIndex) == sceneCount) {
        *sceneIndex = 0;
      }
      scenes[*sceneIndex]->init(*windowWidth, *windowHeight);
    }
    void key_P_pressed() {
      scenes[*sceneIndex]->deinit();
      if(*sceneIndex == 0) {
        *sceneIndex = sceneCount -1;
      } else { --(*sceneIndex); }
      scenes[*sceneIndex]->init(*windowWidth, *windowHeight);
    }
    void key_Esc(){
      glfwSetWindowShouldClose(window, GL_TRUE);
    }
    void key_AltEnter_pressed() {
      toggleWindowSize(window, VIEWPORT_INIT_WIDTH, VIEWPORT_INIT_HEIGHT);
    }
    void windowSizeChanged(uint32 width, uint32 height) {
      *windowWidth = width;
      *windowHeight = height;
      scenes[*sceneIndex]->framebufferSizeChange(*windowWidth, *windowHeight);
    }
  } inputConsumer(window, scenes, &sceneIndex, ArrayCount(scenes), &windowWidth, &windowHeight);
  subscribeKeyboardInput(&inputConsumer);
  subscribeWindowSize(&inputConsumer);

  scenes[sceneIndex]->init(windowWidth, windowHeight);
  float32 deltaTime = 1.0;
  float32 lastFrame = (float32)glfwGetTime();
  while (glfwWindowShouldClose(window) == GL_FALSE)
  {
    processInput(window);

    scenes[sceneIndex]->drawFrame();

    uint32 numFrames = (uint32)(1 / deltaTime);
    scenes[sceneIndex]->renderText(std::to_string(numFrames) + " FPS", 25.0f, 25.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    float32 t = (float32)glfwGetTime();
    deltaTime = t - lastFrame;
    lastFrame = t;

    glfwSwapBuffers(window); // swaps double buffers (call after all render commands are completed)
    glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
  }
  scenes[sceneIndex]->deinit();

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
                                        NULL, // GLFWmonitor* Monitor: Specified for which monitor for fullscreen, NULL for windowed mode
                                        NULL); // GLFWwindow* Share: window to share resources with
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

void toggleWindowSize(GLFWwindow* window, const uint32 width, const uint32 height)
{
  local_persist bool windowMode = true;
  if (windowMode) {
    toFullScreenMode(window);
  } else{
    toWindowedMode(window, width, height);
  }
  windowMode = !windowMode;
}