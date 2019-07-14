#pragma once

#include "LearnOpenGLPlatform.h"

void initializeCubePosTexNormAttBuffers(uint32& VAO, uint32& VBO, uint32& EBO);
void initializeCubePosNormAttBuffers(uint32& VAO, uint32& VBO, uint32& EBO);
void initializeCubePositionAttBuffers(uint32& VAO, uint32& VBO, uint32& EBO);
void initializeQuadVertexAttBuffers(uint32& VAO, uint32& VBO, uint32& EBO);
void initializeFrameBufferQuadVertexAttBuffers(uint32& VAO, uint32& VBO, uint32& EBO);
void initializeFrameBuffer(uint32& frameBuffer, uint32& rbo, uint32& frameBufferTexture, uint32 width, uint32 height);

// ===== cube values =====
#define BottomLeftTexture 0.0f, 0.0f
#define BottomRightTexture 1.0f, 0.0f
#define TopLeftTexture 0.0f, 1.0f
#define TopRightTexture 1.0f, 1.0f
const uint32 cubePosTexNormAttSizeInBytes = 8 * sizeof(float32);
const uint32 cubePosTexNormNumElements = 12; // 2 triangles per side * 6 sides per cube
const float32 cubePosTexNormAttributes[] = {
  // positions           // normals            // texture positions 
  // face #1
  -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   BottomLeftTexture,     // bottom left
  0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   BottomRightTexture,    // bottom right
  0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   TopRightTexture,       // top right
  -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   TopLeftTexture,        // top left
  // face #2			   
  -0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f,    BottomLeftTexture,     // bottom left
  0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    BottomRightTexture,    // bottom right
  0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    TopRightTexture,       // top right
  -0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,    TopLeftTexture,        // top left
  // face #3			   
  -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,  BottomRightTexture,    // bottom right
  -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,  TopRightTexture,       // top right
  -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,  TopLeftTexture,        // top left
  -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,  BottomLeftTexture,     // bottom left
  // face #4			   
  0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   BottomRightTexture,    // bottom right
  0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   TopRightTexture,       // top right
  0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   TopLeftTexture,        // top left
  0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   BottomLeftTexture,     // bottom left
  // face #5			   
  -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   TopLeftTexture,        // top left
  0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   TopRightTexture,       // top right
  0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   BottomRightTexture,    // bottom right
  -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   BottomLeftTexture,     // bottom left
  // face #6			   
  -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   TopLeftTexture,        // top left
  0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   TopRightTexture,       // top right
  0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   BottomRightTexture,    // bottom right
  -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   BottomLeftTexture      // bottom left
};

const uint32 cubePosNormAttSizeInBytes = 6 * sizeof(float32);
const float32 cubePosNormAttributes[] = {
  // positions          // normals (unit vectors orthogonal to surface) 
  // face #1
  -0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,   // bottom left
  0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,   // bottom right
  0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,   // top right
  -0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,   // top left
  // face #2
  -0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,   // bottom left
  0.5f, -0.5f,  0.5f,		0.0f,  0.0f, 1.0f,   // bottom right
  0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,   // top right
  -0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,   // top left
  // face #3
  -0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f, // bottom right
  -0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f, // top right
  -0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f, // top left
  -0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f, // bottom left
  // face #4
  0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,  // bottom right
  0.5f,  0.5f, -0.5f,		1.0f,  0.0f,  0.0f,  // top right
  0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,  // top left
  0.5f, -0.5f,  0.5f,		1.0f,  0.0f,  0.0f,  // bottom left
  // face #5
  -0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,  // top left
  0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,  // top right
  0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,  // bottom right
  -0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,  // bottom left
  // face #6
  -0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,  // top left
  0.5f,  0.5f, -0.5f,		0.0f,  1.0f,  0.0f,  // top right
  0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,  // bottom right
  -0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f   // bottom left
};

const uint32 cubePosNormIndices[]{
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
// ===== cube values =====

// ===== Skybox values =====
const uint32 cubePositionSizeInBytes = 3 * sizeof(float32);
const float32 cubePositionAttributes[] = {
  // positions      
  -1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f, 1.0f,
  -1.0f, 1.0f, -1.0f,
  -1.0f, 1.0f, 1.0f,
  1.0f, -1.0f, -1.0f,
  1.0f, -1.0f, 1.0f,
  1.0f, 1.0f, -1.0f,
  1.0f, 1.0f, 1.0f
};

const uint32 cubePositionIndices[]{
  0, 2, 1,
  1, 2, 3,
  0, 1, 4,
  1, 5, 4,
  0, 4, 2,
  4, 6, 2,
  2, 6, 3,
  6, 7, 3,
  1, 7, 5,
  1, 3, 7,
  4, 5, 7,
  4, 7, 6,
};
// ===== Skybox values =====

// ===== frame buffer quad values =====
const uint32 quadVertexAttSizeInBytes = 8 * sizeof(float32);
const float32 quadVertexAttributes[] = {
	// positions		// normal			// texCoords
	-1.0f,  1.0f, 0.0f,	0.0f,  0.0f, 1.0f,	0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f,	0.0f,  0.0f, 1.0f,	0.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,	0.0f,  0.0f, 1.0f,	1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,	0.0f,  0.0f, 1.0f,	1.0f, 1.0f
};

const uint32 quadIndices[]{
  0, 1, 2,
  0, 2, 3
};
// ===== frame buffer quad values =====

// ===== frame buffer quad values =====
const uint32 frameBufferQuadVertexAttSizeInBytes = 4 * sizeof(float32);
const float32 frameBufferQuadVertexAttributes[] = {
  // positions   // texCoords
  -1.0f,  1.0f,  0.0f, 1.0f,
  -1.0f, -1.0f,  0.0f, 0.0f,
   1.0f, -1.0f,  1.0f, 0.0f,
   1.0f,  1.0f,  1.0f, 1.0f
};
// ===== frame buffer quad values =====