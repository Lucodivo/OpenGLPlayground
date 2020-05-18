//
// Created by Connor on 11/21/2019.
//

#include "MengerSpongeScene.h"
#include "../../common/FileLocations.h"
#include "../../common/ObjectData.h"
#include "../../common/Util.h"

MengerSpongeScene::MengerSpongeScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
        : GodModeScene(window, initScreenHeight, initScreenWidth),
          mengerSpongeShader(UVCoordVertexShaderFileLoc, MengerSpongeFragmentShaderFileLoc),
          pixel2DShader(pixel2DVertexShaderFileLoc, textureFragmentShaderFileLoc),
          cubeShader(posNormalVertexShaderFileLoc, singleColorFragmentShaderFileLoc) {}

void MengerSpongeScene::runScene()
{
  uint32 quadVAO, quadVBO, quadEBO;
  initializeFrameBufferQuadVertexAttBuffers(quadVAO, quadVBO, quadEBO);

  // TODO: delete
  uint32 cubeVAO, cubeVBO, cubeEBO;
  initializeCubePosTexNormVertexAttBuffers(cubeVAO, cubeVBO, cubeEBO);

  renderLoop(quadVAO, cubeVAO);

  glDeleteVertexArrays(1, &quadVAO);
  glDeleteBuffers(1, &quadVBO);
  glDeleteBuffers(1, &quadEBO);
}

void MengerSpongeScene::renderLoop(uint32 quadVAO, uint32 cubeVAO)
{
  resolution largestResolution = screenResolutions[ArrayCount(screenResolutions) - 1];
  initializeFrameBuffer(frameBuffer, rbo, frameBufferTexture, largestResolution.width, largestResolution.height);


  const float cubeScale = 1.0f;
  const glm::vec3 cubePos = glm::vec3(0.0, 0.0, 0.0);
  const glm::vec3 cubeColor = glm::vec3(0.5, 0.0, 0.0);
  glm::mat4 cubeModel;
  cubeModel = glm::scale(cubeModel, glm::vec3(cubeScale));
  cubeModel = glm::translate(cubeModel, cubePos);

  // background clear color
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  load2DTexture("C:\\developer\\repos\\Assets\\Sprites\\plant-002.png", textureId, true);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureId);

  pixel2DShader.use();
  pixel2DShader.setUniform("windowDimens", glm::vec2(currentResolution.width, currentResolution.height));
  // TODO: get width/height of image from load2DTexture()
  pixel2DShader.setUniform("lowerLeftOffset", glm::vec2((currentResolution.width / 2) - 16.0, (currentResolution.height / 2) - 16.0));
  pixel2DShader.setUniform("spriteDimens", glm::vec2(32.0, 32.0));

  mengerSpongeShader.use();
  mengerSpongeShader.setUniform("viewPortResolution", glm::vec2(currentResolution.width, currentResolution.height));
  mengerSpongeShader.setUniform("rayOrigin", camera.Position);

  cubeShader.use();
  cubeShader.setUniform("model", cubeModel);
  cubeShader.setUniform("color", glm::vec3(0.5, 0.0, 0.0));

  glBindVertexArray(quadVAO);

  camera.Position = glm::vec3(0.0f, 1.0f, 10.0f);

  lastFrame = (float32)glfwGetTime();
  float32 startTime = lastFrame;
  glEnable(GL_DEPTH_TEST);

  const glm::mat4 projectionMat = glm::perspective(glm::radians(camera.Zoom), (float32)currentResolution.width / (float32)currentResolution.height, 0.1f, 200.0f);

  // NOTE: render/game loop
  while (glfwWindowShouldClose(window) == GL_FALSE)
  {
    // check for input
    processKeyboardInput(window, this);
    processXInput(this);


    float32 t = (float32)glfwGetTime() - startTime;
    deltaTime = t - lastFrame;
    lastFrame = t;

    // Auto run forward
//    glm::vec3 deltaCameraPos = camera.Front;
//    deltaCameraPos *= 0.07;
//    camera.Position += deltaCameraPos;
    glm::mat4 cameraMat = camera.GetViewMatrix(deltaTime);


    // bind our frame buffer
    glViewport(0, 0, currentResolution.width, currentResolution.height);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    mengerSpongeShader.use();
    glBindVertexArray(quadVAO);

    if(mengerSpongeShader.updateFragmentShaderIfOutdated()) {
      mengerSpongeShader.setUniform("viewPortResolution", glm::vec2(currentResolution.width, currentResolution.height));
    }

    mengerSpongeShader.setUniform("rayOrigin", camera.Position);
    mengerSpongeShader.setUniform("elapsedTime", t);
    mengerSpongeShader.setUniform("viewRotationMat", reverseZMat4 * cameraMat);
    mengerSpongeShader.setUniform("view", cameraMat);
    mengerSpongeShader.setUniform("projection", projectionMat);
    glDrawElements(GL_TRIANGLES, // drawing mode
                   6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
                   GL_UNSIGNED_INT, // type of the indices
                   0 /* offset in the EBO */);

//    static uint32 numSnapshots = 0;
//    if(numSnapshots < 1) {
//      snapshot(currentResolution.width, currentResolution.height, "C:\\Users\\Connor\\Desktop\\tmp\\snapshot.bmp", frameBuffer);
//      ++numSnapshots;
//    }

    cubeShader.use();
    cubeShader.setUniform("view", cameraMat);
    cubeShader.setUniform("projection", projectionMat);

    // TODO: Fix so perspective created by ray marching and by perspective matrix match up
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, // drawing mode
                   cubePosTexNormNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                   GL_UNSIGNED_INT, // type of the indices
                   0); // offset in the EBO
    glBindVertexArray(0);

//    pixel2DShader.use();
//    glDrawElements(GL_TRIANGLES, // drawing mode
//                   6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
//                   GL_UNSIGNED_INT, // type of the indices
//                   0); // offset in the EBO

    glViewport(0, 0, windowWidth, windowHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // bind our frame buffer
    glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);
    glBlitFramebuffer(0, 0, currentResolution.width, currentResolution.height, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

    glfwSwapBuffers(window); // swaps double buffers (call after all render commands are completed)
    glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
  }
}

void MengerSpongeScene::frameBufferSize(uint32 width, uint32 height)
{
  FirstPersonScene::frameBufferSize(width, height);
  mengerSpongeShader.use();
}

void MengerSpongeScene::key_E_pressed() {
  if(++currentResolutionIndex >= ArrayCount(screenResolutions)) {
    currentResolutionIndex = 0;
  }

  currentResolution = screenResolutions[currentResolutionIndex];

  mengerSpongeShader.use();
  mengerSpongeShader.setUniform("viewPortResolution", glm::vec2(currentResolution.width, currentResolution.height));

  pixel2DShader.use();
  pixel2DShader.setUniform("windowDimens", glm::vec2(currentResolution.width, currentResolution.height));
  pixel2DShader.setUniform("lowerLeftOffset", glm::vec2((currentResolution.width / 2) - 16.0, (currentResolution.height / 2) - 16.0));
}

void MengerSpongeScene::key_Q_pressed() {
  if(currentResolutionIndex == 0) {
    currentResolutionIndex = ArrayCount(screenResolutions) - 1;
  } else {
    --currentResolutionIndex;
  }

  currentResolution = screenResolutions[currentResolutionIndex];

  mengerSpongeShader.use();
  mengerSpongeShader.setUniform("viewPortResolution", glm::vec2(currentResolution.width, currentResolution.height));

  pixel2DShader.use();
  pixel2DShader.setUniform("windowDimens", glm::vec2(currentResolution.width, currentResolution.height));
  pixel2DShader.setUniform("lowerLeftOffset", glm::vec2((currentResolution.width / 2) - 16.0, (currentResolution.height / 2) - 16.0));
}