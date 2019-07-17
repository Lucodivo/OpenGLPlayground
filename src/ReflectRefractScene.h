#pragma once

#include <glm/glm.hpp>

#include "LearnOpenGLPlatform.h"
#include "Kernels.h"
#include "FirstPersonScene.h"
#include "ObjectData.h"
#include "Shader.h"

class ReflectRefractScene final : public FirstPersonScene
{
public:
  ReflectRefractScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth);
  void runScene();

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
  float32 deltaTime = 0.0f;	// Time between current frame and last frame
  float32 lastFrame = 0.0f; // Time of last frame

  double reflactionModeSwitchTimer = 0.0f;
  double modeSwitchTimer = 0.0f;

  void renderLoop(GLFWwindow* window, uint32& cubeVAO, uint32& skyboxVAO);

  void key_Up();
  void key_Down();
  void key_Left();
  void key_Right();
  void button_dPadUp_pressed();
  void button_dPadDown_pressed();
  void button_dPadLeft_pressed();
  void button_dPadRight_pressed();

  void nextModelReflaction();
  void prevModelReflaction();
  void nextMode();
  void prevMode();
};