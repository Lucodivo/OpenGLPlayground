//
// Created by Connor on 11/12/2019.
//

#include "RayMarchingScene.h"
#include "../../common/FileLocations.h"
#include "../../common/ObjectData.h"

RayMarchingScene::RayMarchingScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
        : FirstPersonScene(window, initScreenHeight, initScreenWidth),
          rayMarchingShader(rayMarchingVertexShaderFileLoc, rayMarchingFragmentShaderFileLoc) {}

void RayMarchingScene::runScene()
{
  uint32 quadVAO, quadVBO, quadEBO;
  initializeFrameBufferQuadVertexAttBuffers(quadVAO, quadVBO, quadEBO);

  renderLoop(quadVAO);

  glDeleteVertexArrays(1, &quadVAO);
  glDeleteBuffers(1, &quadVBO);
  glDeleteBuffers(1, &quadEBO);
}

void RayMarchingScene::renderLoop(uint32 quadVAO)
{
  initializeFrameBuffer(frameBuffer, rbo, frameBufferTexture, viewportWidth, viewportHeight);

  // background clear color
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  rayMarchingShader.use();
  rayMarchingShader.setUniform("viewPortResolution", glm::vec2(viewportWidth, viewportHeight));

  glBindVertexArray(quadVAO);

  camera.Position = glm::vec3(0.0f, 1.0f, 0.0f);

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

    glClear(GL_COLOR_BUFFER_BIT);

    camera.changePositioning(deltaTime);
    rayMarchingShader.setUniform("rayOrigin", camera.Position);
    rayMarchingShader.setUniform("elapsedTime", t);
    glDrawElements(GL_TRIANGLES, // drawing mode
                   6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
                   GL_UNSIGNED_INT, // type of the indices
                   0); // offset in the EBO

    glfwSwapBuffers(window); // swaps double buffers (call after all render commands are completed)
    glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
  }
}

void RayMarchingScene::frameBufferSize(uint32 width, uint32 height)
{
  FirstPersonScene::frameBufferSize(width, height);
  initializeFrameBuffer(frameBuffer, rbo, frameBufferTexture, width, height);
  rayMarchingShader.use();
  rayMarchingShader.setUniform("viewPortResolution", glm::vec2(width, height));
}
