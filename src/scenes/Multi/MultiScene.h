#pragma once

#include "../Scene.h"

class MultiScene : public Scene
{
public:
  MultiScene(Scene** fourScenes, uint32 sceneCount, uint32 startingIndex = 0);
  void init(Extent2D windowExtent);
  Framebuffer drawFrame();
  void deinit();
  void drawGui();
  void framebufferSizeChangeRequest(Extent2D windowExtent);
  void inputStatesUpdated();
  const char* title() { return "Multi Scene"; }

private:
  Extent2D quarterWindowExtent;

  Scene** scenes;
  uint32 sceneCount;

  uint32 startingIndex;
  Scene* fourScenes[4];
  Framebuffer fourFramebuffers[4];

  Framebuffer drawFramebuffer;
};


