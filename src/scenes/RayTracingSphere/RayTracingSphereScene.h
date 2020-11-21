//
// Created by Connor on 11/21/2019.
//

#pragma once

#include "../FirstPersonScene.h"
#include "../../common/ObjectData.h"
#include "../../ShaderProgram.h"

class RayTracingSphereScene final : public FirstPersonScene {
public:
  RayTracingSphereScene();
  void init(Extent2D windowExtent);
  Framebuffer drawFrame();
  void deinit();
  virtual void framebufferSizeChangeRequest(Extent2D windowExtent);
  const char* title();

private:

  ShaderProgram* rayTracingSphereShader = NULL;

  VertexAtt quadVertexAtt = {};

  Framebuffer drawFramebuffer;

  float32 deltaTime = 0;
  float32 lastFrame = 0;
  float32 startTime = 0;
};
