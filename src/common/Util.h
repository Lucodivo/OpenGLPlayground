#pragma once

#define GLFW_INCLUDE_NONE // ensure GLFW doesn't load OpenGL headers

#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "../LearnOpenGLPlatform.h"

// TODO: Split this file between GLFW util(toWindowMode), OpenGL util (load2DTexture), and general util(swap)
struct Framebuffer {
  uint32 id;
  uint32 colorAttachment;
  uint32 depthStencilAttachment;
};

void load2DTexture(const char* imgLocation, uint32& textureId, bool flipImageVert = false, bool inputSRGB = false, uint32* width = NULL, uint32* height = NULL);
void loadCubeMapTexture(const char* const imgLocations[6], uint32& textureId, bool flipImageVert = false);
Framebuffer initializeFrameBuffer(uint32 width, uint32 height);
void deleteFrameBuffer(Framebuffer framebuffer);
void deleteFrameBuffers(uint32 count, Framebuffer* framebuffer);
void toFullScreenMode(GLFWwindow* window);
void toWindowedMode(GLFWwindow* window, const uint32 width, const uint32 height);
void snapshot(int width, int height, uint32 frameBuffer = 0);
void swap(float32* a, float32* b);
glm::mat4& reverseZ(glm::mat4& mat);