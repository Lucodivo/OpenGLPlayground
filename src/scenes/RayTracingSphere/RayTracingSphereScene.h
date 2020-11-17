//
// Created by Connor on 11/21/2019.
//

#ifndef LEARNOPENGL_RAYTRACINGSPHERESCENE_H
#define LEARNOPENGL_RAYTRACINGSPHERESCENE_H

#include "../FirstPersonScene.h"
#include "../../common/ObjectData.h"

class RayTracingSphereScene final : public FirstPersonScene {
public:
  RayTracingSphereScene();
  void init(uint32 windowWidth, uint32 windowHeight);
  Framebuffer drawFrame();
  void deinit();
  void inputStatesUpdated();
  const char* title();

private:

  Shader* rayTracingSphereShader = NULL;

  VertexAtt quadVertexAtt = {};

  Framebuffer drawFramebuffer;

  float32 deltaTime = 0;
  float32 lastFrame = 0;
  float32 startTime = 0;
};


#endif //LEARNOPENGL_RAYTRACINGSPHERESCENE_H
