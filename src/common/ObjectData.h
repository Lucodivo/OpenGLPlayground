#pragma once

#include "../LearnOpenGLPlatform.h"

void initializeCubePosNormTexVertexAttBuffers(uint32& VAO, uint32& VBO, uint32& EBO, bool invertNormals = false);
void initializeCubePosNormVertexAttBuffers(uint32& VAO, uint32& VBO, uint32& EBO);
void initializeCubePositionVertexAttBuffers(uint32& VAO, uint32& VBO, uint32& EBO);
void initializeQuadPosNormTexVertexAttBuffers(uint32& VAO, uint32& VBO, uint32& EBO);
void initializeQuadPosNormTexTanBiVertexAttBuffers(uint32& VAO, uint32& VBO, uint32& EBO);
void initializeFrameBufferQuadVertexAttBuffers(uint32& VAO, uint32& VBO, uint32& EBO);
void initializeFrameBuffer(uint32& frameBuffer, uint32& rbo, uint32& frameBufferTexture, uint32 width, uint32 height);

// ===== cube values =====
#define BottomLeftTexture 0.0f, 0.0f
#define BottomRightTexture 1.0f, 0.0f
#define TopLeftTexture 0.0f, 1.0f
#define TopRightTexture 1.0f, 1.0f
const uint32 cubePosTexNormAttSizeInBytes = 8 * sizeof(float32);
const uint32 cubePosNormTexNumElements = 12; // 2 triangles per side * 6 sides per cube
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
const float32 cubePosTexInvertedNormAttributes[] = {
  // positions           // normals            // texture positions 
  // face #1
  -0.5f, -0.5f, -0.5f,   0.0f,  0.0f,  1.0f,   BottomLeftTexture,     // bottom left
  0.5f, -0.5f, -0.5f,    0.0f,  0.0f,  1.0f,   BottomRightTexture,    // bottom right
  0.5f,  0.5f, -0.5f,    0.0f,  0.0f,  1.0f,   TopRightTexture,       // top right
  -0.5f,  0.5f, -0.5f,   0.0f,  0.0f,  1.0f,   TopLeftTexture,        // top left
  // face #2			               
  -0.5f, -0.5f,  0.5f,   0.0f,  0.0f, -1.0f,   BottomLeftTexture,     // bottom left
  0.5f, -0.5f,  0.5f,    0.0f,  0.0f, -1.0f,   BottomRightTexture,    // bottom right
  0.5f,  0.5f,  0.5f,    0.0f,  0.0f, -1.0f,   TopRightTexture,       // top right
  -0.5f,  0.5f,  0.5f,   0.0f,  0.0f, -1.0f,   TopLeftTexture,        // top left
  // face #3			               -     
  -0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   BottomRightTexture,    // bottom right
  -0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   TopRightTexture,       // top right
  -0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   TopLeftTexture,        // top left
  -0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   BottomLeftTexture,     // bottom left
  // face #4			                     
  0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,   BottomRightTexture,    // bottom right
  0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,   TopRightTexture,       // top right
  0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,   TopLeftTexture,        // top left
  0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,   BottomLeftTexture,     // bottom left
  // face #5			                     
  -0.5f, -0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   TopLeftTexture,        // top left
  0.5f, -0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   TopRightTexture,       // top right
  0.5f, -0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   BottomRightTexture,    // bottom right
  -0.5f, -0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   BottomLeftTexture,     // bottom left
  // face #6			                     
  -0.5f,  0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   TopLeftTexture,        // top left
  0.5f,  0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   TopRightTexture,       // top right
  0.5f,  0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   BottomRightTexture,    // bottom right
  -0.5f,  0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   BottomLeftTexture      // bottom left
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

// QUAD VALUES
const uint32 quadIndices[]{
        0, 1, 2,
        0, 2, 3
};

// ===== Quad values (vec3 pos, vec3 norm, vec2 tex) =====
const uint32 quadPosNormTexVertexAttSizeInBytes = 8 * sizeof(float32);
const float32 quadPosNormTexVertexAttributes[] = {
  // positions		    // normal		    // texCoords
  -1.0f,  1.0f, 0.0f,	0.0f,  0.0f, 1.0f,	0.0f, 1.0f,
  -1.0f, -1.0f, 0.0f,	0.0f,  0.0f, 1.0f,	0.0f, 0.0f,
   1.0f, -1.0f, 0.0f,	0.0f,  0.0f, 1.0f,	1.0f, 0.0f,
   1.0f,  1.0f, 0.0f,	0.0f,  0.0f, 1.0f,	1.0f, 1.0f
};
// ===== Quad values (vec3 pos, vec3 norm, vec2 tex) =====

// ===== Quad values (vec2 position, vec2 tex) =====
const uint32 quadPosTexVertexAttSizeInBytes = 4 * sizeof(float32);
const float32 quadPosTexVertexAttributes[] = {
  // positions   // texCoords
  -1.0f,  1.0f,  0.0f, 1.0f,
  -1.0f, -1.0f,  0.0f, 0.0f,
   1.0f, -1.0f,  1.0f, 0.0f,
   1.0f,  1.0f,  1.0f, 1.0f
};
// ===== Quad values (vec2 position, vec2 tex) =====


// ===== Quad values (vec3 position, vec3 normal, vec2 tex, vec3 tangent, vec3 bitangent) =====
const uint32 quadPosNormTexTanBiVertexAttSizeInBytes = 14 * sizeof(float32);
const float32 quadPosNormTexTanBiVertexAttributes[] = {
        // positions		    // normal		        // texCoords    // tangent          // bitangent
        -1.0f,  1.0f, 0.0f,	    0.0f,  0.0f, 1.0f,	    0.0f, 1.0f,     1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,	    0.0f,  0.0f, 1.0f,	    0.0f, 0.0f,     1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,	    0.0f,  0.0f, 1.0f,	    1.0f, 0.0f,     1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,	    0.0f,  0.0f, 1.0f,	    1.0f, 1.0f,     1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f
};
// ===== Quad values (vec3 position, vec3 normal, vec2 tex, vec3 tangent, vec3 bitangent) =====
// QUAD VALUES