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
          pixel2DShader(pixel2DVertexShaderFileLoc, textureFragmentShaderFileLoc) {}

void MengerSpongeScene::runScene()
{
  uint32 quadVAO, quadVBO, quadEBO;
  initializeFrameBufferQuadVertexAttBuffers(quadVAO, quadVBO, quadEBO);

  renderLoop(quadVAO);

  glDeleteVertexArrays(1, &quadVAO);
  glDeleteBuffers(1, &quadVBO);
  glDeleteBuffers(1, &quadEBO);
}

void MengerSpongeScene::renderLoop(uint32 quadVAO)
{
  resolution largestResolution = screenResolutions[ArrayCount(screenResolutions) - 1];
  initializeFrameBuffer(frameBuffer, rbo, frameBufferTexture, largestResolution.width, largestResolution.height);

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

  glBindVertexArray(quadVAO);

  camera.Position = glm::vec3(0.0f, 1.0f, 10.0f);

  lastFrame = (float32)glfwGetTime();
  float32 startTime = lastFrame;

  // NOTE: render/game loop
  while (glfwWindowShouldClose(window) == GL_FALSE)
  {
    // check for input
    processKeyboardInput(window, this);
    processXInput(this);

    float32 t = (float32)glfwGetTime() - startTime;
    deltaTime = t - lastFrame;
    lastFrame = t;

    // Autoscroll
    glm::vec3 deltaCameraPos = camera.Front;
    deltaCameraPos *= 0.07;
    camera.Position += deltaCameraPos;
    camera.changePositioning(deltaTime);
    glm::mat4 cameraRotationMatrix = camera.lookAtRotationMat();

    // bind our frame buffer
    glViewport(0, 0, currentResolution.width, currentResolution.height);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glClear(GL_COLOR_BUFFER_BIT);

    mengerSpongeShader.use();

    if(mengerSpongeShader.updateFragmentShaderIfOutdated()) {
      mengerSpongeShader.setUniform("viewPortResolution", glm::vec2(currentResolution.width, currentResolution.height));
    }

    mengerSpongeShader.setUniform("rayOrigin", camera.Position);
    mengerSpongeShader.setUniform("elapsedTime", t);
    mengerSpongeShader.setUniform("viewRotationMat", cameraRotationMatrix);
    glDrawElements(GL_TRIANGLES, // drawing mode
                   6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
                   GL_UNSIGNED_INT, // type of the indices
                   0 /* offset in the EBO */);

    static uint32 numSnapshots = 0;
    if(numSnapshots < 1) {
      snapshot(currentResolution.width, currentResolution.height, "C:\\Users\\Connor\\Desktop\\tmp\\snapshot.bmp", frameBuffer);
      ++numSnapshots;
    }

//    pixel2DShader.use();
//    glDrawElements(GL_TRIANGLES, // drawing mode
//                   6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
//                   GL_UNSIGNED_INT, // type of the indices
//                   0); // offset in the EBO

    glViewport(0, 0, windowWidth, windowHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // bind our frame buffer
    glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);
    glBlitFramebuffer(0, 0, currentResolution.width, currentResolution.height, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

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