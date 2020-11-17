#pragma once

#include "../Scene.h"
#include "../../common/Input.h"
#include "../../common/ObjectData.h"

class Pixel2DScene : public Scene
{
public:
  Pixel2DScene();
  void init(uint32 windowWidth, uint32 windowHeight);
  Framebuffer drawFrame();
  void deinit();
  void inputStatesUpdated();
  const char* title();

private:

  Shader* pixel2DShader;

  VertexAtt quadVertexAtt;

  Framebuffer drawFramebuffer;

  uint32 textureId;

  uint32 textureWidth, textureHeight;

  float32 startTime = 0.0f;
  float32 deltaTime = 0.0f;  // Time between current frame and last frame
  float32 lastFrame = 0.0f; // Time of last frame
};
