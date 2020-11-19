#pragma once

#define GLFW_INCLUDE_NONE // ensure GLFW doesn't load OpenGL headers
#include <GLFW/glfw3.h>

#include "../LearnOpenGLPlatform.h"

void toFullScreenMode(GLFWwindow* window);
void toWindowedMode(GLFWwindow* window, const uint32 width, const uint32 height);

