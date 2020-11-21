#pragma once

#include "../LearnOpenGLPlatform.h"
#include "../common/OpenGLUtil.h"

class Scene
{
public:
  Scene(){};
  virtual void init(Extent2D windowExtent) { this->windowExtent = windowExtent; };
  virtual Framebuffer drawFrame() = 0; // draws scene to framebuffer and returns that framebuffer
  virtual void deinit() {}
  virtual void drawGui() {}
  virtual void inputStatesUpdated() {}
  virtual void framebufferSizeChangeRequest(Extent2D windowExtent) { this->windowExtent = windowExtent; }
  virtual const char* title() { return "unnamed Scene"; };

protected:
  Extent2D windowExtent = { 0, 0 };
};