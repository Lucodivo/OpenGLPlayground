#pragma once

#include <glm/glm.hpp>

#include "LearnOpenGLPlatform.h"
#include "Kernels.h"
#include "Scene.h"
#include "ObjectData.h"

class InfiniteCubeScene final : public FirstPersonScene {
public:
	InfiniteCubeScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth);
	void runScene();

	// override
	void frameBufferSize(uint32 width, uint32 height);

private:
	// frame rate
	float32 deltaTime = 0.0f;	// Time between current frame and last frame
	float32 lastFrame = 0.0f; // Time of last frame

	uint32 frameBuffer = 0;
	uint32 frameBufferTexture = 0;
	uint32 rbo = 0;

	void renderLoop(GLFWwindow* window, uint32& shapesVAO, uint32& quadVAO);
};