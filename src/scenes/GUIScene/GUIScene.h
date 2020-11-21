//
// Created by Connor on 10/24/2019.
//

#pragma once

#define GLFW_INCLUDE_NONE // ensure GLFW doesn't load OpenGL headers
#include <GLFW/glfw3.h>

#include "../FirstPersonScene.h"
#include "../../common/ObjectData.h"
#include "../../ShaderProgram.h"

const uint32 numCubes = 3;

struct Cube {
  bool wireframe = false;
  glm::vec3 worldPos;
  glm::vec3 boundingBoxMin;
  glm::vec3 boundingBoxMax;
};

class GUIScene final : public FirstPersonScene {
public:
  GUIScene(GLFWwindow* window);
  void init(Extent2D windowExtent);
  Framebuffer drawFrame();
  void deinit();
  void inputStatesUpdated();
  virtual void framebufferSizeChangeRequest(Extent2D windowExtent);
  const char* title();

private:
  GLFWwindow* window = NULL;

  bool cursorModeEnabled = false;

  ShaderProgram* cubeShader;

  VertexAtt cubeVertexAtt;

  Framebuffer drawFramebuffer;

  float32 startTime = 0;
  float32 deltaTime = 0;
  float32 lastFrame = 0;

  std::string text;
  glm::mat4 projectionMat;
  glm::mat4 viewMat;

  Cube cubes[numCubes];
  glm::mat4 cubeModelMats[3];

  const glm::vec3 cubeColor = { 0.8f, 0.0f, 0.0f };
  const glm::vec3 wireFrameColor = { 0.0f, 0.0f, 0.0f };

  void checkMouseClickCollision(float32 mouseX, float32 mouseY);
  bool checkCubeCollision(glm::vec3* worldRay, glm::vec3* rayOrigin, Cube* cube);
};
