#pragma once

#define GLFW_INCLUDE_NONE // ensure GLFW doesn't load OpenGL headers
#include <GLFW/glfw3.h>

#define VIEWPORT_INIT_WIDTH 1920
#define VIEWPORT_INIT_HEIGHT 1080

void runScenes(GLFWwindow* window);