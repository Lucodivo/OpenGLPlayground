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
  void runScene() override;

  // FrameBufferSizeConsumer override
  void frameBufferSize(uint32 width, uint32 height) override;

  // KeyboardConsumer overrides
  void key_LeftMouseButton_pressed(float32 xPos, float32 yPos) override;
  void key_E_released() override;
  void key_Q_released() override;

  // ControllerConsumer overrides
  void button_X_pressed() override;
  void button_dPadUp_pressed() override;
  void button_dPadDown_pressed() override;

private:
  Shader cubeShader;
  Shader lightShader;
  Shader modelShader;
  Shader stencilShader;
  Shader frameBufferShader;
  Shader skyboxShader;

  // frame rate
  float32 deltaTime = 0.0f;  // Time between current frame and last frame
  float32 lastFrame = 0.0f; // Time of last frame

  bool flashLightOn = false;

  Framebuffer framebuffer = {};

  uint32 selectedKernelIndex = 0;

  double kernelModeSwitchTimer = 0.0f;
  uint32 kernelCount = ArrayCount(kernels5x5);

  void renderLoop(uint32& shapesVAO, uint32& lightVAO, uint32& quadVAO, uint32& skyboxVAO);
  void initializeTextures(uint32& diffTextureId, uint32& specTextureId, uint32& skyboxTextureId);
  void toggleFlashlight();
  void nextImageKernel();
  void prevImageKernel();
};