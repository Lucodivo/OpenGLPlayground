#pragma once

#include "GodModeScene.h"

class FloorScene : public GodModeScene
{
public:
	FloorScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth);
	void runScene();

private:

	Shader floorShader; 
	Shader lightShader;

	float32 deltaTime = 0.0f;	// Time between current frame and last frame
	float32 lastFrame = 0.0f; // Time of last frame

	void renderLoop(uint32 quadVAO, uint32 lightVAO);
};