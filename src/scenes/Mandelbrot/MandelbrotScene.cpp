//
// Created by Connor on 11/21/2019.
//

#include "MandelbrotScene.h"
#include "../../common/FileLocations.h"
#include "../../common/ObjectData.h"

MandelbrotScene::MandelbrotScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
        : FirstPersonScene(window, initScreenHeight, initScreenWidth),
          mandelbrotShader(UVCoordVertexShaderFileLoc, MandelbrotFragmentShaderFileLoc) {}

void MandelbrotScene::runScene()
{
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  uint32 quadVAO, quadVBO, quadEBO;
  initializeFrameBufferQuadVertexAttBuffers(quadVAO, quadVBO, quadEBO);

  renderLoop(quadVAO);

  glDeleteVertexArrays(1, &quadVAO);
  glDeleteBuffers(1, &quadVBO);
  glDeleteBuffers(1, &quadEBO);
}

void MandelbrotScene::renderLoop(uint32 quadVAO)
{
  initializeFrameBuffer(frameBuffer, rbo, frameBufferTexture, windowWidth, windowHeight);

  // background clear color
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  mandelbrotShader.use();
  mandelbrotShader.setUniform("viewPortResolution", glm::vec2(windowWidth, windowHeight));

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
    mandelbrotShader.setUniform("elapsedTime", t);
    mandelbrotShader.setUniform("zoom", zoom);
    mandelbrotShader.setUniform("centerOffset", centerOffset);
    mandelbrotShader.setUniform("colorFavor", colorFavors[currentColorFavorIndex]);
    glDrawElements(GL_TRIANGLES, // drawing mode
                   6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
                   GL_UNSIGNED_INT, // type of the indices
                   0); // offset in the EBO

    glfwSwapBuffers(window); // swaps double buffers (call after all render commands are completed)
    glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
  }
}

void MandelbrotScene::frameBufferSize(uint32 width, uint32 height)
{
  float32 oldWidth = (float32)windowWidth;
  float32 oldHeight = (float32)windowHeight;

  FirstPersonScene::frameBufferSize(width, height);
  initializeFrameBuffer(frameBuffer, rbo, frameBufferTexture, width, height);
  mandelbrotShader.use();
  mandelbrotShader.setUniform("viewPortResolution", glm::vec2(width, height));

  // The center needs to be adjusted when the viewport size changes in order to maintain the same position
  float32 widthRatio = windowWidth / oldWidth;
  float32 heightRatio = windowHeight / oldHeight;
  centerOffset *= glm::vec2(widthRatio, heightRatio);
}

void MandelbrotScene::mouseScroll(float32 yOffset)
{
  float zoomDelta = zoom * 0.03f * zoomSpeed;
  if(yOffset < 0) {
    zoom -= zoomDelta;
  } else if(yOffset > 0) {
    zoom += zoomDelta;
  }
}

void MandelbrotScene::key_LeftMouseButton_pressed(float32 xPos, float32 yPos)
{
  mouseDownTime = (float32)glfwGetTime();
  mouseDown = true;
}

void MandelbrotScene::key_LeftMouseButton_released(float32 xPos, float32 yPos)
{
  if((float32)glfwGetTime() - mouseDownTime < MOUSE_ACTION_TIME_SECONDS) {
    currentColorFavorIndex++;
    if(currentColorFavorIndex >= ArrayCount(colorFavors)) currentColorFavorIndex = 0;
  }
  mouseDown = false;
}

void MandelbrotScene::mouseMovement(float32 xOffset, float32 yOffset)
{
  if(mouseDown) {
    centerOffset -= glm::vec2(xOffset / zoom, yOffset / zoom);
  }
}

void MandelbrotScene::key_LeftShift_pressed()
{
  zoomSpeed = ZOOM_SPEED_FAST;
}

void MandelbrotScene::key_LeftShift_released()
{
  zoomSpeed = ZOOM_SPEED_NORMAL;
}
