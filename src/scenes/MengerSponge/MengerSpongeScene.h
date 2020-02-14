//
// Created by Connor on 11/21/2019.
//

#ifndef LEARNOPENGL_MENGERSPONGESCENE_H
#define LEARNOPENGL_MENGERSPONGESCENE_H

#include "../GodModeScene.h"

class MengerSpongeScene final : public GodModeScene {
public:
  MengerSpongeScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth);

  // FrameBufferSizeConsumer override
  void frameBufferSize(uint32 width, uint32 height) override;
  void runScene() override;

private:

  Shader mengerSpongeShader;

  float32 deltaTime = 0;
  float32 lastFrame = 0;

  uint32 frameBuffer = 0;
  uint32 frameBufferTexture = 0;
  uint32 rbo = 0;

  void renderLoop(uint32 quadVAO);
};


#endif //LEARNOPENGL_MENGERSPONGESCENE_H
