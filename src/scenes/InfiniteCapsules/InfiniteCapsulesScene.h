//
// Created by Connor on 11/12/2019.
//

#pragma once

#include "../FirstPersonScene.h"
#include "../../common/ObjectData.h"

class InfiniteCapsulesScene final : public FirstPersonScene {
public:
  InfiniteCapsulesScene();
  void init(Extent2D windowExtent);
  Framebuffer drawFrame();
  void deinit();
  virtual void inputStatesUpdated();
  const char* title();

private:
  ShaderProgram* rayMarchingShader = NULL;

  float32 startTime = 0;
  float32 deltaTime = 0;
  float32 lastFrame = 0;

  VertexAtt quadVertexAtt;

  Framebuffer drawFramebuffer;

  glm::vec3 lightPosition = { 0.0f, 0.0f, 0.0f };
  bool lightAlive = false;
  glm::vec3 lightMoveDir;
  float lightDistanceTraveled;
};
