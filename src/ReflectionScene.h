#pragma once

#include <glm/glm.hpp>

#include "LearnOpenGLPlatform.h"
#include "Kernels.h"
#include "Scene.h"
#include "ObjectData.h"
#include "Shader.h"

class ReflectionScene final : public FirstPersonScene {
public:
	ReflectionScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth);
	void runScene();

private:
	Shader cubeShader;
	Shader modelShader;
	Shader skyboxShader;

	// frame rate
	float32 deltaTime = 0.0f;	// Time between current frame and last frame
	float32 lastFrame = 0.0f; // Time of last frame

	void renderLoop(GLFWwindow* window, uint32& cubeVAO, uint32& skyboxVAO);
	void initializeTextures(uint32& skyboxTextureId);
};