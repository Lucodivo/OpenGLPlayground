#pragma once

#include <map>

#include "../LearnOpenGLPlatform.h"
#include "../common/Input.h"
#include "../Shader.h"
#include "../common/Util.h"

class Scene
{
public:
  Scene(){};
  // TODO: switch params to int32 due to GLFW returning signed integers?
  virtual void init(uint32 windowWidth, uint32 windowHeight);
  virtual Framebuffer drawFrame() = 0; // draws scene to framebuffer and returns that framebuffer
  virtual void deinit() {}
  virtual void drawGui() {};
  virtual void inputStatesUpdated();
  virtual const char* title();

protected:
  uint32 windowWidth = 0;
  uint32 windowHeight = 0;

private:
  // TODO: change below to use Extent2D
  void framebufferSizeChange(uint32 width, uint32 height);
};