//
// Created by Connor on 11/12/2019.
//

#ifndef LEARNOPENGL_INFINITECAPSULESSCENE_H
#define LEARNOPENGL_INFINITECAPSULESSCENE_H

#include "../FirstPersonScene.h"
#include "../GodModeScene.h"
#include "../../common/ObjectData.h"

class InfiniteCapsulesScene final : public GodModeScene {
public:
  InfiniteCapsulesScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth);

  // FrameBufferSizeConsumer override
  void frameBufferSize(uint32 width, uint32 height);
  void key_LeftMouseButton_pressed(float32 xPos, float32 yPos);
  void runScene() override;

private:

  Shader* rayMarchingShader = NULL;

  float32 startTime = 0;
  float32 deltaTime = 0;
  float32 lastFrame = 0;

  VertexAtt quadVertexAtt;

  void init();
  void drawFrame();
  void deinit();

  glm::vec3 lightPosition;
  bool lightAlive = false;
  glm::vec3 lightMoveDir;
  float lightDistanceTraveled;
};

#endif //LEARNOPENGL_INFINITECAPSULESSCENE_H
