#pragma once

#include <map>

#include "../LearnOpenGLPlatform.h"
#include "../common/Input.h"
#include "../ShaderProgram.h"
#include "../common/OpenGLUtil.h"

class Scene
{
public:
  Scene(){};
  virtual void init(Extent2D windowExtent);
  virtual Framebuffer drawFrame() = 0; // draws scene to framebuffer and returns that framebuffer
  virtual void deinit() {}
  virtual void drawGui() {};
  virtual void inputStatesUpdated();
  virtual const char* title();

protected:
  Extent2D windowExtent = { 0, 0 };

private:
  void framebufferSizeChange(Extent2D windowExtent);
};