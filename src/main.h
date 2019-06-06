#pragma once

#include "LearnOpenGLPlatform.h"

void modelTest(GLFWwindow* window, uint32 lightVAO);
void loadGLFW();
GLFWwindow * createWindow();
void initializeGLAD();
void initializeObjectBuffers(uint32 & VAO, uint32 & VBO, uint32 & EBO);
void initializeLightBuffers(uint32 & VAO, uint32 & VBO, uint32 & EBO);
void renderLoop(GLFWwindow *window, uint32 &shapesVAO, uint32 &lightVAO);
void initializeTextures(uint32& diffTextureId, uint32& specTextureId);
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
    0, 1, 2,
    2, 3, 0,
    4, 5, 6,
    6, 7, 4,
    8, 9, 10,
    10, 11, 8,
    12, 13, 14,
    14, 15, 12,
    16, 17, 18,
    18, 19, 16,
    20, 21, 22,
    22, 23, 20
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