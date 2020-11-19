#include "Scene.h"

#include "../common/FileLocations.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <glad/glad.h>

// TODO: have function take in Extent2D
void Scene::init(uint32 windowWidth, uint32 windowHeight) {
  Scene::framebufferSizeChange(windowWidth, windowHeight);
}

const char* Scene::title()
{
  return "unnamed Scene";
}

void Scene::inputStatesUpdated()
{
  if(isActive(WindowInput_SizeChange))
  {
    Extent2D extent = getWindowExtent();
    framebufferSizeChange(extent.x, extent.y);
  }
}

// Callback function for when user resizes our window
void Scene::framebufferSizeChange(uint32 width, uint32 height)
{
  glViewport(0, 0, width, height);
  windowHeight = height;
  windowWidth = width;
}