#include "Scene.h"

#include "../common/FileLocations.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <glad/glad.h>

// TODO: have function take in Extent2D
void Scene::init(Extent2D windowExtent)
{
  Scene::framebufferSizeChange(windowExtent);
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
    framebufferSizeChange(windowExtent);
  }
}

// Callback function for when user resizes our window
void Scene::framebufferSizeChange(Extent2D windowExtent)
{
  glViewport(0, 0, windowExtent.width, windowExtent.height);
  this->windowExtent = windowExtent;
}