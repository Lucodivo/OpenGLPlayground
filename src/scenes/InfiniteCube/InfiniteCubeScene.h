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
  InfiniteCubeScene(GLFWwindow* window);
  void runScene() override;
  void framebufferSizeChange(uint32 width, uint32 height) override;

private:
  Shader cubeShader;
  Shader cubeOutlineShader;

  // frame rate
  float32 deltaTime = 0.0f;  // Time between current frame and last frame
  float32 lastFrame = 0.0f; // Time of last frame

  Framebuffer framebuffer;
  uint32 framebufferDimen;
  uint32 colorIndex = 0;

  void renderLoop(GLFWwindow* window, uint32& shapesVAO);
};