//
// Created by Connor on 11/21/2019.
//

#include "FragmentShaderPlaygroundScene.h"
#include "../../common/FileLocations.h"
#include "../../common/ObjectData.h"

FragmentShaderPlaygroundScene::FragmentShaderPlaygroundScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
        : FirstPersonScene(window, initScreenHeight, initScreenWidth),
          playgroundShader(UVCoordVertexShaderFileLoc, MandelbrotFragmentShaderFileLoc) {}

void FragmentShaderPlaygroundScene::runScene()
{
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  uint32 quadVAO, quadVBO, quadEBO;
  initializeFrameBufferQuadVertexAttBuffers(quadVAO, quadVBO, quadEBO);

  renderLoop(quadVAO);

  glDeleteVertexArrays(1, &quadVAO);
  glDeleteBuffers(1, &quadVBO);
  glDeleteBuffers(1, &quadEBO);
}

void FragmentShaderPlaygroundScene::renderLoop(uint32 quadVAO)
{
  initializeFrameBuffer(frameBuffer, rbo, frameBufferTexture, viewportWidth, viewportHeight);

  // background clear color
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  playgroundShader.use();
  playgroundShader.setUniform("viewPortResolution", glm::vec2(viewportWidth, viewportHeight));

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
    glm::mat4 cameraRotationMatrix = camera.lookAtRotationMat();
    playgroundShader.setUniform("rayOrigin", camera.Position);
    playgroundShader.setUniform("elapsedTime", t);
    playgroundShader.setUniform("zoom", zoom);
    playgroundShader.setUniform("centerOffset", centerOffset);
    glDrawElements(GL_TRIANGLES, // drawing mode
                   6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
                   GL_UNSIGNED_INT, // type of the indices
                   0); // offset in the EBO

    glfwSwapBuffers(window); // swaps double buffers (call after all render commands are completed)
    glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
  }
}

void FragmentShaderPlaygroundScene::frameBufferSize(uint32 width, uint32 height)
{
  FirstPersonScene::frameBufferSize(width, height);
  initializeFrameBuffer(frameBuffer, rbo, frameBufferTexture, width, height);
  playgroundShader.use();
  playgroundShader.setUniform("viewPortResolution", glm::vec2(width, height));
}

void FragmentShaderPlaygroundScene::mouseScroll(float32 yOffset)
{
  float zoomDelta = zoom * 0.03f;
  if(yOffset < 0) {
    zoom -= zoomDelta;
  } else if(yOffset > 0) {
    zoom += zoomDelta;
  }
}

void FragmentShaderPlaygroundScene::key_LeftMouseButton_pressed(float32 xPos, float32 yPos)
{
  mouseDown = true;
}

void FragmentShaderPlaygroundScene::key_LeftMouseButton_released(float32 xPos, float32 yPos)
{
  mouseDown = false;
}

void FragmentShaderPlaygroundScene::mouseMovement(float32 xOffset, float32 yOffset)
{
  if(mouseDown) {
    centerOffset -= glm::vec2(xOffset / zoom, yOffset / zoom);
  }
}
