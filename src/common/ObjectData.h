#pragma once

#include <glm/vec3.hpp>
#include "../LearnOpenGLPlatform.h"

struct VertexAtt {
  uint32 arrayObject;
  uint32 bufferObject;
  uint32 indexObject;
};

VertexAtt initializeCubePosNormTexVertexAttBuffers(bool invertNormals = false);
VertexAtt initializeCubePosNormVertexAttBuffers();
VertexAtt initializeCubePositionVertexAttBuffers();
void cubeAttributeIndicesBackToFront(glm::vec3 viewPosCubeSpace, uint32* cubeAttrIndices);
void cubeAttributeIndicesBackToFront(glm::vec3 viewPos, glm::mat3 cubeModel, uint32* cubeAttrIndices);
VertexAtt initializeQuadPosNormTexVertexAttBuffers();
VertexAtt initializeQuadPosNormTexTanBiVertexAttBuffers();
VertexAtt initializeFrameBufferQuadVertexAttBuffers();
void deleteVertexAtt(VertexAtt vertexAtt);
void deleteVertexAtts(uint32 count, VertexAtt* vertexAtts);

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
  -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,  BottomRightTexture,    // bottom right
  -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,  TopRightTexture,       // top right
  -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,  TopLeftTexture,        // top left
  -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,  BottomLeftTexture,     // bottom left
  // face #2
  0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   BottomRightTexture,    // bottom right
  0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   TopRightTexture,       // top right
  0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   TopLeftTexture,        // top left
  0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   BottomLeftTexture,     // bottom left
  // face #3
  -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   TopLeftTexture,        // top left
  0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   TopRightTexture,       // top right
  0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   BottomRightTexture,    // bottom right
  -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   BottomLeftTexture,     // bottom left
  // face #4
  -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   TopLeftTexture,        // top left
  0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   TopRightTexture,       // top right
  0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   BottomRightTexture,    // bottom right
  -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   BottomLeftTexture,      // bottom left
  // face #5
  -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   BottomLeftTexture,     // bottom left
  0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   BottomRightTexture,    // bottom right
  0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   TopRightTexture,       // top right
  -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   TopLeftTexture,        // top left
  // face #6
  -0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f,    BottomLeftTexture,     // bottom left
  0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    BottomRightTexture,    // bottom right
  0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    TopRightTexture,       // top right
  -0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,    TopLeftTexture,        // top left
};
const float32 cubePosTexInvertedNormAttributes[] = {
  // positions           // normals            // texture positions
  // face #1			               -
  -0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   BottomRightTexture,    // bottom right
  -0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   TopRightTexture,       // top right
  -0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   TopLeftTexture,        // top left
  -0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   BottomLeftTexture,     // bottom left
  // face #2
  0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,   BottomRightTexture,    // bottom right
  0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,   TopRightTexture,       // top right
  0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,   TopLeftTexture,        // top left
  0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,   BottomLeftTexture,     // bottom left
  // face #3
  -0.5f, -0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   TopLeftTexture,        // top left
  0.5f, -0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   TopRightTexture,       // top right
  0.5f, -0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   BottomRightTexture,    // bottom right
  -0.5f, -0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   BottomLeftTexture,     // bottom left
  // face #4
  -0.5f,  0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   TopLeftTexture,        // top left
  0.5f,  0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   TopRightTexture,       // top right
  0.5f,  0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   BottomRightTexture,    // bottom right
  -0.5f,  0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   BottomLeftTexture,      // bottom left
  // face #5
  -0.5f, -0.5f, -0.5f,   0.0f,  0.0f,  1.0f,   BottomLeftTexture,     // bottom left
  0.5f, -0.5f, -0.5f,    0.0f,  0.0f,  1.0f,   BottomRightTexture,    // bottom right
  0.5f,  0.5f, -0.5f,    0.0f,  0.0f,  1.0f,   TopRightTexture,       // top right
  -0.5f,  0.5f, -0.5f,   0.0f,  0.0f,  1.0f,   TopLeftTexture,        // top left
  // face #6
  -0.5f, -0.5f,  0.5f,   0.0f,  0.0f, -1.0f,   BottomLeftTexture,     // bottom left
  0.5f, -0.5f,  0.5f,    0.0f,  0.0f, -1.0f,   BottomRightTexture,    // bottom right
  0.5f,  0.5f,  0.5f,    0.0f,  0.0f, -1.0f,   TopRightTexture,       // top right
  -0.5f,  0.5f,  0.5f,   0.0f,  0.0f, -1.0f,   TopLeftTexture,        // top left
};

const uint32 cubePosNormAttSizeInBytes = 6 * sizeof(float32);
const float32 cubePosNormAttributes[] = {
  // positions          // normals (unit vectors orthogonal to surface)
  // face #1
  -0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f, // bottom right
  -0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f, // top right
  -0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f, // top left
  -0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f, // bottom left
  // face #2
  0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,  // bottom right
  0.5f,  0.5f, -0.5f,		1.0f,  0.0f,  0.0f,  // top right
  0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,  // top left
  0.5f, -0.5f,  0.5f,		1.0f,  0.0f,  0.0f,  // bottom left
  // face #3
  -0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,  // top left
  0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,  // top right
  0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,  // bottom right
  -0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,  // bottom left
  // face #4
  -0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,  // top left
  0.5f,  0.5f, -0.5f,		0.0f,  1.0f,  0.0f,  // top right
  0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,  // bottom right
  -0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,   // bottom left
  // face #5
  -0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,   // bottom left
  0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,   // bottom right
  0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,   // top right
  -0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,   // top left
  // face #6
  -0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,   // bottom left
  0.5f, -0.5f,  0.5f,		0.0f,  0.0f, 1.0f,   // bottom right
  0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,   // top right
  -0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,   // top left
};
const uint32 cubeAttributeIndices[]{ // 531024   // 420135
        0, 1, 2, // 0
        2, 3, 0,
        4, 6, 5, // 1
        6, 4, 7,
        8, 9, 10, // 2
        10, 11, 8,
        12, 14, 13, // 3
        14, 12, 15,
        16, 18, 17, // 4
        18, 16, 19,
        20, 21, 22, // 5
        22, 23, 20,
};

const glm::vec3 cubeFaceCenterPos[]
{
        {-0.5, 0.0, 0.0}, //-X
        {0.5, 0.0, 0.0},  //+X
        {0.0, -0.5, 0.0}, //-Y
        {0.0, 0.5, 0.0},  //+Y
        {0.0, 0.0, -0.5}, //-Z
        {0.0, 0.0, 0.5},  //+Z
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
        1.0f, 1.0f, 1.0f,
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
        0, 2, 3,
};

// ===== Quad values (vec3 pos, vec3 norm, vec2 tex) =====
const uint32 quadPosNormTexVertexAttSizeInBytes = 8 * sizeof(float32);
const float32 quadPosNormTexVertexAttributes[] = {
  // positions		    // normal		    // texCoords
  -1.0f,  1.0f, 0.0f,	0.0f,  0.0f, 1.0f,	0.0f, 1.0f,
  -1.0f, -1.0f, 0.0f,	0.0f,  0.0f, 1.0f,	0.0f, 0.0f,
   1.0f, -1.0f, 0.0f,	0.0f,  0.0f, 1.0f,	1.0f, 0.0f,
   1.0f,  1.0f, 0.0f,	0.0f,  0.0f, 1.0f,	1.0f, 1.0f,
};
// ===== Quad values (vec3 pos, vec3 norm, vec2 tex) =====

// ===== Quad values (vec2 position, vec2 tex) =====
const uint32 quadPosTexVertexAttSizeInBytes = 4 * sizeof(float32);
const float32 quadPosTexVertexAttributes[] = {
  // positions   // texCoords
  -1.0f,  1.0f,  0.0f, 1.0f,
  -1.0f, -1.0f,  0.0f, 0.0f,
   1.0f, -1.0f,  1.0f, 0.0f,
   1.0f,  1.0f,  1.0f, 1.0f,
};
// ===== Quad values (vec2 position, vec2 tex) =====


// ===== Quad values (vec3 position, vec3 normal, vec2 tex, vec3 tangent, vec3 bitangent) =====
const uint32 quadPosNormTexTanBiVertexAttSizeInBytes = 14 * sizeof(float32);
const float32 quadPosNormTexTanBiVertexAttributes[] = {
        // positions		    // normal		        // texCoords    // tangent          // bitangent
        -1.0f,  1.0f, 0.0f,	    0.0f,  0.0f, 1.0f,	    0.0f, 1.0f,     1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,	    0.0f,  0.0f, 1.0f,	    0.0f, 0.0f,     1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,	    0.0f,  0.0f, 1.0f,	    1.0f, 0.0f,     1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,	    0.0f,  0.0f, 1.0f,	    1.0f, 1.0f,     1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
};
// ===== Quad values (vec3 position, vec3 normal, vec2 tex, vec3 tangent, vec3 bitangent) =====
// QUAD VALUES