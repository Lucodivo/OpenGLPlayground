#pragma once

#include <glm/glm.hpp>

#include "../../Model.h"
#include "../../LearnOpenGLPlatform.h"
#include "../../common/Kernels.h"
#include "../FirstPersonScene.h"
#include "../../common/ObjectData.h"
#include "../../Shader.h"

class NessCubesScene final : public FirstPersonScene
{
public:
  NessCubesScene(GLFWwindow* window);
  void runScene() override;

  // FrameBufferSizeConsumer override
  void framebufferSizeChange(uint32 width, uint32 height) override;
  void init(uint32 windowWidth, uint32 windowHeight);
  void deinit();
  void drawFrame();

  // KeyboardConsumer overrides
  void key_LeftMouseButton_pressed(float32 xPos, float32 yPos) override;
  void key_E_released() override;
  void key_Q_released() override;

  // ControllerConsumer overrides
  void button_X_pressed() override;
  void button_dPadUp_pressed() override;
  void button_dPadDown_pressed() override;

private:
  Shader* cubeShader = NULL;
  Shader* lightShader = NULL;
  Shader* modelShader = NULL;
  Shader* stencilShader = NULL;
  Shader* frameBufferShader = NULL;
  Shader* skyboxShader = NULL;

  VertexAtt lightVertexAtt;
  VertexAtt cubeVertexAtt;
  VertexAtt quadVertexAtt;
  VertexAtt skyboxVertexAtt;

  uint32 cubeDiffTextureId;
  uint32 cubeSpecTextureId;
  uint32 skyboxTextureId;

  Model* nanoSuitModel;

  uint32 globalFSUniformBuffer;
  const uint32 globalFSBufferBindIndex = 0;

  uint32 globalVSUniformBuffer;
  const uint32 globalVSBufferBindIndex = 1;
  const uint32 globalVSBufferViewMatOffset = sizeof(glm::mat4);

  glm::mat4 projectionMat;

  const float32 lightOrbitSpeed = 1.0f;
  const float32 lightOrbitRadius = 2.5f;
  const float32 lightScale = 0.2f;

  const float32 cubRotAngle = 7.3f;

  // frame rate
  float32 deltaTime = 0.0f;  // Time between current frame and last frame
  float32 lastFrame = 0.0f; // Time of last frame

  bool flashLightOn = false;

  Framebuffer framebuffer = {};

  uint32 selectedKernelIndex = 0;

  double kernelModeSwitchTimer = 0.0f;
  uint32 kernelCount = ArrayCount(kernels5x5);

  void initializeTextures(uint32& diffTextureId, uint32& specTextureId, uint32& skyboxTextureId);
  void toggleFlashlight();
  void nextImageKernel();
  void prevImageKernel();
};