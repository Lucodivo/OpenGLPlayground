#pragma once

#include "LearnOpenGLPlatform.h"

#include<vector>

void load2DTexture(const char* imgLocation, uint32& textureId);
void loadCubeMapTexture(std::vector<const char*> imgLocations, uint32& textureId);