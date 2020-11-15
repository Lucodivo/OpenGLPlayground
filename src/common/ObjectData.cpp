#include <glad/glad.h>

#include <iostream>
#include <glm/detail/type_mat4x4.hpp>

#include "ObjectData.h"

void deleteVertexAtt(VertexAtt vertexAtt) {
  glDeleteBuffers(1, &vertexAtt.indexObject);
  glDeleteVertexArrays(1, &vertexAtt.arrayObject);
  glDeleteBuffers(1, &vertexAtt.bufferObject);
}

void deleteVertexAtts(uint32 count, VertexAtt* vertexAtts)
{
  uint32* deleteBufferObjects = new uint32[count * 3];
  uint32* deleteIndexObjects = deleteBufferObjects + count;
  uint32* deleteVertexArrays = deleteIndexObjects + count;
  for(uint32 i = 0; i < count; i++) {
    deleteBufferObjects[i] = vertexAtts[i].bufferObject;
    deleteIndexObjects[i] = vertexAtts[i].indexObject;
    deleteVertexArrays[i] = vertexAtts[i].arrayObject;
  }

  glDeleteBuffers(count * 2, deleteBufferObjects);
  glDeleteVertexArrays(count, deleteVertexArrays);

  delete[] deleteBufferObjects;
}

VertexAtt initializeCubePosNormTexVertexAttBuffers(bool invertNormals) {
  VertexAtt vertexAtt;
  glGenVertexArrays(1, &vertexAtt.arrayObject);
  glGenBuffers(1, &vertexAtt.bufferObject);
  glGenBuffers(1, &vertexAtt.indexObject);

  glBindVertexArray(vertexAtt.arrayObject);

  const float32* attributes = invertNormals ? cubePosTexInvertedNormAttributes : cubePosTexNormAttributes;

  glBindBuffer(GL_ARRAY_BUFFER, vertexAtt.bufferObject);
  glBufferData(GL_ARRAY_BUFFER, // which buffer data is being entered in
               sizeof(cubePosTexNormAttributes), // size
               attributes,        // data
               GL_STATIC_DRAW); // GL_STATIC_DRAW (most likely not change), GL_DYNAMIC_DRAW (likely to change), GL_STREAM_DRAW (changes every time drawn)

  // position attribute
  glVertexAttribPointer(0, // position vertex attribute (used for location = 0 of Vertex Shader)
                        3, // size (we're using vec3)
                        GL_FLOAT, // type of data
                        GL_FALSE, // whether the data needs to be normalized
                        cubePosTexNormAttSizeInBytes, // vertex attribute stride
                        (void*)0); // offset
  glEnableVertexAttribArray(0);

  // normal attribute
  glVertexAttribPointer(1,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        cubePosTexNormAttSizeInBytes,
                        (void*)(3 * sizeof(float32)));
  glEnableVertexAttribArray(1);

  // texture coords attribute
  glVertexAttribPointer(2,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        cubePosTexNormAttSizeInBytes,
                        (void*)(6 * sizeof(float32)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexAtt.indexObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeAttributeIndices), cubeAttributeIndices, GL_STATIC_DRAW);

  // unbind VBO, VAO, & EBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  // Must unbind EBO AFTER unbinding VAO, since VAO stores all glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _) calls
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  return vertexAtt;
}

VertexAtt initializeCubePosNormVertexAttBuffers() {
  VertexAtt vertexAtt;
  glGenVertexArrays(1, &vertexAtt.arrayObject);
  glGenBuffers(1, &vertexAtt.bufferObject);
  glGenBuffers(1, &vertexAtt.indexObject);

  glBindVertexArray(vertexAtt.arrayObject);

  glBindBuffer(GL_ARRAY_BUFFER, vertexAtt.bufferObject); // bind object to array buffer
  glBufferData(GL_ARRAY_BUFFER, // which buffer data is being entered in
               sizeof(cubePosNormAttributes), // size
               cubePosNormAttributes,        // dat
               GL_STATIC_DRAW); // GL_STATIC_DRAW (most likely not change), GL_DYNAMIC_DRAW (likely to change), GL_STREAM_DRAW (changes every time drawn)

  // position attribute
  glVertexAttribPointer(0, // position vertex attribute (used for location = 0 of Vertex Shader)
                        3, // size (we're using vec3)
                        GL_FLOAT, // type of data
                        GL_FALSE, // whether the data needs to be normalized
                        cubePosNormAttSizeInBytes, // vertex attribute stride
                        (void*)0); // offset
  glEnableVertexAttribArray(0);

  // normal attribute
  glVertexAttribPointer(1,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        cubePosNormAttSizeInBytes,
                        (void*)(3 * sizeof(float32)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexAtt.indexObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeAttributeIndices), cubeAttributeIndices, GL_STATIC_DRAW);

  // unbind VBO, VAO, & EBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  // Must unbind EBO AFTER unbinding VAO, since VAO stores all glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _) calls
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  return vertexAtt;
}

VertexAtt initializeCubePositionVertexAttBuffers() {
  VertexAtt vertexAtt;
  glGenVertexArrays(1, &vertexAtt.arrayObject); // vertex array object
  glGenBuffers(1, &vertexAtt.bufferObject); // vertex buffer object backing the VAO
  glGenBuffers(1, &vertexAtt.indexObject);

  glBindVertexArray(vertexAtt.arrayObject);
  glBindBuffer(GL_ARRAY_BUFFER, vertexAtt.bufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubePositionAttributes), cubePositionAttributes, GL_STATIC_DRAW);

  // set the vertex attributes
  // position attribute
  glVertexAttribPointer(0, // index
                        3, // size
                        GL_FLOAT, // type of data
                        GL_FALSE, // whether the data needs to be normalized
                        cubePositionSizeInBytes, // stride
                        (void*)0); // offset
  glEnableVertexAttribArray(0);

  // bind element buffer object to give indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexAtt.indexObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubePositionIndices), cubePositionIndices, GL_STATIC_DRAW);

  // unbind VBO, VAO, & EBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  // Must unbind EBO AFTER unbinding VAO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  return vertexAtt;
}

VertexAtt initializeQuadPosNormTexVertexAttBuffers() {
  VertexAtt vertexAtt;
  glGenVertexArrays(1, &vertexAtt.arrayObject);
  glGenBuffers(1, &vertexAtt.bufferObject);
  glGenBuffers(1, &vertexAtt.indexObject);

  glBindVertexArray(vertexAtt.arrayObject);

  glBindBuffer(GL_ARRAY_BUFFER, vertexAtt.bufferObject);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(quadPosNormTexVertexAttributes),
               quadPosNormTexVertexAttributes,
               GL_STATIC_DRAW);

  // set the vertex attributes (position, normal, and texture)
  // position attribute
  glVertexAttribPointer(0,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        quadPosNormTexVertexAttSizeInBytes,
                        (void*)0);
  glEnableVertexAttribArray(0);

  // normal attribute
  glVertexAttribPointer(1,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        quadPosNormTexVertexAttSizeInBytes,
                        (void*)(3 * sizeof(float32)));
  glEnableVertexAttribArray(1);

  // texture attribute
  glVertexAttribPointer(2,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        quadPosNormTexVertexAttSizeInBytes,
                        (void*)(6 * sizeof(float32)));
  glEnableVertexAttribArray(2);

  // bind element buffer object to give indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexAtt.indexObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

  // unbind VBO, VAO, & EBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  // Must unbind EBO AFTER unbinding VAO, since VAO stores all glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _) calls
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  return vertexAtt;
}

/*
NOTE: Alternative without having to sort sides of an individual cube is to cull front faces to draw only the back faces
NOTE: And then follow it up with a second draw call not culling the back faces
Arguments:
 - viewPos: This is the views position in the cube's model space. This may require
    transforming the view position with the inverse matrix of the cube's model matrix
 - cubeAttributeIndices: This function fills an indices array equal to ArrayCount(cubeAttributeIndices)
    And the caller must supply us with an array of exactly that size
 */
void cubeAttributeIndicesBackToFront(glm::vec3 viewPosCubeSpace, uint32* cubeAttrIndices)
{
  const uint32 numSides = 6;
  const uint32 indicesPerSide = 6;

  struct FaceDistance
  {
    uint32 faceIndex;
    float32 distance;
  } distances[] = { // each index represents a specific face
          {0, glm::distance(viewPosCubeSpace, cubeFaceCenterPos[0])}, // -X
          {1, glm::distance(viewPosCubeSpace, cubeFaceCenterPos[1])}, // +X
          {2, glm::distance(viewPosCubeSpace, cubeFaceCenterPos[2])}, // -Y
          {3, glm::distance(viewPosCubeSpace, cubeFaceCenterPos[3])}, // +Y
          {4, glm::distance(viewPosCubeSpace, cubeFaceCenterPos[4])}, // -Z
          {5, glm::distance(viewPosCubeSpace, cubeFaceCenterPos[5])}  // +Z
  };

  // sorting network optimized for 6 items
#define SWAP(x,y) if (distances[y].distance < distances[x].distance) { FaceDistance tmp = distances[x]; distances[x] = distances[y]; distances[y] = tmp; }
  SWAP(1, 2);
  SWAP(0, 2);
  SWAP(0, 1);
  SWAP(4, 5);
  SWAP(3, 5);
  SWAP(3, 4);
  SWAP(0, 3);
  SWAP(1, 4);
  SWAP(2, 5);
  SWAP(2, 4);
  SWAP(1, 3);
  SWAP(2, 3);
#undef SWAP

  for(uint32 i = 0; i < numSides; ++i)
  {
    uint32 cubeAttributeStartingIndex = distances[numSides - 1 - i].faceIndex * indicesPerSide;
    for(int j = 0; j < indicesPerSide; ++j) {
      cubeAttrIndices[(i * indicesPerSide) + j] = cubeAttributeIndices[cubeAttributeStartingIndex + j];
    }
  }
}

void cubeAttributeIndicesBackToFront(glm::vec3 viewPos, glm::mat3 cubeModel, uint32* cubeAttrIndices)
{
  // NOTE: if you are using the overloaded function that takes in the view position already in the cube model space]
  // NOTE: Remember that matrix multiplication with a vector is not commutative. If things look right but feel
  // NOTE: slightly off, you could just be multiplying the inverse on the wrong side.
  glm::vec3 viewPositionInCubeModelSpace = glm::inverse(cubeModel) * viewPos;
  cubeAttributeIndicesBackToFront(viewPositionInCubeModelSpace, cubeAttrIndices);
}

VertexAtt initializeQuadPosNormTexTanBiVertexAttBuffers() {
  VertexAtt vertexAtt;
  glGenVertexArrays(1, &vertexAtt.arrayObject);
  glGenBuffers(1, &vertexAtt.bufferObject);
  glGenBuffers(1, &vertexAtt.indexObject);

  glBindVertexArray(vertexAtt.arrayObject);

  glBindBuffer(GL_ARRAY_BUFFER, vertexAtt.bufferObject);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(quadPosNormTexTanBiVertexAttributes),
               quadPosNormTexTanBiVertexAttributes,
               GL_STATIC_DRAW);

  // set the vertex attributes (position, normal, texture, tangent, and bitangent)
  // position attribute
  glVertexAttribPointer(0,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        quadPosNormTexTanBiVertexAttSizeInBytes,
                        (void*)0);
  glEnableVertexAttribArray(0);

  // normal attribute
  glVertexAttribPointer(1,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        quadPosNormTexTanBiVertexAttSizeInBytes,
                        (void*)(3 * sizeof(float32)));
  glEnableVertexAttribArray(1);

  // texture attribute
  glVertexAttribPointer(2,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        quadPosNormTexTanBiVertexAttSizeInBytes,
                        (void*)(6 * sizeof(float32)));
  glEnableVertexAttribArray(2);

  // tangent attribute
  glVertexAttribPointer(3,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        quadPosNormTexTanBiVertexAttSizeInBytes,
                        (void*)(8 * sizeof(float32)));
  glEnableVertexAttribArray(3);

  // bitangent attribute
  glVertexAttribPointer(4,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        quadPosNormTexTanBiVertexAttSizeInBytes,
                        (void*)(11 * sizeof(float32)));
  glEnableVertexAttribArray(4);

  // bind element buffer object to give indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexAtt.indexObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

  // unbind VBO, VAO, & EBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  // Must unbind EBO AFTER unbinding VAO, since VAO stores all glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _) calls
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  return vertexAtt;
}

VertexAtt initializeFrameBufferQuadVertexAttBuffers() {
  VertexAtt vertexAtt;
  glGenVertexArrays(1, &vertexAtt.arrayObject);
  glGenBuffers(1, &vertexAtt.bufferObject);
  glGenBuffers(1, &vertexAtt.indexObject);

  glBindVertexArray(vertexAtt.arrayObject);

  glBindBuffer(GL_ARRAY_BUFFER, vertexAtt.bufferObject);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(quadPosTexVertexAttributes),
               quadPosTexVertexAttributes,
               GL_STATIC_DRAW);

  // set the vertex attributes (position and texture)
  // position attribute
  glVertexAttribPointer(0,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        quadPosTexVertexAttSizeInBytes,
                        (void*)0);
  glEnableVertexAttribArray(0);

  // texture attribute
  glVertexAttribPointer(1,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        quadPosTexVertexAttSizeInBytes,
                        (void*)(2 * sizeof(float32)));
  glEnableVertexAttribArray(1);

  // bind element buffer object to give indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexAtt.indexObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

  // unbind VBO, VAO, & EBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  // Must unbind EBO AFTER unbinding VAO, since VAO stores all glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _) calls
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  return vertexAtt;
}