//
// Created by Connor on 11/21/2019.
//

#pragma once

#include "../GodModeScene.h"
#include "../../common/ObjectData.h"

struct resolution {
  uint32 width;
  uint32 height;
};

const resolution screenResolutions[] = {
        {120, 68},
        {240, 135},
        {480, 270},
        {960, 540},
        {1920, 1080},
        {3840, 2160}
};

class MengerSpongeScene final : public GodModeScene {
public:
  MengerSpongeScene(GLFWwindow* window);

  void init(uint32 windowWidth, uint32 windowHeight);
  void deinit();
  void drawFrame();
  void inputStatesUpdated();
  void drawGui();
  const char* title();

private:

  GLFWwindow* window = NULL;

  Shader* mengerSpongeShader = NULL;
  Shader* pixel2DShader = NULL;
  Shader* cubeShader = NULL;

  VertexAtt quadVertexAtt = {};
  VertexAtt cubeVertexAtt = {};
  Framebuffer dynamicResolutionFBO = {};

  uint32 textureDiff1Id;
  uint32 textureSpec1Id;
  uint32 textureDiff2Id;
  uint32 textureSpec2Id;

  uint32 textureWidth;
  uint32 textureHeight;

  glm::mat4 projectionMat;

  bool showDebugWindows = false;

  float32 startTime = 0;
  float32 deltaTime = 0;
  float32 lastFrame = 0;
  float32 lastShaderFileUpdateCheck = 0;
  const float32 shaderFileCheckIntervalInSeconds = 2.0f;

  uint32 currentResolutionIndex = 1;
  resolution currentResolution = screenResolutions[currentResolutionIndex];

  int32 numSamples = 1;

  const float cubeScale = 10.0f;
  const float32 frameTime = 0.2f;

  const glm::vec3 directionalLightAmb = glm::vec3(0.15, 0.15, 0.15);
  const glm::vec3 directionalLightDiff = glm::vec3(0.6, 0.6, 0.6);
  const glm::vec3 directionalLightSpec = glm::vec3(0.8, 0.8, 0.8);
  const glm::vec3 directionalLightDir = glm::vec3(1.0, -1.0, -1.0);
  const glm::vec3 cubePos = glm::vec3(0.0, 0.0, 0.0);
  const glm::vec3 cubeRotAxis = glm::vec3(1.0, 1.0, -1.0);

  int32 originalCursorInputMode = GLFW_CURSOR_DISABLED;
};
