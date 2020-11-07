//
// Created by Connor on 11/21/2019.
//

#ifndef LEARNOPENGL_RAYTRACINGSPHERESCENE_H
#define LEARNOPENGL_RAYTRACINGSPHERESCENE_H

#include "../GodModeScene.h"

class RayTracingSphereScene final : public GodModeScene {
public:
  RayTracingSphereScene(GLFWwindow* window);
  void runScene() override;
  void init(uint32 windowWidth, uint32 windowHeight);
  void deinit();
  void drawFrame();

  // FrameBufferSizeConsumer override
  void framebufferSizeChange(uint32 width, uint32 height) override;

private:

  Shader rayTracingSphereShader;

  float32 deltaTime = 0;
  float32 lastFrame = 0;

  void renderLoop(uint32 quadVAO);
};


#endif //LEARNOPENGL_RAYTRACINGSPHERESCENE_H
