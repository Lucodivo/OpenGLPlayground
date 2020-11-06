//
// Created by Connor on 11/21/2019.
//

#include "RayTracingSphereScene.h"
#include "../../common/FileLocations.h"
#include "../../common/ObjectData.h"
#include "../../common/Util.h"

RayTracingSphereScene::RayTracingSphereScene(GLFWwindow* window)
        : GodModeScene(window),
          rayTracingSphereShader(UVCoordVertexShaderFileLoc, RayTracingSphereFragmentShaderFileLoc) {}

void RayTracingSphereScene::runScene()
{
  VertexAtt quadVertexAtt = initializeFrameBufferQuadVertexAttBuffers();

  renderLoop(quadVertexAtt.arrayObject);

  deleteVertexAtt(quadVertexAtt);
}

void RayTracingSphereScene::renderLoop(uint32 quadVAO)
{
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

void RayTracingSphereScene::framebufferSizeChange(uint32 width, uint32 height)
{
  FirstPersonScene::framebufferSizeChange(width, height);
  rayTracingSphereShader.use();
  rayTracingSphereShader.setUniform("viewPortResolution", glm::vec2(width, height));
}