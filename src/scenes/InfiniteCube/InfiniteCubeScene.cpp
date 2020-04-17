#include "InfiniteCubeScene.h"
#include "../../common/FileLocations.h"
#include "../../common/ObjectData.h"
#include "../../common/Util.h"

InfiniteCubeScene::InfiniteCubeScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
        : FirstPersonScene(window, initScreenHeight, initScreenWidth),
          cubeShader(posNormTexVertexShaderFileLoc, cropCenterSquareTexFragmentShader),
          cubeOutlineShader(posNormTexVertexShaderFileLoc, discardAlphaFragmentShaderFileLoc),
          frameBufferShader(frameBufferVertexShaderFileLoc, textureFragmentShaderFileLoc) {}

void InfiniteCubeScene::runScene()
{
  uint32 cubeVAO, cubeVBO, cubeEBO;
  initializeCubePosTexNormVertexAttBuffers(cubeVAO, cubeVBO, cubeEBO);

  uint32 quadVAO, quadVBO, quadEBO;
  initializeFrameBufferQuadVertexAttBuffers(quadVAO, quadVBO, quadEBO);

  initializeFrameBuffer(frameBuffers[0].frameBuffer, frameBuffers[0].rbo, frameBuffers[0].frameBufferTexture, windowWidth, windowHeight);
  initializeFrameBuffer(frameBuffers[1].frameBuffer, frameBuffers[1].rbo, frameBuffers[1].frameBufferTexture, windowWidth, windowHeight);

  renderLoop(window, cubeVAO, quadVAO);

  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteBuffers(1, &cubeVBO);
  glDeleteBuffers(1, &cubeEBO);

  glDeleteVertexArrays(1, &quadVAO);
  glDeleteBuffers(1, &quadVBO);
  glDeleteBuffers(1, &quadEBO);
}

void InfiniteCubeScene::frameBufferSize(uint32 width, uint32 height)
{
  FirstPersonScene::frameBufferSize(width, height);
  initializeFrameBuffer(frameBuffers[0].frameBuffer, frameBuffers[0].rbo, frameBuffers[0].frameBufferTexture, width, height);
  initializeFrameBuffer(frameBuffers[1].frameBuffer, frameBuffers[1].rbo, frameBuffers[1].frameBufferTexture, width, height);

  // start frame buffer with white background
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  // bind custom framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[0].frameBuffer);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  // bind custom framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[1].frameBuffer);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, frameBuffers[0].frameBufferTexture);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, frameBuffers[1].frameBufferTexture);
  glActiveTexture(GL_TEXTURE0);

  cubeShader.use();
  cubeShader.setUniform("texWidth", (float32)windowWidth);
  cubeShader.setUniform("texHeight", (float32)windowHeight);
}

void InfiniteCubeScene::renderLoop(GLFWwindow* window, uint32& cubeVAO, uint32& quadVAO)
{
  uint32 outlineTexture;
  load2DTexture(outlineTextureLoc, outlineTexture);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, frameBuffers[0].frameBufferTexture);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, frameBuffers[1].frameBufferTexture);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, outlineTexture);
  glActiveTexture(GL_TEXTURE0);

  // start frame buffer with white background
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  // bind custom framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[0].frameBuffer);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  // bind custom framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[1].frameBuffer);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  const glm::mat4 projectionMat = glm::perspective(glm::radians(camera.Zoom), (float32)windowWidth / (float32)windowHeight, 0.1f, 100.0f);

  const float32 cubeRotationAngle = 2.5f;

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

#if 0
  // draw in wireframe
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

  uint32 currentFrameBufferIndex = 0;
  uint32 previousFrameBufferIndex = 1;
  uint32 outlineTextureIndex = 2;

  // set constant uniforms
  cubeShader.use();
  cubeShader.setUniform("projection", projectionMat);
  cubeShader.setUniform("texWidth", (float32)windowWidth);
  cubeShader.setUniform("texHeight", (float32)windowHeight);

  cubeOutlineShader.use();
  cubeOutlineShader.setUniform("projection", projectionMat);
  cubeOutlineShader.setUniform("diffTexture", outlineTextureIndex);


  uint32 globalVSUniformBuffer;
  uint32 globalVSBufferBindIndex = 0;
  uint32 globalVSBufferViewMatOffset = sizeof(glm::mat4);
  {
    glGenBuffers(1, &globalVSUniformBuffer);

    glBindBuffer(GL_UNIFORM_BUFFER, globalVSUniformBuffer);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);

    glBindBufferRange(GL_UNIFORM_BUFFER,    // target
                      globalVSBufferBindIndex,  // index of binding point
                      globalVSUniformBuffer,  // buffer id
                      0,            // starting offset into buffer object
                      4 * 16);        // size: 4 vec3's, 16 bits alignments

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projectionMat));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    cubeShader.bindBlockIndex("globalBlockVS", globalVSBufferBindIndex);
    cubeOutlineShader.bindBlockIndex("globalBlockVS", globalVSBufferBindIndex);
  }

  while (glfwWindowShouldClose(window) == GL_FALSE)
  {
    // check for input
    processKeyboardInput(window, this);
    processXInput(this);

    float32 t = (float32)glfwGetTime();
    deltaTime = t - lastFrame;
    lastFrame = t;

#if 0
    // control when we "change frames" for the cube
    local_persist float32 elapsedTime = 0;
    elapsedTime += deltaTime;
    if (elapsedTime > 1.0f)
    {
      elapsedTime = 0;

      previousFrameBufferIndex = currentFrameBufferIndex;
      currentFrameBufferIndex = currentFrameBufferIndex == 0 ? 1 : 0;

      // set background color
      // more abrupt color changes
      colorIndex = (colorIndex + 1) % ArrayCount(colors);
      glClearColor(colors[colorIndex].x, colors[colorIndex].y, colors[colorIndex].z, 1.0f);
    }
#else
    // constant frame changes for cube
    previousFrameBufferIndex = currentFrameBufferIndex;
    currentFrameBufferIndex = currentFrameBufferIndex == 0 ? 1 : 0;

    // set background color
    // smoother color change
    float32 lightR = (sinf((t + 30.0f) / 3.0f) / 2.0f) + 0.5f;
    float32 lightG = (sinf((t + 60.0f) / 8.0f) / 2.0f) + 0.5f;
    float32 lightB = (sinf(t / 17.0f) / 2.0f) + 0.5f;
    glClearColor(lightR, lightG, lightB, 1.0f);
#endif

    // bind default frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[currentFrameBufferIndex].frameBuffer);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glm::mat4 viewMat = camera.GetViewMatrix(deltaTime);

    glBindBuffer(GL_UNIFORM_BUFFER, globalVSUniformBuffer);
    // update global view matrix uniform
    glBufferSubData(GL_UNIFORM_BUFFER, globalVSBufferViewMatOffset, sizeof(glm::mat4), glm::value_ptr(viewMat));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // draw cube
    cubeOutlineShader.use();
    glBindVertexArray(cubeVAO);

    // rotate with time
    glm::mat4 cubeModelMatrix = glm::mat4();
    cubeModelMatrix = glm::rotate(cubeModelMatrix, t * glm::radians(cubeRotationAngle), glm::vec3(1.0f, 0.3f, 0.5f));

    cubeOutlineShader.setUniform("model", cubeModelMatrix);

    glDrawElements(GL_TRIANGLES,
                   cubePosTexNormNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                   GL_UNSIGNED_INT,
                   0);

    cubeShader.use();
    glBindVertexArray(cubeVAO);
    cubeShader.setUniform("model", cubeModelMatrix);
    cubeShader.setUniform("diffTexture", previousFrameBufferIndex);
    glDrawElements(GL_TRIANGLES,
                   cubePosTexNormNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                   GL_UNSIGNED_INT,
                   0);

    // draw scene to quad
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    frameBufferShader.use();
    glBindVertexArray(quadVAO);
    frameBufferShader.setUniform("tex", currentFrameBufferIndex);
    glDrawElements(GL_TRIANGLES, // drawing mode
                   6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
                   GL_UNSIGNED_INT, // type of the indices
                   0); // offset in the EBO

    glfwSwapBuffers(window); // swaps double buffers (call after all render commands are completed)
    glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
  }
}