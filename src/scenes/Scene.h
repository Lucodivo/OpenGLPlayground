#pragma once

#include <map>

#include "../LearnOpenGLPlatform.h"
#include "../common/Input.h"
#include "../Shader.h"

class Scene : public KeyboardConsumer, public InputConsumer_
{
public:
  Scene(){};
  // TODO: switch params to int32 due to GLFW returning signed integers?
  virtual void init(uint32 windowWidth, uint32 windowHeight);
  virtual void deinit() {} // de-initializes scene
  virtual void drawFrame() {} // draws scene to back buffer
  virtual void inputStatesUpdated();

protected:
  uint32 windowWidth = 0;
  uint32 windowHeight = 0;

private:
  // TODO: change below to use Extent2D
  void framebufferSizeChange(uint32 width, uint32 height);
};