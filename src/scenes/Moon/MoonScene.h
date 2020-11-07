#pragma once

#include "../GodModeScene.h"

class MoonScene : public GodModeScene
{
public:
  MoonScene(GLFWwindow* window);
  void runScene();
  void init(uint32 windowWidth, uint32 windowHeight);
  void deinit();
  void drawFrame();

private:

  Shader directionalLightShader;
  Shader quadTextureShader;
  Shader depthMapShader;

  float32 deltaTime = 0.0f;  // Time between current frame and last frame
  float32 lastFrame = 0.0f; // Time of last frame

  void renderLoop(uint32 quadVAO, uint32 cubeVAO);
  void generateDepthMap(uint32& depthMapTextureId, uint32& depthMapFBO);
};