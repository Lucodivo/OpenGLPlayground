#include "SceneManager.h"

#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

#include "../TextDebugShader.h"
#include "../common/Input.h"

#include "NessCube/NessCubesScene.h"
#include "InfiniteCube/InfiniteCubeScene.h"
#include "ReflectRefract/ReflectRefractScene.h"
#include "AsteroidBelt/AsteroidBeltScene.h"
#include "Moon/MoonScene.h"
#include "Room/RoomScene.h"
#include "GUIScene/GUIScene.h"
#include "InfiniteCapsules/InfiniteCapsulesScene.h"
#include "Mandelbrot/MandelbrotScene.h"
#include "MengerSponge/MengerSpongeScene.h"
#include "RayTracingSphere/RayTracingSphereScene.h"
#include "Pixel2D/Pixel2DScene.h"

void toggleWindowSize(GLFWwindow* window, const uint32 width, const uint32 height);

class InputConsumer_ : public KeyboardConsumer, public WindowSizeConsumer {
public:
  InputConsumer_(GLFWwindow* window, Scene** scenes, uint32* sceneIndex, uint32 sceneCount,
                 uint32* windowWidth, uint32* windowHeight, TextDebugShader* textShader){
    this->window = window;
    this->scenes = scenes;
    this->sceneIndex = sceneIndex;
    this->sceneCount = sceneCount;
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    this->textShader = textShader;
  }
  GLFWwindow* window;
  Scene** scenes;
  TextDebugShader* textShader;
  uint32* sceneIndex;
  uint32 sceneCount;
  uint32* windowWidth;
  uint32* windowHeight;
  void key_O_pressed() {
    scenes[*sceneIndex]->deinit();
    if(*sceneIndex == 0) {
      *sceneIndex = sceneCount - 1;
    } else { --(*sceneIndex); }
    scenes[*sceneIndex]->init(*windowWidth, *windowHeight);
  }
  void key_P_pressed() {
    scenes[*sceneIndex]->deinit();
    if(++(*sceneIndex) == sceneCount) {
      *sceneIndex = 0;
    }
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
    textShader->updateWindowDimens(width, height);
  }
};


void runScenes(GLFWwindow* window) {
  uint32 windowWidth = VIEWPORT_INIT_WIDTH;
  uint32 windowHeight = VIEWPORT_INIT_HEIGHT;

  TextDebugShader textDebugShader = TextDebugShader(windowWidth, windowHeight);

  uint32 sceneIndex = 0;
  NessCubesScene nessCubeScene = NessCubesScene();
  InfiniteCapsulesScene infiniteCapsulesScene = InfiniteCapsulesScene();
  InfiniteCubeScene infiniteCubeScene = InfiniteCubeScene();
  AsteroidBeltScene asteroidBeltScene = AsteroidBeltScene();
  MandelbrotScene mandelbrotScene = MandelbrotScene(window);
  RayTracingSphereScene rayTracingSphereScene = RayTracingSphereScene();
  MengerSpongeScene mengerSpongeScene = MengerSpongeScene(window);
  MoonScene moonScene = MoonScene();
  RoomScene roomScene = RoomScene();
  ReflectRefractScene reflectRefractScene = ReflectRefractScene();
  GUIScene guiScene = GUIScene(window);
  //Pixel2DScene pixel2DScene = Pixel2DScene();
  Scene* scenes[] = { &mengerSpongeScene, &rayTracingSphereScene, &mandelbrotScene, &infiniteCubeScene,
                      &infiniteCapsulesScene, &roomScene, &guiScene, &moonScene, &asteroidBeltScene,
                      &reflectRefractScene, &nessCubeScene };

  InputConsumer_ inputConsumer = InputConsumer_(window, scenes, &sceneIndex, ArrayCount(scenes), &windowWidth, &windowHeight, &textDebugShader);
  subscribeKeyboardInput(&inputConsumer);
  subscribeWindowSize(&inputConsumer);

  scenes[sceneIndex]->init(windowWidth, windowHeight);
  float32 deltaTime = 1.0;
  float32 lastFrame = (float32)glfwGetTime();
  while (glfwWindowShouldClose(window) == GL_FALSE)
  {
    processInput(window);

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    scenes[sceneIndex]->drawFrame();

    // debug text
    uint32 numFrames = (uint32)(1 / deltaTime);
    textDebugShader.renderText(std::to_string(numFrames) + " FPS", 25.0f, 25.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    float32 t = (float32)glfwGetTime();
    deltaTime = t - lastFrame;
    lastFrame = t;

    // Rendering ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window); // swaps double buffers (call after all render commands are completed)
    glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
  }
  scenes[sceneIndex]->deinit();

  glfwTerminate(); // clean up gl resources
}

void toggleWindowSize(GLFWwindow* window, const uint32 width, const uint32 height)
{
  local_access bool windowMode = true;
  if (windowMode) {
    toFullScreenMode(window);
  } else{
    toWindowedMode(window, width, height);
  }
  windowMode = !windowMode;
}