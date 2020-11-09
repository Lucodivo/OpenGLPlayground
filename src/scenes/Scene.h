#pragma once

#include <map>

#include "../LearnOpenGLPlatform.h"
#include "../common/Input.h"
#include "../Shader.h"

class Scene : public KeyboardConsumer
{
public:
  Scene();
  virtual void init(uint32 windowWidth, uint32 windowHeight);
  virtual void deinit() {} // de-initializes scene
  virtual void drawFrame() {} // draws scene to back buffer
  virtual void framebufferSizeChange(uint32 width, uint32 height);

protected:
  uint32 windowWidth = 0;
  uint32 windowHeight = 0;
};