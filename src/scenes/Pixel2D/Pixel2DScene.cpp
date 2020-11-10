//
// Created by Connor on 4/17/2020.
//

#include "Pixel2DScene.h"
#include "../../common/FileLocations.h"
#include "../../common/ObjectData.h"
#include "../../common/Util.h"

Pixel2DScene::Pixel2DScene() : Scene() {}

const char* Pixel2DScene::title()
{
  return "2D Pixel";
}

void Pixel2DScene::init(uint32 windowWidth, uint32 windowHeight)
{
  Scene::init(windowWidth, windowHeight);

  pixel2DShader = new Shader(pixel2DVertexShaderFileLoc, textureFragmentShaderFileLoc);
          
  quadVertexAtt = initializeFrameBufferQuadVertexAttBuffers();

  // background clear color
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glDisable(GL_DEPTH_TEST);

  uint32 width, height;
  load2DTexture(flowerTextureLoc, textureId, true, true, &width, &height);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureId);

  // Turn on free gamma correction for entire scene, only affects color attachments
  glEnable(GL_FRAMEBUFFER_SRGB);

  uint32 widthOffset = (windowWidth / 2) - (width / 2);
  uint32 heightOffset = (windowHeight / 2) - (height / 2);

  pixel2DShader->use();
  pixel2DShader->setUniform("windowDimens", glm::vec2(windowWidth, windowHeight));
  pixel2DShader->setUniform("lowerLeftOffset", glm::vec2(widthOffset, heightOffset));
  pixel2DShader->setUniform("spriteDimens", glm::vec2(64.0, 64.0));
  pixel2DShader->setUniform("tex", 0);

  glBindVertexArray(quadVertexAtt.arrayObject);

  lastFrame = (float32)glfwGetTime();
  startTime = lastFrame;
}

void Pixel2DScene::deinit()
{
  Scene::deinit();
  
  pixel2DShader->deleteShaderResources();
  delete pixel2DShader;
  
  deleteVertexAtt(quadVertexAtt);

  glDeleteTextures(1, &textureId);

  glDisable(GL_FRAMEBUFFER_SRGB);
}

void Pixel2DScene::drawFrame()
{
  Scene::drawFrame();

  float32 t = (float32)glfwGetTime() - startTime;
  deltaTime = t - lastFrame;
  lastFrame = t;

  glClear(GL_COLOR_BUFFER_BIT);

  glDrawElements(GL_TRIANGLES, // drawing mode
                 6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
                 GL_UNSIGNED_INT, // type of the indices
                 0 /*offset in the EBO */);
}