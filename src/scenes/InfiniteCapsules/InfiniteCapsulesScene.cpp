//
// Created by Connor on 11/12/2019.
//

#include "InfiniteCapsulesScene.h"
#include "../../common/FileLocations.h"
#include "../../common/Util.h"

InfiniteCapsulesScene::InfiniteCapsulesScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
        : GodModeScene(window, initScreenHeight, initScreenWidth) {}

void InfiniteCapsulesScene::runScene()
{
  // TODO: Remove
//  init();
//  while (glfwWindowShouldClose(window) == GL_FALSE)
//  {
//    drawFrame();
//    glfwSwapBuffers(window); // swaps double buffers (call after all render commands are completed)
//    glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
//  }
//  deinit();
}

void InfiniteCapsulesScene::init() {
  GodModeScene::init();

  rayMarchingShader = new Shader(UVCoordVertexShaderFileLoc, InfiniteCapsulesFragmentShaderFileLoc);

  quadVertexAtt = initializeFrameBufferQuadVertexAttBuffers();

  rayMarchingShader->use();
  rayMarchingShader->setUniform("viewPortResolution", glm::vec2(windowWidth, windowHeight));
  rayMarchingShader->setUniform("lightColor", glm::vec3(0.5f, 0.5f, 0.5f));
  rayMarchingShader->setUniform("lightPos", glm::vec3(0.0f, 0.0f, 0.0f));

  camera.Position = glm::vec3(0.0f, 1.0f, 0.0f);

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glBindVertexArray(quadVertexAtt.arrayObject);

  lastFrame = (float32)glfwGetTime();
  startTime = lastFrame;
}

void InfiniteCapsulesScene::drawFrame() {
  GodModeScene::drawFrame();

  float32 t = (float32)glfwGetTime() - startTime;
  deltaTime = t - lastFrame;
  lastFrame = t;

  glClear(GL_COLOR_BUFFER_BIT);

  glm::mat4 cameraRotationMatrix = camera.GetViewMatrix(deltaTime);
  rayMarchingShader->setUniform("rayOrigin", camera.Position);
  rayMarchingShader->setUniform("elapsedTime", t);
  rayMarchingShader->setUniform("viewRotationMat", reverseZMat4 * cameraRotationMatrix);
  if(lightAlive) {
    rayMarchingShader->setUniform("lightPos", lightPosition);
    glm::vec3 lightDelta = lightMoveDir * deltaTime * 25.0f;
    lightPosition += lightDelta;
    lightDistanceTraveled += glm::length(lightDelta);
    if(lightDistanceTraveled > 100.0) lightAlive = false;
  }
  glDrawElements(GL_TRIANGLES, // drawing mode
                 6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
                 GL_UNSIGNED_INT, // type of the indices
                 0); // offset in the EBO
}

void InfiniteCapsulesScene::deinit() {
  GodModeScene::deinit();

  delete rayMarchingShader;

  deleteVertexAtt(quadVertexAtt);
}

void InfiniteCapsulesScene::frameBufferSize(uint32 width, uint32 height)
{
  FirstPersonScene::frameBufferSize(width, height);
  rayMarchingShader->use();
  rayMarchingShader->setUniform("viewPortResolution", glm::vec2(width, height));
}

void InfiniteCapsulesScene::key_LeftMouseButton_pressed(float32 xPos, float32 yPos) {
  lightAlive = true;
  lightDistanceTraveled = 0.0f;
  lightMoveDir = camera.Front;
  lightPosition = camera.Position + lightMoveDir;
}