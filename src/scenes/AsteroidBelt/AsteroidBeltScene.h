#pragma once

#include "../FirstPersonScene.h"

class AsteroidBeltScene : public FirstPersonScene
{
public:
  AsteroidBeltScene(GLFWwindow* window);
  void runScene() override;

private:

  Shader modelShader;
  Shader modelInstanceShader;
  Shader reflectModelInstanceShader;
  Shader skyboxShader;

  float32 deltaTime = 0.0f;  // Time between current frame and last frame
  float32 lastFrame = 0.0f; // Time of last frame

  void renderLoop(uint32 skyboxVAO);
};