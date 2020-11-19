#pragma once

#include "../LearnOpenGLPlatform.h"

#define NO_FRAMEBUFFER_ATTACHMENT 0

struct Framebuffer {
  uint32 id;
  uint32 colorAttachment;
  uint32 depthStencilAttachment;
  uint32 width;
  uint32 height;
};

enum FramebufferCreationFlags {
  FramebufferCreate_NoValue = 0,
  FramebufferCreate_NoDepthStencil = 1 << 0,
  FramebufferCreate_color_sRGB = 1 << 1,
};

void load2DTexture(const char* imgLocation, uint32& textureId, bool flipImageVert = false, bool inputSRGB = false, uint32* width = NULL, uint32* height = NULL);
void loadCubeMapTexture(const char* const imgLocations[6], uint32& textureId, bool flipImageVert = false);
Framebuffer initializeFramebuffer(uint32 width, uint32 height, FramebufferCreationFlags flags = FramebufferCreate_NoValue);
void deleteFramebuffer(Framebuffer* framebuffer);
void deleteFramebuffers(uint32 count, Framebuffer** framebuffer);
void snapshot(Framebuffer* framebuffer);