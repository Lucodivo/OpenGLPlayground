#pragma once

#include <glm/glm.hpp>

#include "LearnOpenGLPlatform.h"
#include "Kernels.h"
#include "Scene.h"
#include "ObjectData.h"
#include "Shader.h"

class PlaygroundScene final : public FirstPersonScene {
public:
	PlaygroundScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth);
	void runScene();

	// override
	void frameBufferSize(uint32 width, uint32 height);

	void key_LeftMouseButton_pressed();
	void key_Up();
	void key_Down();

private:
	Shader cubeShader;
	Shader lightShader;
	Shader modelShader;
	Shader stencilShader;
	Shader frameBufferShader;

	// frame rate
	float32 deltaTime = 0.0f;	// Time between current frame and last frame
	float32 lastFrame = 0.0f; // Time of last frame

	bool flashLightOn = true;

	uint32 frameBuffer = 0;
	uint32 frameBufferTexture = 0;
	uint32 rbo = 0;

	uint32 selectedKernelIndex = 0;

	double kernelModeSwitchTimer = 0.0f;
	uint32 kernelCount = ArrayCount(kernels3x3);

	void renderLoop(GLFWwindow* window, uint32& shapesVAO, uint32& lightVAO, uint32& quadVAO);
	void initializeTextures(uint32& diffTextureId, uint32& specTextureId);
};


// ===== cube values =====
const uint32 numCubes = 9;
const glm::vec3 cubePositions[] = {
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

const float32 cubeScales[] = {
	0.6f,
	0.65f,
	0.7f,
	0.75f,
	0.8f,
	0.85f,
	0.9f,
	0.95f,
	1.0f
};
// ===== cube values =====