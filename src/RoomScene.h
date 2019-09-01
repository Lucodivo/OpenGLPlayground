#pragma once

#include "GodModeScene.h"

class RoomScene : public GodModeScene
{
public:
  RoomScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth);
  void runScene();

private:

  Shader positionalLightShader;
  Shader singleColorShader;
  Shader depthCubeMapShader;

  float32 deltaTime = 0.0f;	// Time between current frame and last frame
  float32 lastFrame = 0.0f; // Time of last frame

  void renderLoop(uint32 cubeVAO, uint32 invertedNormCubeVAO);
  void generateDepthCubeMap(uint32& depthMapTextureId, uint32& depthMapFBO);
};