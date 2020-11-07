#pragma once

#include "scenes/NessCube/NessCubesScene.h"
#include "scenes/InfiniteCube/InfiniteCubeScene.h"
#include "scenes/ReflectRefract/ReflectRefractScene.h"
#include "scenes/AsteroidBelt/AsteroidBeltScene.h"
#include "scenes/Moon/MoonScene.h"
#include "scenes/Room/RoomScene.h"
#include "scenes/GUIScene/GUIScene.h"
#include "scenes/InfiniteCapsules/InfiniteCapsulesScene.h"
#include "scenes/Mandelbrot/MandelbrotScene.h"
#include "scenes/MengerSponge/MengerSpongeScene.h"
#include "scenes/RayTracingSphere/RayTracingSphereScene.h"
#include "scenes/Pixel2D/Pixel2DScene.h"

#define VIEWPORT_INIT_WIDTH 1920
#define VIEWPORT_INIT_HEIGHT 1080

void runScenes(GLFWwindow* window) {
  uint32 windowWidth = VIEWPORT_INIT_WIDTH;
  uint32 windowHeight = VIEWPORT_INIT_HEIGHT;

  uint32 sceneIndex = 0;
  NessCubesScene nessCubeScene = NessCubesScene(window);
  InfiniteCapsulesScene infiniteCapsulesScene = InfiniteCapsulesScene(window);
  InfiniteCubeScene infiniteCubeScene = InfiniteCubeScene(window);
  AsteroidBeltScene asteroidBeltScene = AsteroidBeltScene(window);
  MandelbrotScene mandelbrotScene = MandelbrotScene(window);
  RayTracingSphereScene rayTracingSphereScene = RayTracingSphereScene(window);
  MengerSpongeScene mengerSpongeScene = MengerSpongeScene(window);
  MoonScene moonScene = MoonScene(window);
  RoomScene roomScene = RoomScene(window);
  ReflectRefractScene reflectRefractScene = ReflectRefractScene(window);
  Scene* scenes[] = { &mengerSpongeScene, &rayTracingSphereScene, &mandelbrotScene, &infiniteCubeScene,
                      &infiniteCapsulesScene, &roomScene, &moonScene, &asteroidBeltScene,
                      &reflectRefractScene, &nessCubeScene };

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