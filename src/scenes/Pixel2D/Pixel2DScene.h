#pragma once

#include "../Scene.h"
#include "../../common/Input.h"

class Pixel2DScene : public Scene
{
public:
  Pixel2DScene(GLFWwindow* window);
  void init(uint32 windowWidth, uint32 windowHeight);
  void deinit();
  void drawFrame();

  void runScene();

private:

  Shader pixel2DShader;

  float32 deltaTime = 0.0f;  // Time between current frame and last frame
  float32 lastFrame = 0.0f; // Time of last frame

  void renderLoop(uint32 skyboxVAO);
};
