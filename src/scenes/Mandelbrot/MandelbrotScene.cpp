//
// Created by Connor on 11/21/2019.
//

#include "MandelbrotScene.h"
#include "../../common/FileLocations.h"
#include "../../common/ObjectData.h"
#include "../../common/Input.h"
#include "../../common/Util.h"

MandelbrotScene::MandelbrotScene(GLFWwindow* window): Scene(), window(window) {}

const char* MandelbrotScene::title()
{
  return "Mandelbrot";
}

void MandelbrotScene::init(Extent2D windowExtent)
{
  Scene::init(windowExtent);

  if(prevWindowExtent.width != 0) // Adjust the center offset in the event that the window size has changed since last init
  {
    float32 widthRatio = (float32)windowExtent.width / prevWindowExtent.width;
    float32 heightRatio = (float32)windowExtent.height / prevWindowExtent.height;
    centerOffset *= glm::vec2(widthRatio, heightRatio);
  }

  drawFramebuffer = initializeFramebuffer(windowExtent, FramebufferCreate_NoDepthStencil);

  prevWindowExtent = {windowExtent.width, windowExtent.height };

  mandelbrotShader = new ShaderProgram(UVCoordVertexShaderFileLoc, MandelbrotFragmentShaderFileLoc);
  mandelbrotShader->use();
  mandelbrotShader->setUniform("viewPortResolution", glm::vec2( windowExtent.width, windowExtent.height ));

  enableCursor(window, true);

  quadVertexAtt = initializeFramebufferQuadVertexAttBuffers();

  lastFrame = getTime();
  startTime = lastFrame;
}

void MandelbrotScene::deinit()
{
  Scene::deinit();

  deleteFramebuffer(&drawFramebuffer);

  mandelbrotShader->deleteShaderResources();
  delete mandelbrotShader;

  deleteVertexAtt(quadVertexAtt);
}

Framebuffer MandelbrotScene::drawFrame()
{
  local_access float32 previousZoom = zoom - 0.5f; // NOTE: values initially set to slight offset for the check below
  local_access glm::vec2 previousOffset = glm::vec2{centerOffset.x, centerOffset.y};
  local_access uint32 previousColorFavorIndex = currentColorFavorIndex;
  local_access Extent2D prevWindowExtent = windowExtent;

  // we don't need to draw if we have not zoomed in since last frame
  if(previousZoom == zoom && previousOffset == centerOffset
      && previousColorFavorIndex == currentColorFavorIndex
      && prevWindowExtent.height == windowExtent.height
      && prevWindowExtent.width == windowExtent.width)
  {
    return drawFramebuffer;
  }
  previousZoom = zoom;
  previousOffset = centerOffset;
  previousColorFavorIndex = currentColorFavorIndex;

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glDisable(GL_DEPTH_TEST);
  glViewport(0, 0, windowExtent.width, windowExtent.height);

  glBindVertexArray(quadVertexAtt.arrayObject);

  glBindFramebuffer(GL_FRAMEBUFFER, drawFramebuffer.id);
  glClear(GL_COLOR_BUFFER_BIT);

  float32 t = getTime() - startTime;
  deltaTime = t - lastFrame;
  lastFrame = t;

  mandelbrotShader->use();
  mandelbrotShader->setUniform("elapsedTime", t); // used with HexagonPlayground, NOT mandelbrot shader
  mandelbrotShader->setUniform("zoom", zoom);
  mandelbrotShader->setUniform("centerOffset", centerOffset);
  mandelbrotShader->setUniform("colorFavor", colorFavors[currentColorFavorIndex]);
  glDrawElements(GL_TRIANGLES, // drawing mode
                 6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
                 GL_UNSIGNED_INT, // type of the indices
                 0); // offset in the EBO

   return drawFramebuffer;
}

void MandelbrotScene::inputStatesUpdated() {
  Scene::inputStatesUpdated();

  if(isActive(MouseInput_Scroll)) {
    float32 yOffset = getMouseScrollY();
    float zoomDelta = zoom * 0.03f * zoomSpeed;
    if(yOffset < 0) {
      zoom -= zoomDelta;
    } else if(yOffset > 0) {
      zoom += zoomDelta;
    }
  }

  InputState mouseLeftState = getInputState(MouseInput_Left);
  if(mouseLeftState == INPUT_HOT_PRESS) {
    mouseDownTime = getTime();
    mouseDown = true;
  } else if(mouseLeftState == INPUT_HOT_RELEASE) {
    if(getTime() - mouseDownTime < MOUSE_ACTION_TIME_SECONDS) {
      currentColorFavorIndex++;
      if(currentColorFavorIndex >= ArrayCount(colorFavors)) currentColorFavorIndex = 0;
    }
    mouseDown = false;
  }

  if(isActive(MouseInput_Movement)) {
    MouseCoord mouseDelta = getMouseDelta();
    if(mouseDown) {
      // Note: mouseDelta.y is negative when mouse moves up due to upper left origin (0, 0)
      centerOffset -= glm::vec2(mouseDelta.x / zoom, -mouseDelta.y / zoom);
    }
  }

  InputState leftShiftState = getInputState(KeyboardInput_Shift_Left);
  if(leftShiftState == INPUT_HOT_PRESS) {
    zoomSpeed = ZOOM_SPEED_FAST;
  } else if (leftShiftState == INPUT_HOT_RELEASE) {
    zoomSpeed = ZOOM_SPEED_NORMAL;
  }
}

void MandelbrotScene::framebufferSizeChangeRequest(Extent2D windowExtent)
{
  Scene::framebufferSizeChangeRequest(windowExtent);

  deleteFramebuffer(&drawFramebuffer);
  drawFramebuffer = initializeFramebuffer(windowExtent, FramebufferCreate_NoDepthStencil);

  mandelbrotShader->use();
  mandelbrotShader->setUniform("viewPortResolution", glm::vec2(windowExtent.width, windowExtent.height));

  // The center needs to be adjusted when the viewport size changes in order to maintain the same position
  float32 widthRatio = (float32)windowExtent.width / prevWindowExtent.width;
  float32 heightRatio = (float32)windowExtent.height / prevWindowExtent.height;
  prevWindowExtent = windowExtent;
  centerOffset *= glm::vec2(widthRatio, heightRatio);
}
