#pragma once

#include "LearnOpenGLPlatform.h"
#include <glm/glm.hpp>

void runPlaygroundScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth);
void initializeObjectBuffers(uint32& VAO, uint32& VBO, uint32& EBO);
void initializeLightBuffers(uint32& VAO, uint32& VBO, uint32& EBO);
void initializeQuadBuffers(uint32& quadVAO, uint32& quadVBO, uint32& shapesEBO);
void renderLoop(GLFWwindow* window, uint32& shapesVAO, uint32& lightVAO, uint32& quadVAO);
void initializeTextures(uint32& diffTextureId, uint32& specTextureId);
void initializeFrameBuffer(uint32 width, uint32 height);
void loadTexture(const char* imgLocation, uint32& textureOffset);

// input
void key_LeftShift_pressed();
void key_LeftShift_released();
void key_W();
void key_S();
void key_A();
void key_D();
void key_Space();
void key_LeftMouseButton_pressed();
void key_LeftMouseButton_released();
void key_Up();
void key_Down();
void mouseMove(float32 xOffset, float32 yOffset);
void mouseScroll(float32 yOffset);

// ===== cube values =====
#define BottomLeftTexture 0.0f, 0.0f
#define BottomRightTexture 1.0f, 0.0f
#define TopLeftTexture 0.0f, 1.0f
#define TopRightTexture 1.0f, 1.0f
const uint32 cubeVertexAttSizeInBytes = 8 * sizeof(float32);
const uint32 cubeNumElements = 12;
const uint32 numCubes = 9;
const float cubeVertexAttributes[] = {
	// positions          // texture positions  // normals (unit vectors orthogonal to surface) 
	// face #1
	-0.5f, -0.5f, -0.5f,  BottomLeftTexture,     0.0f,  0.0f, -1.0f, // bottom left
	0.5f, -0.5f, -0.5f,   BottomRightTexture,    0.0f,  0.0f, -1.0f, // bottom right
	0.5f,  0.5f, -0.5f,   TopRightTexture,       0.0f,  0.0f, -1.0f, // top right
	-0.5f,  0.5f, -0.5f,  TopLeftTexture,        0.0f,  0.0f, -1.0f, // top left
	// face #2
	-0.5f, -0.5f,  0.5f,  BottomLeftTexture,     0.0f,  0.0f, 1.0f, // bottom left
	0.5f, -0.5f,  0.5f,   BottomRightTexture,    0.0f,  0.0f, 1.0f, // bottom right
	0.5f,  0.5f,  0.5f,   TopRightTexture,       0.0f,  0.0f, 1.0f, // top right
	-0.5f,  0.5f,  0.5f,  TopLeftTexture,        0.0f,  0.0f, 1.0f, // top left
	// face #3
	-0.5f,  0.5f,  0.5f,  BottomRightTexture,    -1.0f,  0.0f,  0.0f, // bottom right
	-0.5f,  0.5f, -0.5f,  TopRightTexture,       -1.0f,  0.0f,  0.0f, // top right
	-0.5f, -0.5f, -0.5f,  TopLeftTexture,        -1.0f,  0.0f,  0.0f, // top left
	-0.5f, -0.5f,  0.5f,  BottomLeftTexture,     -1.0f,  0.0f,  0.0f, // bottom left
	// face #4
	0.5f,  0.5f,  0.5f,   BottomRightTexture,    1.0f,  0.0f,  0.0f, // bottom right
	0.5f,  0.5f, -0.5f,   TopRightTexture,       1.0f,  0.0f,  0.0f, // top right
	0.5f, -0.5f, -0.5f,   TopLeftTexture,        1.0f,  0.0f,  0.0f, // top left
	0.5f, -0.5f,  0.5f,   BottomLeftTexture,     1.0f,  0.0f,  0.0f, // bottom left
	// face #5
	-0.5f, -0.5f, -0.5f,  TopLeftTexture,        0.0f, -1.0f,  0.0f, // top left
	0.5f, -0.5f, -0.5f,   TopRightTexture,       0.0f, -1.0f,  0.0f, // top right
	0.5f, -0.5f,  0.5f,   BottomRightTexture,    0.0f, -1.0f,  0.0f, // bottom right
	-0.5f, -0.5f,  0.5f,  BottomLeftTexture,     0.0f, -1.0f,  0.0f, // bottom left
	// face #6
	-0.5f,  0.5f, -0.5f,  TopLeftTexture,        0.0f,  1.0f,  0.0f, // top left
	0.5f,  0.5f, -0.5f,   TopRightTexture,       0.0f,  1.0f,  0.0f, // top right
	0.5f,  0.5f,  0.5f,   BottomRightTexture,    0.0f,  1.0f,  0.0f, // bottom right
	-0.5f,  0.5f,  0.5f,  BottomLeftTexture,     0.0f,  1.0f,  0.0f  // bottom left
};

const uint32 cubeIndices[]{
	0, 2, 1,
	2, 0, 3,
	4, 5, 6,
	6, 7, 4,
	8, 9, 10,
	10, 11, 8,
	12, 14, 13,
	14, 12, 15,
	16, 17, 18,
	18, 19, 16,
	20, 22, 21,
	22, 20, 23
};

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

// ===== frame buffer quad values =====
const uint32 quadVertexAttSizeInBytes = 4 * sizeof(float32);
const float quadVertexAttributes[] = {
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
};

const uint32 quadIndices[]{
	0, 1, 2,
	0, 2, 3
};
// ===== frame buffer quad values =====