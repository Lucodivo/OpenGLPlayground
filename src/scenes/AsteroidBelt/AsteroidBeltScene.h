#pragma once

#include "../FirstPersonScene.h"
#include "../../common/ObjectData.h"
#include "../../Model.h"

class AsteroidBeltScene : public FirstPersonScene
{
public:
  AsteroidBeltScene();
  void init(Extent2D windowExtent);
  Framebuffer drawFrame();
  void deinit();
  void inputStatesUpdated();
  const char* title();

private:

  Shader* modelShader;
  Shader* modelInstanceShader;
  Shader* reflectModelInstanceShader;
  Shader* skyboxShader;

  Model* planetModel = NULL;
  Model* asteroidModel = NULL;

  Framebuffer drawFramebuffer;

  VertexAtt skyboxVertexAtt = {};

  uint32 asteroidModelMatrixBuffer;

  uint32 skyboxTextureId;
  uint32 skybox2TextureId;

  const float32 planetRotationSpeed = 5.0f;
  const uint32 numAsteroids = 5000;

  float32 deltaTime = 0.0f;  // Time between current frame and last frame
  float32 lastFrame = 0.0f; // Time of last frame
};