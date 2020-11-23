//
// Created by Connor on 11/12/2019.
//

#include "InfiniteCapsulesScene.h"
#include "../../common/FileLocations.h"
#include "../../common/Util.h"
#include "../../common/Input.h"

InfiniteCapsulesScene::InfiniteCapsulesScene(): FirstPersonScene()
{
  camera.Position = glm::vec3(0.0f, 1.0f, 0.0f);
}

const char* InfiniteCapsulesScene::title()
{
  return "Infinite Capsules";
}

void InfiniteCapsulesScene::init(Extent2D windowExtent)
{
  FirstPersonScene::init(windowExtent);

  rayMarchingShader = new ShaderProgram(UVCoordVertexShaderFileLoc, InfiniteCapsulesFragmentShaderFileLoc);

  quadVertexAtt = initializeFramebufferQuadVertexAttBuffers();

  drawFramebuffer = initializeFramebuffer(windowExtent, FramebufferCreate_NoDepthStencil);

  rayMarchingShader->use();
  rayMarchingShader->setUniform("viewPortResolution", glm::vec2(windowExtent.width, windowExtent.height));
  rayMarchingShader->setUniform("lightColor", glm::vec3(0.5f, 0.5f, 0.5f));
  rayMarchingShader->setUniform("lightPos", lightPosition);

  lastFrame = getTime();
  startTime = lastFrame;
}

void InfiniteCapsulesScene::deinit() {
  FirstPersonScene::deinit();

  rayMarchingShader->deleteShaderResources();
  delete rayMarchingShader;

  deleteVertexAtt(quadVertexAtt);

  deleteFramebuffer(&drawFramebuffer);
}

Framebuffer InfiniteCapsulesScene::drawFrame() {
  glDisable(GL_DEPTH_TEST);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glViewport(0, 0, windowExtent.width, windowExtent.height);
  glBindVertexArray(quadVertexAtt.arrayObject);
  glBindFramebuffer(GL_FRAMEBUFFER, drawFramebuffer.id);
  glClear(GL_COLOR_BUFFER_BIT);

  float32 t = getTime() - startTime;
  deltaTime = t - lastFrame;
  lastFrame = t;

  glm::mat4 cameraRotationMatrix = camera.UpdateViewMatrix(deltaTime, cameraMovementSpeed * 4.0f, false);
  rayMarchingShader->use();
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

   return drawFramebuffer;
}

void InfiniteCapsulesScene::inputStatesUpdated() {
  FirstPersonScene::inputStatesUpdated();

  if(hotPress(MouseInput_Left)) {
    lightAlive = true;
    lightDistanceTraveled = 0.0f;
    lightMoveDir = camera.Front;
    lightPosition = camera.Position + lightMoveDir;
  }
}

void InfiniteCapsulesScene::framebufferSizeChangeRequest(Extent2D windowExtent)
{
  Scene::framebufferSizeChangeRequest(windowExtent);

  deleteFramebuffer(&drawFramebuffer);
  drawFramebuffer = initializeFramebuffer(windowExtent, FramebufferCreate_NoDepthStencil);

  rayMarchingShader->use();
  rayMarchingShader->setUniform("viewPortResolution", glm::vec2(windowExtent.width, windowExtent.height));
}
