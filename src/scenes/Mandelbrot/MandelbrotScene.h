//
// Created by Connor on 11/21/2019.
//

#pragma once

#include "../FirstPersonScene.h"
#include "../../common/ObjectData.h"

#define ZOOM_SPEED_NORMAL 1.0f
#define ZOOM_SPEED_FAST 3.0f
#define MOUSE_ACTION_TIME_SECONDS 0.15f

class MandelbrotScene final : public Scene {
public:
  MandelbrotScene(GLFWwindow* window);
  void init(Extent2D windowExtent);
  Framebuffer drawFrame();
  void deinit();
  void inputStatesUpdated();
  const char* title();

private:

  Extent2D oldWindowExtent = { 0, 0 };

  Framebuffer drawFramebuffer;

  GLFWwindow* window = NULL;

  ShaderProgram* mandelbrotShader = NULL;

  float zoomSpeed = ZOOM_SPEED_NORMAL;

  float32 startTime = 0;
  float32 deltaTime = 0;
  float32 lastFrame = 0;

  float32 zoom = 0.25f;
  glm::vec2 centerOffset = glm::vec2(0.0f, 0.0f);
  bool mouseDown = false;

  VertexAtt quadVertexAtt = {};

  glm::vec3 colorFavors[3] = { glm::vec3(1.0f, 0.33f, 0.66f),
                              glm::vec3(0.165f, 0.33f, 1.0f),
                              glm::vec3(0.5f, 1.0f, 0.5f) };
  int32 currentColorFavorIndex = 0;
  float32 mouseDownTime = 0.0f;
};
