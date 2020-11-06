#pragma once

#define GLFW_INCLUDE_NONE // ensure GLFW doesn't load OpenGL headers

#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "../LearnOpenGLPlatform.h"

void load2DTexture(const char* imgLocation, uint32& textureId, bool flipImageVert = false, bool inputSRGB = false, uint32* width = NULL, uint32* height = NULL);
void loadCubeMapTexture(const char* const imgLocations[6], uint32& textureId, bool flipImageVert = false);
void toFullScreenMode(GLFWwindow* window);
void toWindowedMode(GLFWwindow* window, const uint32 width, const uint32 height);
void snapshot(int windowWidth, int windowHeight, char* filename, uint32 frameBuffer = 0);
void swap(float32* a, float32* b);

// Special Matrices
const glm::mat4 reverseZMat4 = glm::mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, -1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
);