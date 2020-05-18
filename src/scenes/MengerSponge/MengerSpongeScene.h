//
// Created by Connor on 11/21/2019.
//

#ifndef LEARNOPENGL_MENGERSPONGESCENE_H
#define LEARNOPENGL_MENGERSPONGESCENE_H

#include "../GodModeScene.h"

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
  MengerSpongeScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth);

  // FrameBufferSizeConsumer override
  void frameBufferSize(uint32 width, uint32 height) override;
  void runScene() override;

  void key_E_pressed() override;
  void key_Q_pressed() override;

private:

  Shader mengerSpongeShader;
  Shader pixel2DShader;
  Shader cubeShader;
  uint32 textureId;

  float32 deltaTime = 0;
  float32 lastFrame = 0;

  uint32 frameBuffer = 0;
  uint32 frameBufferTexture = 0;
  uint32 rbo = 0;

  void renderLoop(uint32 quadVAO, uint32 cubeVAO);

  uint32 currentResolutionIndex = 0;
  resolution currentResolution = screenResolutions[currentResolutionIndex];
};


#endif //LEARNOPENGL_MENGERSPONGESCENE_H
