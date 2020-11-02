#include "InfiniteCubeScene.h"
#include "../../common/FileLocations.h"
#include "../../common/ObjectData.h"
#include "../../common/Util.h"

const uint32 framebufferTextureIndex = 0;
const uint32 outlineTextureIndex = 1;

InfiniteCubeScene::InfiniteCubeScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
        : FirstPersonScene(window, initScreenHeight, initScreenWidth),
          cubeShader(posNormTexVertexShaderFileLoc, discardAlphaFragmentShaderFileLoc),
          cubeOutlineShader(posNormTexVertexShaderFileLoc, discardAlphaFragmentShaderFileLoc) {}

void InfiniteCubeScene::runScene()
{
  uint32 cubeVAO, cubeVBO, cubeEBO;
  initializeCubePosNormTexVertexAttBuffers(cubeVAO, cubeVBO, cubeEBO);

  uint32 quadVAO, quadVBO, quadEBO;
  initializeFrameBufferQuadVertexAttBuffers(quadVAO, quadVBO, quadEBO);

  renderLoop(window, cubeVAO);

  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteBuffers(1, &cubeVBO);
  glDeleteBuffers(1, &cubeEBO);
}

void InfiniteCubeScene::frameBufferSize(uint32 width, uint32 height)
{
  FirstPersonScene::frameBufferSize(width, height);
  framebufferDimen = width > height ? height : width;

  deleteFrameBuffer(framebuffer);
  framebuffer = initializeFrameBuffer(framebufferDimen, framebufferDimen);

  glActiveTexture(GL_TEXTURE0 + framebufferTextureIndex);
  glBindTexture(GL_TEXTURE_2D, framebuffer.colorAttachment);
}

void InfiniteCubeScene::renderLoop(GLFWwindow* window, uint32& cubeVAO)
{
  uint32 outlineTexture;
  load2DTexture(outlineTextureLoc, outlineTexture);

  framebufferDimen = windowWidth > windowHeight ? windowHeight : windowWidth;
  framebuffer = initializeFrameBuffer(framebufferDimen, framebufferDimen);

  glActiveTexture(GL_TEXTURE0 + framebufferTextureIndex);
  glBindTexture(GL_TEXTURE_2D, framebuffer.colorAttachment);
  glActiveTexture(GL_TEXTURE0 + outlineTextureIndex);
  glBindTexture(GL_TEXTURE_2D, outlineTexture);

  // set texture uniforms
  cubeShader.use();
  cubeShader.setUniform("diffTexture", framebufferTextureIndex);

  cubeOutlineShader.use();
  cubeOutlineShader.setUniform("diffTexture", outlineTextureIndex);

  const glm::mat4 projectionMat = glm::perspective(glm::radians(camera.Zoom), (float32)windowWidth / (float32)windowHeight, 0.1f, 100.0f);
  const float32 cubeRotationAngle = 2.5f;

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  uint32 globalVSUniformBufferID;
  uint32 globalVSBufferBindIndex = 0;
  uint32 globalVSBufferViewMatOffset = sizeof(glm::mat4);
  {
    glGenBuffers(1, &globalVSUniformBufferID);

    glBindBuffer(GL_UNIFORM_BUFFER, globalVSUniformBufferID);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);

    glBindBufferRange(GL_UNIFORM_BUFFER,    // target
                      globalVSBufferBindIndex,  // index of binding point
                      globalVSUniformBufferID,  // buffer id
                      0,            // starting offset into buffer object
                      4 * 16);        // size: 4 vec3's, 16 bits alignments

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projectionMat));

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

      // set background color
      // more abrupt color changes
      colorIndex = (colorIndex + 1) % ArrayCount(colors);
      glClearColor(colors[colorIndex].x, colors[colorIndex].y, colors[colorIndex].z, 1.0f);
    }
#else
    // set background color
    // smoother color change
    float32 lightR = (sinf((t + 30.0f) / 3.0f) / 2.0f) + 0.5f;
    float32 lightG = (sinf((t + 60.0f) / 8.0f) / 2.0f) + 0.5f;
    float32 lightB = (sinf(t / 17.0f) / 2.0f) + 0.5f;
    glClearColor(lightR, lightG, lightB, 1.0f);
#endif

    // bind default frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glm::mat4 viewMat = camera.GetViewMatrix(deltaTime);

    glBindBuffer(GL_UNIFORM_BUFFER, globalVSUniformBufferID);
    // update global view matrix uniform
    glBufferSubData(GL_UNIFORM_BUFFER, globalVSBufferViewMatOffset, sizeof(glm::mat4), glm::value_ptr(viewMat));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // draw cube outline
    cubeOutlineShader.use();
    glBindVertexArray(cubeVAO);

    // rotate with time
    glm::mat4 cubeModelMatrix = glm::mat4();
    cubeModelMatrix = glm::rotate(cubeModelMatrix, t * glm::radians(cubeRotationAngle), glm::vec3(1.0f, 0.3f, 0.5f));

    cubeOutlineShader.setUniform("model", cubeModelMatrix);

    glDrawElements(GL_TRIANGLES,
                   cubePosNormTexNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                   GL_UNSIGNED_INT,
                   0);

    // draw cube
    cubeShader.use();
    cubeShader.setUniform("model", cubeModelMatrix);
    glDrawElements(GL_TRIANGLES,
                   cubePosNormTexNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                   GL_UNSIGNED_INT,
                   0);

    // bind our frame buffer
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer.id);
    uint32 xOffset = (windowWidth - framebufferDimen) / 2;
    uint32 yOffset = (windowHeight - framebufferDimen) / 2;
    glBlitFramebuffer(xOffset, yOffset, framebufferDimen + xOffset, framebufferDimen + yOffset, 0, 0, framebufferDimen, framebufferDimen, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glfwSwapBuffers(window); // swaps double buffers (call after all render commands are completed)
    glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
  }
}