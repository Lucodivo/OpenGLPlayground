#pragma once

#include "LearnOpenGLPlatform.h"

void modelTest(GLFWwindow* window, uint32 lightVAO);
int main();
void loadGLFW();
GLFWwindow * createWindow();
void initializeGLAD();
void initializeObjectBuffers(uint32 & VAO, uint32 & VBO, uint32 & EBO);
void initializeLightBuffers(uint32 & VAO, uint32 & VBO, uint32 & EBO);
void initializeQuadBuffers(uint32 & quadVAO, uint32 & quadVBO, uint32 & shapesEBO);
void renderLoop(GLFWwindow *window, uint32 &shapesVAO, uint32 &lightVAO, uint32 & quadVAO);
void initializeTextures(uint32& diffTextureId, uint32& specTextureId);
void initializeFrameBuffer(uint32 width, uint32 height);
void loadTexture(const char * imgLocation, uint32 &textureOffset);
void processInput(GLFWwindow * window);
void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow * window, int width, int height);

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
float quadVertexAttributes[] = {
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

// ===== double triangle values =====
const uint32 triangleVertexAttSize = 8;
const uint32 triangleNumElements = 2;
const float32 triangleVertices[] = {
    // First triangle
    // positions            // colors           // texture coords
    -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
    0.5f, -0.5f, 0.0f,      0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // bottom right
    0.0f, 0.5f, 0.0f,       0.0f, 1.0f, 0.0f,   0.5, 1.0f,  // top (for first triangle) / bottom (for second triangle)
    // Second triangle
    -0.25f, 0.75f, 0.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // top left
    0.25f, 0.75f, 0.0f,     1.0f, 0.0f, 0.0f,   1.0f, 0.0f  // top right
};
const uint32 indices[]{
    0, 1, 2,    // first triangle
    2, 3, 4     // second triangle
};
// ===== double triangle values =====

// ===== Image Kernels =====
const float32 kernels[7][9] = {
	// normal kernel
	{0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f},

	// blur kernel
	{1.0f / 16, 2.0f / 16, 1.0f / 16,
	2.0f / 16, 4.0f / 16, 2.0f / 16,
	1.0f / 16, 2.0f / 16, 1.0f / 16},

	// sharpen kernel
	{-1.0f, -1.0f, -1.0f,
	-1.0f,  9.0f, -1.0f,
	-1.0f, -1.0f, -1.0f},

	// outline 1 kernel
	{1.0f, 1.0f, 1.0f,
	1.0f, -8.0f, 1.0f,
	1.0f, 1.0f, 1.0f},

	// outline 2 kernel
	{-1.0f, -1.0f, -1.0f,
	-1.0f, 8.0f, -1.0f,
	-1.0f, -1.0f, -1.0f},

	// sketch kernel
	{37.0f, -12.0f, 9.0f,
	-13.0f, 22.0f, -18.0f,
	22.0f, -50.0f, 3.0f},

	// emboss kernel
	{-2.0f, -1.0f, 0.0f,
	-1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 2.0f},

};

const float32* normalKernel = kernels[0];

const float32* sharpenKernel = kernels[1];

const float32* blurKernel = kernels[2];

const float32* edgeKernel = kernels[3];
// ===== Image Kernels =====