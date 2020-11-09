#include "Scene.h"

#include "../common/FileLocations.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <glad/glad.h>

Scene::Scene() {}

void Scene::init(uint32 windowWidth, uint32 windowHeight) {
  glViewport(0, 0, windowWidth, windowHeight);
  this->windowWidth = windowWidth;
  this->windowHeight = windowHeight;
}

// Callback function for when user resizes our window
void Scene::framebufferSizeChange(uint32 width, uint32 height)
{
  glViewport(0, 0, width, height);
  windowHeight = height;
  windowWidth = width;
}