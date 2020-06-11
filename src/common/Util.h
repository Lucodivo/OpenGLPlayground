#pragma once

#include <glm/gtc/type_ptr.hpp>
#include "../LearnOpenGLPlatform.h"

void load2DTexture(const char* imgLocation, uint32& textureId, bool flipImageVert = false, bool sRGB = false, uint32* width = nullptr, uint32* height = nullptr);
void loadCubeMapTexture(const char* const imgLocations[6], uint32& textureId, bool flipImageVert = false);
void snapshot(int windowWidth, int windowHeight, char* filename, uint32 frameBuffer = 0);
void swap(float32* a, float32* b);

// Special Matrices
const glm::mat4 reverseZMat4 = glm::mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, -1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
);