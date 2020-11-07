//
// Created by Connor on 11/21/2019.
//

#ifndef LEARNOPENGL_RAYTRACINGSPHERESCENE_H
#define LEARNOPENGL_RAYTRACINGSPHERESCENE_H

#include "../GodModeScene.h"
#include "../../common/ObjectData.h"

class RayTracingSphereScene final : public GodModeScene {
public:
  RayTracingSphereScene();
  void init(uint32 windowWidth, uint32 windowHeight);
  void deinit();
  void drawFrame();

  // FrameBufferSizeConsumer override
  void framebufferSizeChange(uint32 width, uint32 height) override;

private:

  Shader* rayTracingSphereShader = NULL;

  VertexAtt quadVertexAtt = {};

  float32 deltaTime = 0;
  float32 lastFrame = 0;
  float32 startTime = 0;
};


#endif //LEARNOPENGL_RAYTRACINGSPHERESCENE_H
