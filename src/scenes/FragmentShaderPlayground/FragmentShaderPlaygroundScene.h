//
// Created by Connor on 11/21/2019.
//

#ifndef LEARNOPENGL_FRAGMENTSHADERPLAYGROUNDSCENE_H
#define LEARNOPENGL_FRAGMENTSHADERPLAYGROUNDSCENE_H

#include "../FirstPersonScene.h"

class FragmentShaderPlaygroundScene final : public FirstPersonScene {
public:
  FragmentShaderPlaygroundScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth);

  // FrameBufferSizeConsumer override
  void frameBufferSize(uint32 width, uint32 height) override;
  void runScene() override;

  // KeyboardConsumer override
  void key_LeftMouseButton_pressed(float32 xPos, float32 yPos) override;
  void key_LeftMouseButton_released(float32 xPos, float32 yPos) override;

  // MouseMovementConsumer override
  void mouseMovement(float32 xOffset, float32 yOffset);

  // MouseScrollConsumer override
  void mouseScroll(float32 yOffset) override;

private:

  Shader playgroundShader;

  float32 deltaTime = 0;
  float32 lastFrame = 0;

  float32 zoom = 0.5f;
  glm::vec2 centerOffset = glm::vec2(0.0f, 0.0f);
  bool mouseDown = false;

  uint32 frameBuffer = 0;
  uint32 frameBufferTexture = 0;
  uint32 rbo = 0;

  void renderLoop(uint32 quadVAO);
};


#endif //LEARNOPENGL_FRAGMENTSHADERPLAYGROUNDSCENE_H
