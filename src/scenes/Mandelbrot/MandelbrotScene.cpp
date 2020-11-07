//
// Created by Connor on 11/21/2019.
//

#include "MandelbrotScene.h"
#include "../../common/FileLocations.h"
#include "../../common/ObjectData.h"

MandelbrotScene::MandelbrotScene(GLFWwindow* window)
        : FirstPersonScene(window){} /* TODO: only first person due to input consumers, change? */

void MandelbrotScene::init(uint32 windowWidth, uint32 windowHeight)
{
  FirstPersonScene::init(windowWidth, windowHeight);

  mandelbrotShader = new Shader(UVCoordVertexShaderFileLoc, MandelbrotFragmentShaderFileLoc);
  mandelbrotShader->use();
  mandelbrotShader->setUniform("viewPortResolution", glm::vec2(windowWidth, windowHeight));

  deinitCursorInputMode = glfwGetInputMode(window, GLFW_CURSOR);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  quadVertexAtt = initializeFrameBufferQuadVertexAttBuffers();

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glDisable(GL_DEPTH_TEST);

  glBindVertexArray(quadVertexAtt.arrayObject);

  lastFrame = (float32)glfwGetTime();
  startTime = lastFrame;
}

void MandelbrotScene::deinit()
{
  FirstPersonScene::deinit();

  mandelbrotShader->deleteShaderResources();
  delete mandelbrotShader;

  glfwSetInputMode(window, GLFW_CURSOR, deinitCursorInputMode);

  deleteVertexAtt(quadVertexAtt);
}

void MandelbrotScene::drawFrame()
{
  FirstPersonScene::drawFrame();

  float32 t = (float32)glfwGetTime() - startTime;
  deltaTime = t - lastFrame;
  lastFrame = t;

  glClear(GL_COLOR_BUFFER_BIT);

  mandelbrotShader->setUniform("elapsedTime", t); // used with HexagonPlayground, NOT mandelbrot shader
  mandelbrotShader->setUniform("zoom", zoom);
  mandelbrotShader->setUniform("centerOffset", centerOffset);
  mandelbrotShader->setUniform("colorFavor", colorFavors[currentColorFavorIndex]);
  glDrawElements(GL_TRIANGLES, // drawing mode
                 6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
                 GL_UNSIGNED_INT, // type of the indices
                 0); // offset in the EBO
}

void MandelbrotScene::framebufferSizeChange(uint32 width, uint32 height)
{
  float32 oldWidth = (float32)windowWidth;
  float32 oldHeight = (float32)windowHeight;
  FirstPersonScene::framebufferSizeChange(width, height);

  mandelbrotShader->use();
  mandelbrotShader->setUniform("viewPortResolution", glm::vec2(width, height));

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
