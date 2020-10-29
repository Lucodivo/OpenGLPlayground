//
// Created by Connor on 4/17/2020.
//

#include "Pixel2DScene.h"
#include "../../common/FileLocations.h"
#include "../../common/ObjectData.h"
#include "../../common/Util.h"

Pixel2DScene::Pixel2DScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
  : Scene(window, initScreenHeight, initScreenWidth),
    pixel2DShader(pixel2DVertexShaderFileLoc, textureFragmentShaderFileLoc){}

void Pixel2DScene::runScene() {
    uint32 quadVAO, quadVBO, quadEBO;
    initializeFrameBufferQuadVertexAttBuffers(quadVAO, quadVBO, quadEBO);

    renderLoop(quadVAO);

    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
    glDeleteBuffers(1, &quadEBO);
}

void Pixel2DScene::renderLoop(uint32 quadVAO)
{
  // background clear color
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  uint32 widthOffset = (windowWidth / 2) - 32;
  uint32 heightOffset = (windowHeight / 2) - 32;

  pixel2DShader.use();
  pixel2DShader.setUniform("windowDimens", glm::vec2(windowWidth, windowHeight));
  pixel2DShader.setUniform("lowerLeftOffset", glm::vec2(widthOffset, heightOffset));
  pixel2DShader.setUniform("spriteDimens", glm::vec2(64.0, 64.0));

  uint32 textureId;
  load2DTexture("C:\\developer\\repos\\Assets\\Sprites\\Arcadio_64bit_color.png", textureId, true, true);

  glBindVertexArray(quadVAO);

  lastFrame = (float32)glfwGetTime();
  float32 startTime = lastFrame;

  // NOTE: render/game loop
  while (glfwWindowShouldClose(window) == GL_FALSE)
  {
    // check for input
    processKeyboardInput(window, this);
//    processXInput(this);

//    if(pixel2DShader.updateFragmentShaderIfOutdated()) {
//    }

    float32 t = (float32)glfwGetTime() - startTime;
    deltaTime = t - lastFrame;
    lastFrame = t;

    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, // drawing mode
                   6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
                   GL_UNSIGNED_INT, // type of the indices
                   0 /*offset in the EBO */);

    glfwSwapBuffers(window); // swaps double buffers (call after all render commands are completed)
    glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
  }
}