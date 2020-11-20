#pragma once

#include "../FirstPersonScene.h"
#include "../../common/ObjectData.h"

class MoonScene : public FirstPersonScene
{
public:
  MoonScene();
  void init(Extent2D windowExtent);
  Framebuffer drawFrame();
  void deinit();
  void inputStatesUpdated();
  const char* title();

private:

  ShaderProgram* directionalLightShader;
  ShaderProgram* quadTextureShader;
  ShaderProgram* depthMapShader;

  VertexAtt floorVertexAtt;
  VertexAtt cubeVertexAtt;

  uint32 floorAlbedoTextureId, floorNormalTextureId, floorHeightTextureId;
  uint32 cube1AlbedoTextureId, cube1NormalTextureId, cube1HeightTextureId;
  uint32 cube2AlbedoTextureId, cube2NormalTextureId, cube2HeightTextureId;
  uint32 cube3AlbedoTextureId, cube3NormalTextureId, cube3HeightTextureId;
  uint32 lightTextureId;

  Framebuffer drawFramebuffer;
  Framebuffer depthMapFramebuffer;
  uint32 depthMap2DSamplerIndex;

  float32 startTime = 0.0f;
  float32 deltaTime = 0.0f;  // Time between current frame and last frame
  float32 lastFrame = 0.0f; // Time of last frame

  uint32 globalVSUniformBuffer;
  uint32 globalVSBufferBindIndex = 0;
  uint32 globalVSBufferViewMatOffset = sizeof(glm::mat4);

  glm::mat4 floorModelMat;
  glm::mat4 lightProjMat;

  const glm::vec3 lightColor = glm::vec3(1.0f);
  const float32 lightScale = 8.0f;

  const float32 floorScale = 16.0f;
  const glm::vec3 floorPosition = glm::vec3(0.0f, -3.0f, 0.0f);

  const float32 cubeScale2 = 3.0f;
  const float32 cubeScale3 = 1.7f;

  const float32 lightRadius = 48.0f;
  const float32 lightHeightOffset = 20.0f;
  const float32 lightHeightHalfVariance = 12.0f;
  const float32 lightCameraDistance = 32.0f;

  void generateDepthMap();
};