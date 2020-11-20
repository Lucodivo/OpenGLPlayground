//
// Created by Connor on 11/21/2019.
//

#pragma once

#include "../FirstPersonScene.h"
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

class MengerSpongeScene final : public FirstPersonScene {
public:
  MengerSpongeScene(GLFWwindow* window);

  void init(Extent2D windowExtent);
  Framebuffer drawFrame();
  void deinit();
  void inputStatesUpdated();
  void drawGui();
  const char* title();

private:

  GLFWwindow* window = NULL;

  ShaderProgram* mengerSpongeShader = NULL;
  ShaderProgram* pixel2DShader = NULL;
  ShaderProgram* cubeShader = NULL;

  VertexAtt quadVertexAtt;
  VertexAtt cubeVertexAtt;
  Framebuffer dynamicResolutionFBO;
  Framebuffer publicPseudoDrawFramebuffer; // NOTE: this psuedo-framebuffer lets us hide the true size of our FBO to any caller

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
  Timer timer;

  uint32 currentResolutionIndex = 0;
  resolution currentResolution = screenResolutions[currentResolutionIndex];

  int32 numSamples = 1;

  const float cubeScale = 10.0f;
  const float32 frameTime = 0.2f;

  const glm::vec3 directionalLightAmb = glm::vec3(0.15, 0.15, 0.15);
  const glm::vec3 directionalLightDiff = glm::vec3(0.6, 0.6, 0.6);
  const glm::vec3 directionalLightSpec = glm::vec3(0.8, 0.8, 0.8);
  const glm::vec3 directionalLightDir = glm::vec3(1.0, -1.0, -1.0);
  const glm::vec3 cubeRotAxis = glm::vec3(1.0, 1.0, -1.0);
};
