
#include <glad/glad.h>
#include "MultiScene.h"
#include "../common/Input.h"

// TODO: Currently only works with InfiniteCubeScene, RayTracingSphereScene, ReflectRefractScene, and InfiniteCapsule Scene
// TODO: Other scenes need to ensure that gl state is set within drawFrame() and instead of in init()
MultiScene::MultiScene(Scene** scenes, uint32 sceneCount, uint32 startingIndex) : Scene(), scenes(scenes), sceneCount(sceneCount), startingIndex(startingIndex){}

void MultiScene::init(Extent2D windowExtent)
{
  Scene::init(windowExtent);

  drawFramebuffer = initializeFramebuffer(windowExtent, FramebufferCreate_NoDepthStencil);

  fourScenes[0] = scenes[startingIndex];
  fourScenes[1] = scenes[(startingIndex + 1) % sceneCount];
  fourScenes[2] = scenes[(startingIndex + 2) % sceneCount];
  fourScenes[3] = scenes[(startingIndex + 3) % sceneCount];

  quarterWindowExtent = { windowExtent.width / 2, windowExtent.height / 2 };
  fourScenes[0]->init(quarterWindowExtent);
  fourScenes[1]->init(quarterWindowExtent);
  fourScenes[2]->init(quarterWindowExtent);
  fourScenes[3]->init(quarterWindowExtent);
}

Framebuffer MultiScene::drawFrame()
{
  fourFramebuffers[0] = fourScenes[0]->drawFrame();
  fourFramebuffers[1] = fourScenes[1]->drawFrame();
  fourFramebuffers[2] = fourScenes[2]->drawFrame();
  fourFramebuffers[3] = fourScenes[3]->drawFrame();

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawFramebuffer.id);
  glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glBindFramebuffer(GL_READ_FRAMEBUFFER, fourFramebuffers[0].id); // upper left
  glBlitFramebuffer(0, 0, fourFramebuffers[0].extent.width, fourFramebuffers[0].extent.height, 0, quarterWindowExtent.height, quarterWindowExtent.width, windowExtent.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

  glBindFramebuffer(GL_READ_FRAMEBUFFER, fourFramebuffers[1].id); // upper right
  glBlitFramebuffer(0, 0, fourFramebuffers[1].extent.width, fourFramebuffers[1].extent.height, quarterWindowExtent.width, quarterWindowExtent.height, windowExtent.width, windowExtent.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

  glBindFramebuffer(GL_READ_FRAMEBUFFER, fourFramebuffers[2].id); // lower left
  glBlitFramebuffer(0, 0, fourFramebuffers[2].extent.width, fourFramebuffers[2].extent.height, 0, 0, quarterWindowExtent.width, quarterWindowExtent.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

  glBindFramebuffer(GL_READ_FRAMEBUFFER, fourFramebuffers[3].id); // lower right
  glBlitFramebuffer(0, 0, fourFramebuffers[3].extent.width, fourFramebuffers[3].extent.height, quarterWindowExtent.width, 0, windowExtent.width, quarterWindowExtent.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

  glBindFramebuffer(GL_READ_FRAMEBUFFER, drawFramebuffer.id);

  return drawFramebuffer;
}

void MultiScene::framebufferSizeChangeRequest(Extent2D windowExtent)
{
  Scene::framebufferSizeChangeRequest(windowExtent);

  deleteFramebuffer(&drawFramebuffer);
  drawFramebuffer = initializeFramebuffer(windowExtent, FramebufferCreate_NoDepthStencil);

  quarterWindowExtent = { windowExtent.width / 2, windowExtent.height / 2 };
  fourScenes[0]->framebufferSizeChangeRequest(quarterWindowExtent);
  fourScenes[1]->framebufferSizeChangeRequest(quarterWindowExtent);
  fourScenes[2]->framebufferSizeChangeRequest(quarterWindowExtent);
  fourScenes[3]->framebufferSizeChangeRequest(quarterWindowExtent);
}

void MultiScene::deinit()
{
  deleteFramebuffer(&drawFramebuffer);

  fourScenes[0]->deinit();
  fourScenes[1]->deinit();
  fourScenes[2]->deinit();
  fourScenes[3]->deinit();
}

void MultiScene::inputStatesUpdated()
{
  bool prev = hotPress(KeyboardInput_J);
  bool next = hotPress(KeyboardInput_K);
  if(prev || next) {
    if(prev) {
      if(startingIndex == 0) {
        startingIndex = sceneCount - 1;
      } else {
        --startingIndex;
      }

      fourScenes[3]->deinit();

      fourScenes[0] = scenes[startingIndex];
      fourScenes[1] = scenes[(startingIndex + 1) % sceneCount];
      fourScenes[2] = scenes[(startingIndex + 2) % sceneCount];
      fourScenes[3] = scenes[(startingIndex + 3) % sceneCount];

      fourScenes[0]->init(quarterWindowExtent);
    } else {
      ++startingIndex;
      startingIndex %= sceneCount;

      fourScenes[0]->deinit();

      fourScenes[0] = scenes[startingIndex];
      fourScenes[1] = scenes[(startingIndex + 1) % sceneCount];
      fourScenes[2] = scenes[(startingIndex + 2) % sceneCount];
      fourScenes[3] = scenes[(startingIndex + 3) % sceneCount];

      fourScenes[3]->init(quarterWindowExtent);
    }
  }

  fourScenes[0]->inputStatesUpdated();
  fourScenes[1]->inputStatesUpdated();
  fourScenes[2]->inputStatesUpdated();
  fourScenes[3]->inputStatesUpdated();
}

void MultiScene::drawGui()
{
  fourScenes[0]->drawGui();
  fourScenes[1]->drawGui();
  fourScenes[2]->drawGui();
  fourScenes[3]->drawGui();
}
