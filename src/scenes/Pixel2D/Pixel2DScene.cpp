//
// Created by Connor on 4/17/2020.
//

#include "Pixel2DScene.h"
#include "../../common/FileLocations.h"
#include "../../common/ObjectData.h"
#include "../../common/Util.h"

const uint32 textureIndex = 0;

Pixel2DScene::Pixel2DScene() : Scene() {}

const char* Pixel2DScene::title()
{
  return "2D Pixel";
}

void Pixel2DScene::init(Extent2D windowExtent)
{
  Scene::init(windowExtent);

  pixel2DShader = new ShaderProgram(pixel2DVertexShaderFileLoc, textureFragmentShaderFileLoc);
          
  quadVertexAtt = initializeFramebufferQuadVertexAttBuffers();

  drawFramebuffer = initializeFramebuffer(windowExtent, FramebufferCreate_color_sRGB);

  load2DTexture(flowerTextureLoc, textureId, true, true, &textureWidth, &textureHeight);

  uint32 widthOffset = (windowExtent.width / 2) - (textureWidth / 2);
  uint32 heightOffset = (windowExtent.height / 2) - (textureHeight / 2);

  pixel2DShader->use();
  pixel2DShader->setUniform("windowDimens", glm::vec2(windowExtent.width, windowExtent.height));
  pixel2DShader->setUniform("lowerLeftOffset", glm::vec2(widthOffset, heightOffset));
  pixel2DShader->setUniform("spriteDimens", glm::vec2(textureWidth, textureHeight));
  pixel2DShader->setUniform("tex", 0);

  lastFrame = getTime();
  startTime = lastFrame;

  glBindVertexArray(quadVertexAtt.arrayObject);
  glActiveTexture(GL_TEXTURE0 + textureIndex);
  glBindTexture(GL_TEXTURE_2D, textureId);

  // background clear color
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glDisable(GL_DEPTH_TEST);

  // Turn on free gamma correction for entire scene, only affects color attachments
  glEnable(GL_FRAMEBUFFER_SRGB);

  glViewport(0, 0, windowExtent.width, windowExtent.height);
}

void Pixel2DScene::deinit()
{
  Scene::deinit();
  
  pixel2DShader->deleteShaderResources();
  delete pixel2DShader;
  
  deleteVertexAtt(quadVertexAtt);

  deleteFramebuffer(&drawFramebuffer);

  glDeleteTextures(1, &textureId);

  glDisable(GL_FRAMEBUFFER_SRGB);
}

Framebuffer Pixel2DScene::drawFrame()
{
  float32 t = getTime() - startTime;
  deltaTime = t - lastFrame;
  lastFrame = t;

  glBindFramebuffer(GL_FRAMEBUFFER, drawFramebuffer.id);
  glClear(GL_COLOR_BUFFER_BIT);

  glDrawElements(GL_TRIANGLES, // drawing mode
                 6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
                 GL_UNSIGNED_INT, // type of the indices
                 0 /*offset in the EBO */);

  return drawFramebuffer;
}

void Pixel2DScene::framebufferSizeChangeRequest(Extent2D windowExtent)
{
  Scene::framebufferSizeChangeRequest(windowExtent);

  glViewport(0, 0, windowExtent.width, windowExtent.height);

  deleteFramebuffer(&drawFramebuffer);
  drawFramebuffer = initializeFramebuffer(windowExtent, FramebufferCreate_NoDepthStencil);

  uint32 widthOffset = (windowExtent.width / 2) - (textureWidth / 2);
  uint32 heightOffset = (windowExtent.height / 2) - (textureHeight / 2);

  pixel2DShader->use();
  pixel2DShader->setUniform("windowDimens", glm::vec2(windowExtent.width, windowExtent.height));
  pixel2DShader->setUniform("lowerLeftOffset", glm::vec2(widthOffset, heightOffset));
}
