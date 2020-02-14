//
// Created by Connor on 11/21/2019.
//

#ifndef LEARNOPENGL_MANDELBROTSCENE_H
#define LEARNOPENGL_MANDELBROTSCENE_H

#include "../FirstPersonScene.h"

#define ZOOM_SPEED_NORMAL 1.0f
#define ZOOM_SPEED_FAST 3.0f
#define MOUSE_ACTION_TIME_SECONDS 0.15f

class MandelbrotScene final : public FirstPersonScene {
public:
  MandelbrotScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth);

  // FrameBufferSizeConsumer override
  void frameBufferSize(uint32 width, uint32 height) override;
  void runScene() override;

  // KeyboardConsumer override
  void key_LeftShift_pressed() override;
  void key_LeftShift_released() override;
  void key_LeftMouseButton_pressed(float32 xPos, float32 yPos) override;
  void key_LeftMouseButton_released(float32 xPos, float32 yPos) override;

  // MouseMovementConsumer override
  void mouseMovement(float32 xOffset, float32 yOffset);

  // MouseScrollConsumer override
  void mouseScroll(float32 yOffset) override;

private:

  Shader mandelbrotShader;

  float zoomSpeed = ZOOM_SPEED_NORMAL;

  float32 deltaTime = 0;
  float32 lastFrame = 0;

  float32 zoom = 0.25f;
  glm::vec2 centerOffset = glm::vec2(0.0f, 0.0f);
  bool mouseDown = false;

  uint32 frameBuffer = 0;
  uint32 frameBufferTexture = 0;
  uint32 rbo = 0;

  glm::vec3 colorFavors[3] = { glm::vec3(1.0f, 0.33f, 0.66f),
                              glm::vec3(0.165f, 0.33f, 1.0f),
                              glm::vec3(0.5f, 1.0f, 0.5f) };
  int32 currentColorFavorIndex = 0;
  float32 mouseDownTime = 0.0f;

  void renderLoop(uint32 quadVAO);
};


#endif //LEARNOPENGL_MANDELBROTSCENE_H
