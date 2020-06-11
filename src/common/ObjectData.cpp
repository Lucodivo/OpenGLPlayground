#include <glad/glad.h>

#include <iostream>

#include "ObjectData.h"


void initializeCubePosNormTexVertexAttBuffers(uint32& VAO, uint32& VBO, uint32& EBO, bool invertNormals)
{
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  const float32* attributes = invertNormals ? cubePosTexInvertedNormAttributes : cubePosTexNormAttributes;

  glGenBuffers(1, // Num objects to generate 
               &VBO);  // Out parameters to store IDs of gen objects
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind object to array buffer
  glBufferData(GL_ARRAY_BUFFER, // which buffer data is being entered in
               sizeof(cubePosTexNormAttributes), // size of data being placed in array buffer
               attributes,        // data to store in array buffer
               GL_STATIC_DRAW); // GL_STATIC_DRAW (most likely not change), GL_DYNAMIC_DRAW (likely to change), GL_STREAM_DRAW (changes every time drawn)

  // position attribute
  glVertexAttribPointer(0, // position vertex attribute (used for location = 0 of Vertex Shader) 
                        3, // size of vertex attribute (we're using vec3)
                        GL_FLOAT, // type of data being passed
                        GL_FALSE, // whether the data needs to be normalized
                        cubePosTexNormAttSizeInBytes, // stride: space between consecutive vertex attribute sets
                        (void*)0); // offset of where the data starts in the array
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

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubePosNormIndices), cubePosNormIndices, GL_STATIC_DRAW);

  // unbind VBO, VAO, & EBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  // Must unbind EBO AFTER unbinding VAO, since VAO stores all glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _) calls
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void initializeCubePosNormVertexAttBuffers(uint32& VAO, uint32& VBO, uint32& EBO)
{
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, // Num objects to generate 
               &VBO);  // Out parameters to store IDs of gen objects
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind object to array buffer
  glBufferData(GL_ARRAY_BUFFER, // which buffer data is being entered in
               sizeof(cubePosNormAttributes), // size of data being placed in array buffer
               cubePosNormAttributes,        // data to store in array buffer
               GL_STATIC_DRAW); // GL_STATIC_DRAW (most likely not change), GL_DYNAMIC_DRAW (likely to change), GL_STREAM_DRAW (changes every time drawn)

  // position attribute
  glVertexAttribPointer(0, // position vertex attribute (used for location = 0 of Vertex Shader) 
                        3, // size of vertex attribute (we're using vec3)
                        GL_FLOAT, // type of data being passed
                        GL_FALSE, // whether the data needs to be normalized
                        cubePosNormAttSizeInBytes, // stride: space between consecutive vertex attribute sets
                        (void*)0); // offset of where the data starts in the array
  glEnableVertexAttribArray(0);

  // normal attribute
  glVertexAttribPointer(1,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        cubePosNormAttSizeInBytes,
                        (void*)(3 * sizeof(float32)));
  glEnableVertexAttribArray(1);

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubePosNormIndices), cubePosNormIndices, GL_STATIC_DRAW);

  // unbind VBO, VAO, & EBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  // Must unbind EBO AFTER unbinding VAO, since VAO stores all glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _) calls
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void initializeCubePositionVertexAttBuffers(uint32& VAO, uint32& VBO, uint32& EBO)
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubePositionAttributes), cubePositionAttributes, GL_STATIC_DRAW);

  // set the vertex attributes
  // position attribute
  glVertexAttribPointer(0,
                        3, // size
                        GL_FLOAT, // type of data
                        GL_FALSE, // whether the data needs to be normalized
                        cubePositionSizeInBytes, // stride
                        (void*)0); // offset
  glEnableVertexAttribArray(0);

  // bind element buffer object to give indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubePositionIndices), cubePositionIndices, GL_STATIC_DRAW);

  // unbind VBO, VAO, & EBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  // Must unbind EBO AFTER unbinding VAO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void initializeQuadPosNormTexVertexAttBuffers(uint32& VAO, uint32& VBO, uint32& EBO)
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1,
               &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
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
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

  // unbind VBO, VAO, & EBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  // Must unbind EBO AFTER unbinding VAO, since VAO stores all glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _) calls
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void initializeQuadPosNormTexTanBiVertexAttBuffers(uint32& VAO, uint32& VBO, uint32& EBO)
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
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
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

  // unbind VBO, VAO, & EBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  // Must unbind EBO AFTER unbinding VAO, since VAO stores all glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _) calls
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void initializeFrameBufferQuadVertexAttBuffers(uint32& VAO, uint32& VBO, uint32& EBO)
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1,
               &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
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
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

  // unbind VBO, VAO, & EBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  // Must unbind EBO AFTER unbinding VAO, since VAO stores all glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _) calls
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void initializeFrameBuffer(uint32& frameBuffer, uint32& rbo, uint32& frameBufferTexture, uint32 width, uint32 height)
{
  glDeleteFramebuffers(1, &frameBuffer);
  glDeleteRenderbuffers(1, &rbo);
  glDeleteTextures(1, &frameBufferTexture);

  // creating frame buffer
  glGenFramebuffers(1, &frameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

  // creating frame buffer color texture
  glGenTextures(1, &frameBufferTexture);
  glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0); // unbind

  // attach texture w/ color to frame buffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, // frame buffer we're tageting (draw, read, or both)
                         GL_COLOR_ATTACHMENT0, // type of attachment
                         GL_TEXTURE_2D, // type of texture
                         frameBufferTexture, // texture
                         0); // mipmap level

  // creating frame buffer depth texture
  uint32 depthTexture;
  glGenTextures(1, &depthTexture);
  glBindTexture(GL_TEXTURE_2D, depthTexture);glTexImage2D(
          GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0,
          GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
  );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0); // unbind

  glFramebufferTexture2D(GL_FRAMEBUFFER,
          GL_DEPTH_STENCIL_ATTACHMENT,
          GL_TEXTURE_2D,
          depthTexture,
          0);

  // creating render buffer to be depth/stencil buffer
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glBindRenderbuffer(GL_RENDERBUFFER, 0); // unbind
  // attach render buffer w/ depth & stencil to frame buffer
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, // frame buffer target
                            GL_DEPTH_STENCIL_ATTACHMENT, // attachment point of frame buffer
                            GL_RENDERBUFFER, // render buffer target
                            rbo);  // render buffer

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

