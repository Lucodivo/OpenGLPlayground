//
// Created by Connor on 11/21/2019.
//

#include "RayTracingSphereScene.h"
#include "../../common/FileLocations.h"
#include "../../common/ObjectData.h"
#include "../../common/Util.h"

RayTracingSphereScene::RayTracingSphereScene() : FirstPersonScene()
{
  camera.Position = glm::vec3(0.0f, 0.0f, 10.0f);
}

const char* RayTracingSphereScene::title()
{
  return "Ray Tracing Sphere";
}

void RayTracingSphereScene::init(uint32 windowWidth, uint32 windowHeight)
{
  FirstPersonScene::init(windowWidth, windowHeight);
  rayTracingSphereShader = new Shader(UVCoordVertexShaderFileLoc, RayTracingSphereFragmentShaderFileLoc);
  rayTracingSphereShader->use();
  rayTracingSphereShader->setUniform("viewPortResolution", glm::vec2(windowWidth, windowHeight));
  VertexAtt quadVertexAtt = initializeFrameBufferQuadVertexAttBuffers();

  glBindVertexArray(quadVertexAtt.arrayObject);

  glDisable(GL_DEPTH_TEST);

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  lastFrame = (float32)glfwGetTime();
  startTime = lastFrame;
}

void RayTracingSphereScene::deinit()
{
  FirstPersonScene::deinit();

  rayTracingSphereShader->deleteShaderResources();
  delete rayTracingSphereShader;

  deleteVertexAtt(quadVertexAtt);
}

void RayTracingSphereScene::drawFrame()
{
  FirstPersonScene::drawFrame();

  // NOTE: uncomment for real time testing of fragment shader
//  if(rayTracingSphereShader->updateShadersWhenOutdated(FragmentShaderFlag)) {
//    rayTracingSphereShader->use();
//    rayTracingSphereShader->setUniform("viewPortResolution", glm::vec2(windowWidth, windowHeight));
//  }

  float32 t = (float32)glfwGetTime() - startTime;
  deltaTime = t - lastFrame;
  lastFrame = t;

  glClear(GL_COLOR_BUFFER_BIT);

  glm::mat4 cameraRotationMatrix = camera.UpdateViewMatrix(deltaTime, cameraMovementSpeed * 4.0f, false);
  rayTracingSphereShader->setUniform("rayOrigin", camera.Position);
  rayTracingSphereShader->setUniform("elapsedTime", t);
  rayTracingSphereShader->setUniform("viewRotationMat", reverseZ(cameraRotationMatrix));
  glDrawElements(GL_TRIANGLES, // drawing mode
                 6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
                 GL_UNSIGNED_INT, // type of the indices
                 0); // offset in the EBO
}


void RayTracingSphereScene::inputStatesUpdated() {
  FirstPersonScene::inputStatesUpdated();

  if(isActive(WindowInput_SizeChange)) {
    Extent2D windowExtent = getWindowExtent();
    rayTracingSphereShader->use();
    rayTracingSphereShader->setUniform("viewPortResolution", glm::vec2(windowExtent.x, windowExtent.y));
  }
}