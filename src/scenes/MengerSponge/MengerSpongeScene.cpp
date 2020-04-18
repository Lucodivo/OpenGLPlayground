//
// Created by Connor on 11/21/2019.
//

#include "MengerSpongeScene.h"
#include "../../common/FileLocations.h"
#include "../../common/ObjectData.h"

MengerSpongeScene::MengerSpongeScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
        : GodModeScene(window, initScreenHeight, initScreenWidth),
          mengerSpongeShader(UVCoordVertexShaderFileLoc, MengerSpongeFragmentShaderFileLoc) {}

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
  initializeFrameBuffer(frameBuffer, rbo, frameBufferTexture, currentResolution.width, currentResolution.height);

  // background clear color
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  mengerSpongeShader.use();
  mengerSpongeShader.setUniform("viewPortResolution", glm::vec2(currentResolution.width, currentResolution.height));

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

    glm::vec3 deltaCameraPos = camera.Front;
    deltaCameraPos *= 0.1;
    camera.Position += deltaCameraPos;

    // bind our frame buffer
    glViewport(0, 0, currentResolution.width, currentResolution.height);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    if(mengerSpongeShader.updateFragmentShaderIfOutdated()) {
      mengerSpongeShader.use();
      mengerSpongeShader.setUniform("viewPortResolution", glm::vec2(currentResolution.width, currentResolution.height));
    }

    float32 t = (float32)glfwGetTime() - startTime;
    deltaTime = t - lastFrame;
    lastFrame = t;

    glClear(GL_COLOR_BUFFER_BIT);

    camera.changePositioning(deltaTime);
    glm::mat4 cameraRotationMatrix = camera.lookAtRotationMat();
    mengerSpongeShader.setUniform("rayOrigin", camera.Position);
    mengerSpongeShader.setUniform("elapsedTime", t);
    mengerSpongeShader.setUniform("viewRotationMat", cameraRotationMatrix);
    glDrawElements(GL_TRIANGLES, // drawing mode
                   6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
                   GL_UNSIGNED_INT, // type of the indices
                   0); // offset in the EBO

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
  initializeFrameBuffer(frameBuffer, rbo, frameBufferTexture, currentResolution.width, currentResolution.height);

  mengerSpongeShader.use();
  mengerSpongeShader.setUniform("viewPortResolution", glm::vec2(currentResolution.width, currentResolution.height));
}

void MengerSpongeScene::key_Q_pressed() {
  if(currentResolutionIndex == 0) {
    currentResolutionIndex = ArrayCount(screenResolutions) - 1;
  } else {
    --currentResolutionIndex;
  }

  currentResolution = screenResolutions[currentResolutionIndex];
  initializeFrameBuffer(frameBuffer, rbo, frameBufferTexture, currentResolution.width, currentResolution.height);

  mengerSpongeShader.use();
  mengerSpongeShader.setUniform("viewPortResolution", glm::vec2(currentResolution.width, currentResolution.height));
}