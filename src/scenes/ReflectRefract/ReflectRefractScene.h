#pragma once

#include <glm/glm.hpp>

#include "../../LearnOpenGLPlatform.h"
#include "../../common/Kernels.h"
#include "../FirstPersonScene.h"
#include "../../common/ObjectData.h"
#include "../../Shader.h"

class ReflectRefractScene final : public FirstPersonScene
{
public:
  ReflectRefractScene(GLFWwindow* window);
  void runScene() override;
  void init(uint32 windowWidth, uint32 windowHeight);
  void deinit();
  void drawFrame();

private:
  Shader explodingReflectionShader;
  Shader exploding10InstanceReflectionShader;
  Shader reflectionShader;
  Shader reflection10InstanceShader;
  Shader explodingRefractionShader;
  Shader refractionShader;
  Shader skyboxShader;
  Shader normalVisualizationShader;
  Shader normalVisualization10InstanceShader;

  // frame rate
  float32 deltaTime = 0.0f;  // Time between current frame and last frame
  float32 lastFrame = 0.0f; // Time of last frame

  double reflactionModeSwitchTimer = 0.0f;
  double modeSwitchTimer = 0.0f;

  void renderLoop(uint32& cubeVAO, uint32& skyboxVAO);

  void key_Up() override;
  void key_Down() override;
  void key_Left() override;
  void key_Right() override;
  void button_dPadUp_pressed() override;
  void button_dPadDown_pressed() override;
  void button_dPadLeft_pressed() override;
  void button_dPadRight_pressed() override;

  void nextModelReflaction();
  void prevModelReflaction();
  void nextMode();
  void prevMode();
};