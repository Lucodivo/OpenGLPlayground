#pragma once

#include <glm/glm.hpp>

#include "../../LearnOpenGLPlatform.h"
#include "../../common/Kernels.h"
#include "../FirstPersonScene.h"
#include "../../common/ObjectData.h"
#include "../../Shader.h"

class NessCubesScene final : public FirstPersonScene
{
public:
  NessCubesScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth);
  void runScene();

  // FrameBufferSizeConsumer override
  void frameBufferSize(uint32 width, uint32 height);

  // KeyboardConsumer overrides
  void key_LeftMouseButton_pressed();
  void key_Up();
  void key_Down();

  // ControllerConsumer overrides
  void button_X_pressed();
  void button_dPadUp_pressed();
  void button_dPadDown_pressed();

private:
  Shader cubeShader;
  Shader lightShader;
  Shader modelShader;
  Shader stencilShader;
  Shader frameBufferShader;
  Shader skyboxShader;

  // frame rate
  float32 deltaTime = 0.0f;	// Time between current frame and last frame
  float32 lastFrame = 0.0f; // Time of last frame

  bool flashLightOn = true;

  uint32 frameBuffer = 0;
  uint32 frameBufferTexture = 0;
  uint32 rbo = 0;

  uint32 selectedKernelIndex = 0;

  double kernelModeSwitchTimer = 0.0f;
  uint32 kernelCount = ArrayCount(kernels5x5);

  void renderLoop(GLFWwindow* window, uint32& cubeVAO, uint32& lightVAO, uint32& quadVAO, uint32& skyboxVAO);
  void initializeTextures(uint32& diffTextureId, uint32& specTextureId, uint32& skyboxTextureId);
  void toggleFlashlight();
  void nextImageKernel();
  void prevImageKernel();
};