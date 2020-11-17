//
// Created by Connor on 11/21/2019.
//

#include "MandelbrotScene.h"
#include "../../common/FileLocations.h"
#include "../../common/ObjectData.h"

MandelbrotScene::MandelbrotScene(GLFWwindow* window): Scene(), window(window) {}

const char* MandelbrotScene::title()
{
  return "Mandelbrot";
}

void MandelbrotScene::init(uint32 windowWidth, uint32 windowHeight)
{
  Scene::init(windowWidth, windowHeight);

  if(oldWindowExtent.x != 0) // Adjust the center offset in the event that the window size has changed since last init
  {
    float32 widthRatio = (float32)windowWidth / oldWindowExtent.x;
    float32 heightRatio = (float32)windowHeight / oldWindowExtent.y;
    centerOffset *= glm::vec2(widthRatio, heightRatio);
  }

  drawFramebuffer = initializeFramebuffer(windowWidth, windowHeight, false);

  oldWindowExtent = { (int32)windowWidth, (int32)windowHeight };

  mandelbrotShader = new Shader(UVCoordVertexShaderFileLoc, MandelbrotFragmentShaderFileLoc);
  mandelbrotShader->use();
  mandelbrotShader->setUniform("viewPortResolution", glm::vec2(windowWidth, windowHeight));

  enableCursor(window, true);

  quadVertexAtt = initializeFramebufferQuadVertexAttBuffers();

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glDisable(GL_DEPTH_TEST);

  glBindVertexArray(quadVertexAtt.arrayObject);

  lastFrame = (float32)glfwGetTime();
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
  float32 t = (float32)glfwGetTime() - startTime;
  deltaTime = t - lastFrame;
  lastFrame = t;

  glBindFramebuffer(GL_FRAMEBUFFER, drawFramebuffer.id);
  glClear(GL_COLOR_BUFFER_BIT);

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

  if(isActive(WindowInput_SizeChange)) {
    Extent2D extent2D = getWindowExtent();

    deleteFramebuffer(&drawFramebuffer);
    drawFramebuffer = initializeFramebuffer(windowWidth, windowHeight, false);

    mandelbrotShader->use();
    mandelbrotShader->setUniform("viewPortResolution", glm::vec2(extent2D.x, extent2D.y));

    // The center needs to be adjusted when the viewport size changes in order to maintain the same position
    float32 widthRatio = (float32)windowWidth / oldWindowExtent.x;
    float32 heightRatio = (float32)windowHeight / oldWindowExtent.y;
    oldWindowExtent = extent2D;
    centerOffset *= glm::vec2(widthRatio, heightRatio);
  }

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
    mouseDownTime = (float32)glfwGetTime();
    mouseDown = true;
  } else if(mouseLeftState == INPUT_HOT_RELEASE) {
    if((float32)glfwGetTime() - mouseDownTime < MOUSE_ACTION_TIME_SECONDS) {
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