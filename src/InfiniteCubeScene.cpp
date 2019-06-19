#include "InfiniteCubeScene.h"
#include "Shader.h"
#include "FileLocations.h"
#include "ObjectData.h"

InfiniteCubeScene::InfiniteCubeScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
	: FirstPersonScene(window, initScreenHeight, initScreenWidth) {}

void InfiniteCubeScene::runScene() {
	uint32 cubeVAO, cubeVBO, cubeEBO;
	initializeCubeBuffers(cubeVAO, cubeVBO, cubeEBO);

	uint32 quadVAO, quadVBO, quadEBO;
	initializeQuadBuffers(quadVAO, quadVBO, quadEBO);

	initializeFrameBuffer(frameBuffer, rbo, frameBufferTexture, viewportWidth, viewportHeight);

	renderLoop(window, cubeVAO, quadVAO);
}

void InfiniteCubeScene::frameBufferSize(uint32 width, uint32 height) {
	FirstPersonScene::frameBufferSize(width, height);
	initializeFrameBuffer(frameBuffer, rbo, frameBufferTexture, width, height);
}

void InfiniteCubeScene::renderLoop(GLFWwindow* window, uint32& cubeVAO, uint32& quadVAO) {
	Shader cubeShader = Shader(cubeVertexShaderFileLoc, cubeFragmentShaderFileLoc);
	Shader frameBufferShader = Shader(frameBufferVertexShaderFileLoc, kernelFrameBufferFragmentShaderFileLoc);
}