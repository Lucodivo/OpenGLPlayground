//
// Created by Connor on 11/12/2019.
//

#ifndef LEARNOPENGL_RAYMARCHINGSCENE_H
#define LEARNOPENGL_RAYMARCHINGSCENE_H

#include "../FirstPersonScene.h"
#include "../GodModeScene.h"

class RayMarchingScene final : public GodModeScene {
public:
  RayMarchingScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth);


  // FrameBufferSizeConsumer override
  void frameBufferSize(uint32 width, uint32 height) override;

  void runScene() override;

private:

  Shader rayMarchingShader;

  float32 deltaTime = 0;
  float32 lastFrame = 0;

  uint32 frameBuffer = 0;
  uint32 frameBufferTexture = 0;
  uint32 rbo = 0;

  void renderLoop(uint32 quadVAO);
};

#endif //LEARNOPENGL_RAYMARCHINGSCENE_H
