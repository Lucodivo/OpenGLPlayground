#pragma once

#include "../Scene.h"
#include "../../common/Input.h"
#include "../../common/ObjectData.h"
#include "../../ShaderProgram.h"

class Pixel2DScene : public Scene
{
public:
  Pixel2DScene();
  void init(Extent2D windowExtent);
  Framebuffer drawFrame();
  void deinit();
  virtual void framebufferSizeChangeRequest(Extent2D windowExtent);
  const char* title();

private:

  ShaderProgram* pixel2DShader;

  VertexAtt quadVertexAtt;

  Framebuffer drawFramebuffer;

  uint32 textureId;

  uint32 textureWidth, textureHeight;

  float32 startTime = 0.0f;
  float32 deltaTime = 0.0f;  // Time between current frame and last frame
  float32 lastFrame = 0.0f; // Time of last frame
};
