//
// Created by Connor on 11/21/2019.
//

#include "MandelbrotScene.h"
#include "../../common/FileLocations.h"
#include "../../common/ObjectData.h"

/* TODO: only first person due to input consumers, change? */
MandelbrotScene::MandelbrotScene(GLFWwindow* window): FirstPersonScene(), window(window) {}

void MandelbrotScene::init(uint32 windowWidth, uint32 windowHeight)
{
  FirstPersonScene::init(windowWidth, windowHeight);
  enableDefaultMouseCameraMovement(false);
  enableDefaultKeyboardCameraMovement(false);

  oldWindowExtent = { (int32)windowWidth, (int32)windowHeight };

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



void MandelbrotScene::inputStatesUpdated() {
  FirstPersonScene::inputStatesUpdated();

  if(isActive(WindowInput_SizeChange)) {
    Extent2D extent2D = getWindowExtent();\

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

  if(hotPress(MouseInput_Left)) {
    mouseDownTime = (float32)glfwGetTime();
    mouseDown = true;
  } else if(hotRelease(MouseInput_Left)) {
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

  if(hotPress(KeyboardInput_Shift_Left)) {
    zoomSpeed = ZOOM_SPEED_FAST;
  } else if (hotRelease(KeyboardInput_Shift_Left)) {
    zoomSpeed = ZOOM_SPEED_NORMAL;
  }
}
