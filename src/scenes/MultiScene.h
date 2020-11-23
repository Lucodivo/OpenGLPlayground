#pragma once

#include "Scene.h"

// TODO: currently setup for infinite cube, infinite capsules, ray tracing sphere, and reflect refract
class MultiScene : public Scene
{
public:
  MultiScene(Scene** fourScenes);
  void init(Extent2D windowExtent);
  Framebuffer drawFrame();
  void deinit();
  void framebufferSizeChangeRequest(Extent2D windowExtent);
  void inputStatesUpdated();
  const char* title() { return "Multi Scene"; }

private:
  Extent2D quarterWindowExtent;
  Scene** fourScenes;
  Framebuffer fourFramebuffers[4];
  Framebuffer drawFramebuffer;
};


