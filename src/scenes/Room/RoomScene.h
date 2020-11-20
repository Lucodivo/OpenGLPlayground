#pragma once

#include "../FirstPersonScene.h"
#include "../../common/ObjectData.h"

class RoomScene : public FirstPersonScene
{
public:
  RoomScene();
  void init(Extent2D windowExtent);
  Framebuffer drawFrame();
  void deinit();
  void inputStatesUpdated();
  const char* title();

private:

  ShaderProgram* positionalLightShader;
  ShaderProgram* singleColorShader;
  ShaderProgram* depthCubeMapShader;

  VertexAtt cubeVertexAtt;
  VertexAtt invertedNormCubeVertexAtt;

  Framebuffer drawFramebuffer;

  uint32 depthCubeMapId, depthMapFBO;
  uint32 wallpaperTextureId, cubeTextureId;

  glm::mat4 cubeModelMat[3];

  float32 deltaTime = 0.0f;  // Time between current frame and last frame
  float32 lastFrame = 0.0f; // Time of last frame

  glm::vec3 lightColor = {0.8f, 0.8f, 0.8f};
  const float32 lightScale = 0.3f;
  const float32 lightRadius = 8.0f;
  const float32 lightAmplitude = 8.0f;

  const float32 roomScale = 32.0f;
  const glm::vec3 roomPosition = { 0.0f, -3.0f, 0.0f };

  const float32 cubeScales[3] = { 3.0f, 2.2f, 1.7f };
  const glm::vec3 cubePositions[3] = { { 0.0f, roomPosition.y + (cubeScales[0] / 2.0f), 0.0f},
                                       { 8.0f, roomPosition.y + (cubeScales[1] / 2) - 8.0f, 8.0f},
                                       { 2.0f, roomPosition.y + (cubeScales[2] / 2.0f) + 4.0f, 3.0f} };

  glm::mat4 lightProjMat;

  glm::mat4 roomModelMat;

  uint32 globalVSUniformBuffer;
  uint32 globalVSBufferBindIndex = 0;
  uint32 globalVSBufferViewMatOffset = sizeof(glm::mat4);

  const uint32 depthMap2DSamplerIndex = 2;

  void generateDepthCubeMap();
};