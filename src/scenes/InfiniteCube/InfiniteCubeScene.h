#pragma once

#include <glm/glm.hpp>

#include "../../LearnOpenGLPlatform.h"
#include "../../common/Kernels.h"
#include "../FirstPersonScene.h"
#include "../../common/ObjectData.h"
#include "../../Shader.h"

const glm::vec3 colors[] = {
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.875f, 0.125f, 0.0f),
        glm::vec3(0.75f, 0.25f, 0.0f),
        glm::vec3(0.625f, 0.375f, 0.0f),
        glm::vec3(0.5f, 0.5f, 0.0f),
        glm::vec3(.375f, 0.625f, 0.0f),
        glm::vec3(0.25f, 0.75f, 0.0f),
        glm::vec3(0.125f, 0.875f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.875f, 0.125f),
        glm::vec3(0.0f, 0.75f, 0.25f),
        glm::vec3(0.0f, 0.625f, 0.375f),
        glm::vec3(0.0f, 0.5f, 0.5f),
        glm::vec3(0.0f, 0.375f, 0.625f),
        glm::vec3(0.0f, 0.25f, 0.75f),
        glm::vec3(0.0f, 0.125f, 0.875f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.125f, 0.0f, 0.875f),
        glm::vec3(0.25f, 0.0f, 0.75f),
        glm::vec3(0.375f, 0.0f, 0.625f),
        glm::vec3(0.5f, 0.0f, 0.5f),
        glm::vec3(0.625f, 0.0f, 0.375f),
        glm::vec3(0.75f, 0.0f, 0.25f),
        glm::vec3(0.875f, 0.0f, 0.125f),
};

class InfiniteCubeScene final : public FirstPersonScene
{
public:
  InfiniteCubeScene();
  void init(uint32 windowWidth, uint32 windowHeight);
  void deinit();
  void drawFrame();

  void framebufferSizeChange(uint32 width, uint32 height) override;

private:
  Shader* cubeShader = NULL;
  Shader* cubeOutlineShader = NULL;

  VertexAtt cubeVertexAtt;
  VertexAtt quadVertexAtt;

  uint32 outlineTexture;
  uint32 globalVSUniformBufferID;
  uint32 globalVSBufferBindIndex = 0;
  uint32 globalVSBufferViewMatOffset = sizeof(glm::mat4);
  const float32 cubeRotationAngle = 2.5f;

  // frame rate
  float32 deltaTime = 0.0f;  // Time between current frame and last frame
  float32 lastFrame = 0.0f; // Time of last frame

  Framebuffer framebuffer;
  uint32 framebufferDimen;
  uint32 colorIndex = 0;
};