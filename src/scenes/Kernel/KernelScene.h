#pragma once

#include <glm/glm.hpp>

#include "../../Model.h"
#include "../../LearnOpenGLPlatform.h"
#include "../../common/Kernels.h"
#include "../FirstPersonScene.h"
#include "../../common/ObjectData.h"
#include "../../Shader.h"

class KernelScene final : public FirstPersonScene
{
public:
  KernelScene();
  void init(uint32 windowWidth, uint32 windowHeight);
  void deinit();
  void drawFrame();
  Framebuffer getDrawFramebuffer();
  void inputStatesUpdated();
  const char* title();

private:
  Shader* cubeShader = NULL;
  Shader* lightShader = NULL;
  Shader* modelShader = NULL;
  Shader* stencilShader = NULL;
  Shader* framebufferShader = NULL;
  Shader* skyboxShader = NULL;

  VertexAtt lightVertexAtt;
  VertexAtt cubeVertexAtt;
  VertexAtt quadVertexAtt;
  VertexAtt skyboxVertexAtt;

  uint32 cubeDiffTextureId;
  uint32 cubeSpecTextureId;
  uint32 skyboxTextureId;

  const uint32 skyboxTextureIndex = 0;
  const uint32 nessCubeDiffuseTextureIndex = 1;
  const uint32 nessCubeSpecularTextureIndex = 2;
  const uint32 framebufferTextureIndex = 3;

  Model* nanoSuitModel;

  const float32 modelScale = 0.2f;
  glm::mat4 nanoSuitModelMatrix;

  uint32 globalFSUniformBuffer;
  const uint32 globalFSBufferBindIndex = 0;

  uint32 globalVSUniformBuffer;
  const uint32 globalVSBufferBindIndex = 1;
  const uint32 globalVSBufferViewMatOffset = sizeof(glm::mat4);

  const float32 lightOrbitSpeed = 1.0f;
  const float32 lightOrbitRadius = 2.5f;
  const float32 lightScale = 0.2f;

  const float32 cubRotAngle = 7.3f;

  // frame rate
  float32 deltaTime = 0.0f;  // Time between current frame and last frame
  float32 lastFrame = 0.0f; // Time of last frame

  bool flashLightOn = false;

  Framebuffer preprocessFramebuffer;
  Framebuffer postprocessFramebuffer;

  uint32 selectedKernelIndex = 0;

  double kernelModeSwitchTimer = 0.0f;
  uint32 kernelCount = ArrayCount(kernels5x5);

  void initializeTextures(uint32& diffTextureId, uint32& specTextureId, uint32& skyboxTextureId);
  void toggleFlashlight();
  void nextImageKernel();
  void prevImageKernel();
};