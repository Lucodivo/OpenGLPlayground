//
// Created by Connor on 11/21/2019.
//

#include "RayTracingSphereScene.h"
#include "../../common/FileLocations.h"
#include "../../common/ObjectData.h"
#include "../../common/Util.h"

RayTracingSphereScene::RayTracingSphereScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
        : GodModeScene(window, initScreenHeight, initScreenWidth),
          rayTracingSphereShader(UVCoordVertexShaderFileLoc, RayTracingSphereFragmentShaderFileLoc) {}

void RayTracingSphereScene::runScene()
{
  uint32 quadVAO, quadVBO, quadEBO;
  initializeFrameBufferQuadVertexAttBuffers(quadVAO, quadVBO, quadEBO);

  renderLoop(quadVAO);

  glDeleteVertexArrays(1, &quadVAO);
  glDeleteBuffers(1, &quadVBO);
  glDeleteBuffers(1, &quadEBO);
}

void RayTracingSphereScene::renderLoop(uint32 quadVAO)
{
  initializeFrameBuffer(frameBuffer, rbo, frameBufferTexture, windowWidth, windowHeight);

  // background clear color
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  rayTracingSphereShader.use();
  rayTracingSphereShader.setUniform("viewPortResolution", glm::vec2(windowWidth, windowHeight));

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

    if(rayTracingSphereShader.updateFragmentShaderIfOutdated()) {
      rayTracingSphereShader.use();
      rayTracingSphereShader.setUniform("viewPortResolution", glm::vec2(windowWidth, windowHeight));
    }

    float32 t = (float32)glfwGetTime() - startTime;
    deltaTime = t - lastFrame;
    lastFrame = t;

    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 cameraRotationMatrix = camera.GetViewMatrix(deltaTime);
    rayTracingSphereShader.setUniform("rayOrigin", camera.Position);
    rayTracingSphereShader.setUniform("elapsedTime", t);
    rayTracingSphereShader.setUniform("viewRotationMat", reverseZMat4 * cameraRotationMatrix);
    glDrawElements(GL_TRIANGLES, // drawing mode
                   6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
                   GL_UNSIGNED_INT, // type of the indices
                   0); // offset in the EBO

    glfwSwapBuffers(window); // swaps double buffers (call after all render commands are completed)
    glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
  }
}

void RayTracingSphereScene::frameBufferSize(uint32 width, uint32 height)
{
  FirstPersonScene::frameBufferSize(width, height);
  initializeFrameBuffer(frameBuffer, rbo, frameBufferTexture, width, height);
  rayTracingSphereShader.use();
  rayTracingSphereShader.setUniform("viewPortResolution", glm::vec2(width, height));
}