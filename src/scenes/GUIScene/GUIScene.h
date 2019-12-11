//
// Created by Connor on 10/24/2019.
//

#ifndef LEARNOPENGL_GUISCENE_H
#define LEARNOPENGL_GUISCENE_H

#include "../FirstPersonScene.h"

const uint32 numCubes = 3;

struct Cube {
  bool wireframe;
  glm::vec3 worldPos;
  glm::vec3 boundingBoxMin;
  glm::vec3 boundingBoxMax;
};

class GUIScene final : public FirstPersonScene {
public:
  GUIScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth);

  void runScene() override;
  void key_Tab_pressed() override;
  void key_LeftMouseButton_pressed(float32 xPos, float32 yPos) override;
  void key_RightMouseButton_pressed(float32 xPos, float32 yPos) override;
  void mouseMovement(float32 xOffset, float32 yOffset) override;

private:
  bool cursorMode = false;
  bool displayMouseClick = false;

  Shader cubeShader;

  float32 deltaTime = 0;
  float32 lastFrame = 0;

  std::string text;
  glm::mat4 projectionMat;
  glm::mat4 viewMat;

  Cube cubes[numCubes];

  void renderLoop(uint32 cubeVAO);

  void checkMouseClickCollision(float32 mouseX, float32 mouseY);

  bool checkCubeCollision(glm::vec3* worldRay, glm::vec3* rayOrigin, Cube* cube);
};


#endif //LEARNOPENGL_GUISCENE_H
