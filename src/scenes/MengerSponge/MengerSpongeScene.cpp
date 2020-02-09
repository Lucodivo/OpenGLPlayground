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
  initializeFrameBuffer(frameBuffer, rbo, frameBufferTexture, viewportWidth, viewportHeight);

  // background clear color
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  mengerSpongeShader.use();
  mengerSpongeShader.setUniform("viewPortResolution", glm::vec2(viewportWidth, viewportHeight));

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

    if(mengerSpongeShader.updateFragmentShaderIfOutdated()) {
      mengerSpongeShader.use();
      mengerSpongeShader.setUniform("viewPortResolution", glm::vec2(viewportWidth, viewportHeight));
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

    glfwSwapBuffers(window); // swaps double buffers (call after all render commands are completed)
    glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
  }
}

void MengerSpongeScene::frameBufferSize(uint32 width, uint32 height)
{
  FirstPersonScene::frameBufferSize(width, height);
  initializeFrameBuffer(frameBuffer, rbo, frameBufferTexture, width, height);
  mengerSpongeShader.use();
  mengerSpongeShader.setUniform("viewPortResolution", glm::vec2(width, height));
}