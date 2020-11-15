//
// Created by Connor on 11/12/2019.
//

#include "InfiniteCapsulesScene.h"
#include "../../common/FileLocations.h"
#include "../../common/Util.h"

InfiniteCapsulesScene::InfiniteCapsulesScene(): FirstPersonScene()
{
  camera.Position = glm::vec3(0.0f, 1.0f, 0.0f);
}

const char* InfiniteCapsulesScene::title()
{
  return "Infinite Capsules";
}

void InfiniteCapsulesScene::init(uint32 windowWidth, uint32 windowHeight)
{
  FirstPersonScene::init(windowWidth, windowHeight);

  rayMarchingShader = new Shader(UVCoordVertexShaderFileLoc, InfiniteCapsulesFragmentShaderFileLoc);

  quadVertexAtt = initializeFrameBufferQuadVertexAttBuffers();

  rayMarchingShader->use();
  rayMarchingShader->setUniform("viewPortResolution", glm::vec2(windowWidth, windowHeight));
  rayMarchingShader->setUniform("lightColor", glm::vec3(0.5f, 0.5f, 0.5f));
  rayMarchingShader->setUniform("lightPos", lightPosition);

  glDisable(GL_DEPTH_TEST);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  glBindVertexArray(quadVertexAtt.arrayObject);

  lastFrame = (float32)glfwGetTime();
  startTime = lastFrame;
}

void InfiniteCapsulesScene::deinit() {
  FirstPersonScene::deinit();

  rayMarchingShader->deleteShaderResources();
  delete rayMarchingShader;

  deleteVertexAtt(quadVertexAtt);
}

void InfiniteCapsulesScene::drawFrame() {
  FirstPersonScene::drawFrame();

  float32 t = (float32)glfwGetTime() - startTime;
  deltaTime = t - lastFrame;
  lastFrame = t;

  glClear(GL_COLOR_BUFFER_BIT);

  glm::mat4 cameraRotationMatrix = camera.UpdateViewMatrix(deltaTime, cameraMovementSpeed * 4.0f, false);
  rayMarchingShader->setUniform("rayOrigin", camera.Position);
  rayMarchingShader->setUniform("elapsedTime", t);
  rayMarchingShader->setUniform("viewRotationMat", reverseZ(cameraRotationMatrix));
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

void InfiniteCapsulesScene::inputStatesUpdated() {
  FirstPersonScene::inputStatesUpdated();

  if(hotPress(MouseInput_Left)) {
    lightAlive = true;
    lightDistanceTraveled = 0.0f;
    lightMoveDir = camera.Front;
    lightPosition = camera.Position + lightMoveDir;
  }

  if(isActive(WindowInput_SizeChange)) {
    Extent2D extent2D = getWindowExtent();
    rayMarchingShader->use();
    rayMarchingShader->setUniform("viewPortResolution", glm::vec2(extent2D.x, extent2D.y));
  }
}