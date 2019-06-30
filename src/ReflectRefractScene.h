#pragma once

#include <glm/glm.hpp>

#include "LearnOpenGLPlatform.h"
#include "Kernels.h"
#include "Scene.h"
#include "ObjectData.h"
#include "Shader.h"

class ReflectRefractScene final : public FirstPersonScene {
public:
	ReflectRefractScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth);
	void runScene();

private:
	Shader explodingReflectionShader;
  Shader reflectionShader;
	Shader explodingRefractionShader;
	Shader skyboxShader;
  Shader normalVisualizationShader;

	// frame rate
	float32 deltaTime = 0.0f;	// Time between current frame and last frame
	float32 lastFrame = 0.0f; // Time of last frame

	double reflactionModeSwitchTimer = 0.0f;

	void renderLoop(GLFWwindow* window, uint32& cubeVAO, uint32& skyboxVAO);
	void initializeTextures(uint32& skyboxTextureId);

	void key_Up();
	void key_Down();
	void button_dPadUp_pressed();
	void button_dPadDown_pressed();

	void nextModelReflaction();
	void prevModelReflaction();
};