//
// Created by Connor on 10/24/2019.
//

#ifndef LEARNOPENGL_GUISCENE_H
#define LEARNOPENGL_GUISCENE_H

#include "../FirstPersonScene.h"

class GUIScene final : public FirstPersonScene {
public:
  GUIScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth);

  void runScene();
  void key_Tab_pressed();

private:
  float32 deltaTime = 0;
  float32 lastFrame = 0;

  void renderLoop();
};


#endif //LEARNOPENGL_GUISCENE_H
