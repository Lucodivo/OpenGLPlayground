#include "OpenGLUtil.h"

#include <stb/stb_image.h>
#include <glad/glad.h>
#include <iostream>
#include <windows.h>
#include <time.h>

#define SNAPSHOT_NAME_FORMAT "build/SaveData/snapshot_%Y%m%d_%H%M%S.bmp"
#define SNAPSHOT_NAME_SIZE 44

void snapshot(Framebuffer* framebuffer)
{
  const uint32 bytesPerPixel = 3;
  uint32 bmpSize = framebuffer->extent.width * framebuffer->extent.height * bytesPerPixel;
  char* bmpBuffer = (char*) malloc(bmpSize);
  if (!bmpBuffer) return;

  char fileName[SNAPSHOT_NAME_SIZE];
  time_t now = time(0);
  strftime(fileName, sizeof(fileName), SNAPSHOT_NAME_FORMAT, localtime(&now));
  FILE *filePtr = fopen(fileName, "wb"); // mode w: Create an empty file for output operations, mode b: Open as binary file
  if (!filePtr) { return; }

  BITMAPFILEHEADER bitmapFileHeader = {0};
  bitmapFileHeader.bfType = 0x4D42; //"BM"
  bitmapFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmpSize;
  bitmapFileHeader.bfReserved1 = 0;
  bitmapFileHeader.bfReserved2 = 0;
  bitmapFileHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

  BITMAPINFOHEADER bitmapInfoHeader = {0};
  bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
  bitmapInfoHeader.biWidth = framebuffer->extent.width;
  bitmapInfoHeader.biHeight = framebuffer->extent.height;
  bitmapInfoHeader.biPlanes = 1;
  bitmapInfoHeader.biBitCount = 24;
  bitmapInfoHeader.biCompression = BI_RGB;
  bitmapInfoHeader.biSizeImage = 0;
  bitmapInfoHeader.biXPelsPerMeter = 0;
  bitmapInfoHeader.biYPelsPerMeter = 0;
  bitmapInfoHeader.biClrUsed = 0;
  bitmapInfoHeader.biClrImportant = 0;

  GLint originalReadFramebuffer;
  glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &originalReadFramebuffer);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer->id);
  glReadPixels((GLint)0, (GLint)0,
               (GLint)framebuffer->extent.width, (GLint)framebuffer->extent.height,
               GL_BGR, GL_UNSIGNED_BYTE, bmpBuffer);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, originalReadFramebuffer);

  fwrite(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
  fwrite(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
  fwrite(bmpBuffer, bmpSize, 1, filePtr);
  fclose(filePtr);

  free(bmpBuffer);
}

void load2DTexture(const char* imgLocation, uint32& textureId, bool flipImageVert, bool inputSRGB, uint32* width, uint32* height)
{
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);

  // load image data
  int w, h, numChannels;
  stbi_set_flip_vertically_on_load(flipImageVert);
  unsigned char* data = stbi_load(imgLocation, &w, &h, &numChannels, 0 /*desired channels*/);
  if (data && numChannels <= 4)
  {
    uint32 dataColorSpace;
    uint32 dataComponentComposition;
    if (numChannels == 3)
    {
      dataColorSpace = inputSRGB ? GL_SRGB : GL_RGB;
      dataComponentComposition = GL_RGB;
    } else if(numChannels == 4)
    {
      dataColorSpace = inputSRGB ? GL_SRGB_ALPHA : GL_RGBA;
      dataComponentComposition = GL_RGBA;
    } else if(numChannels == 1) {
      dataColorSpace = dataComponentComposition = GL_RED;
    } else if(numChannels == 2) {
      dataColorSpace = dataComponentComposition = GL_RG;
    }

    glTexImage2D(GL_TEXTURE_2D, // target
                 0, // level of detail (level n is the nth mipmap reduction image)
                 dataColorSpace, // What is the color space of the data
                 w, // width of texture
                 h, // height of texture
                 0, // border (legacy stuff, MUST BE 0)
                 dataComponentComposition, // How are the components of the data composed
                 GL_UNSIGNED_BYTE, // specifies data type of pixel data
                 data); // pointer to the image data
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // set texture options
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // disables bilinear filtering (creates sharp edges when magnifying texture)

    if (width != NULL) *width = w;
    if (height != NULL) *height = h;
  } else
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data); // free texture image memory
}

void loadCubeMapTexture(const char* const imgLocations[6], uint32& textureId, bool flipImageVert)
{
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(flipImageVert);
  for (uint32 i = 0; i < 6; i++)
  {
    unsigned char* data = stbi_load(imgLocations[i], &width, &height, &nrChannels, 0);
    if (data)
    {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                   0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
      );
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    } else
    {
      std::cout << "Cubemap texture failed to load at path: " << imgLocations[i] << std::endl;
    }
    stbi_image_free(data);
  }
}

Framebuffer initializeFramebuffer(Extent2D framebufferExtent, FramebufferCreationFlags flags)
{
  Framebuffer resultBuffer;
  resultBuffer.extent = framebufferExtent;

  GLint originalDrawFramebuffer, originalReadFramebuffer, originalActiveTexture, originalTexture0;
  glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &originalDrawFramebuffer);
  glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &originalReadFramebuffer);
  glGetIntegerv(GL_ACTIVE_TEXTURE, &originalActiveTexture);

  // creating frame buffer
  glGenFramebuffers(1, &resultBuffer.id);
  glBindFramebuffer(GL_FRAMEBUFFER, resultBuffer.id);

  // creating frame buffer color texture
  glGenTextures(1, &resultBuffer.colorAttachment);
  // NOTE: Binding the texture to the GL_TEXTURE_2D target, means that
  // NOTE: gl operations on the GL_TEXTURE_2D target will affect our texture
  // NOTE: while it is remains bound to that target
  glActiveTexture(GL_TEXTURE0);
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &originalTexture0);
  glBindTexture(GL_TEXTURE_2D, resultBuffer.colorAttachment);
  GLint internalFormat = (flags & FramebufferCreate_color_sRGB) ? GL_SRGB : GL_RGB;
  glTexImage2D(GL_TEXTURE_2D, 0/*LoD*/, internalFormat, framebufferExtent.width, framebufferExtent.height, 0/*border*/, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // attach texture w/ color to frame buffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, // frame buffer we're targeting (draw, read, or both)
                         GL_COLOR_ATTACHMENT0, // type of attachment
                         GL_TEXTURE_2D, // type of texture
                         resultBuffer.colorAttachment, // texture
                         0); // mipmap level

  if (flags & FramebufferCreate_NoDepthStencil)
  {
    resultBuffer.depthStencilAttachment = NO_FRAMEBUFFER_ATTACHMENT;
  } else {
    // creating render buffer to be depth/stencil buffer
    glGenRenderbuffers(1, &resultBuffer.depthStencilAttachment);
    glBindRenderbuffer(GL_RENDERBUFFER, resultBuffer.depthStencilAttachment);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framebufferExtent.width, framebufferExtent.height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0); // unbind
    // attach render buffer w/ depth & stencil to frame buffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, // frame buffer target
                              GL_DEPTH_STENCIL_ATTACHMENT, // attachment point of frame buffer
                              GL_RENDERBUFFER, // render buffer target
                              resultBuffer.depthStencilAttachment);  // render buffer
  }

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
  }
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, originalDrawFramebuffer);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, originalReadFramebuffer);
  glBindTexture(GL_TEXTURE_2D, originalTexture0); // re-bind original texture
  glActiveTexture(originalActiveTexture);
  return resultBuffer;
}

void deleteFramebuffer(Framebuffer* framebuffer)
{
  glDeleteFramebuffers(1, &framebuffer->id);
  glDeleteTextures(1, &framebuffer->colorAttachment);
  if (framebuffer->depthStencilAttachment != NO_FRAMEBUFFER_ATTACHMENT)
  {
    glDeleteRenderbuffers(1, &framebuffer->depthStencilAttachment);
  }
  *framebuffer = {0, 0, 0, 0, 0};
}

void deleteFramebuffers(uint32 count, Framebuffer** framebuffer)
{
  uint32* deleteFramebufferObjects = new uint32[count * 3];
  uint32* deleteColorAttachments = deleteFramebufferObjects + count;
  uint32* deleteDepthStencilAttachments = deleteColorAttachments + count;
  uint32 depthStencilCount = 0;
  for (uint32 i = 0; i < count; i++)
  {
    deleteFramebufferObjects[i] = framebuffer[i]->id;
    deleteColorAttachments[i] = framebuffer[i]->colorAttachment;
    if (framebuffer[i]->depthStencilAttachment != NO_FRAMEBUFFER_ATTACHMENT)
    {
      deleteDepthStencilAttachments[depthStencilCount++] = framebuffer[i]->depthStencilAttachment;
      *(framebuffer[i]) = {0, 0, 0, 0, 0};
    }
  }

  glDeleteFramebuffers(count, deleteFramebufferObjects);
  glDeleteTextures(count, deleteColorAttachments);
  if (depthStencilCount != 0)
  {
    glDeleteRenderbuffers(depthStencilCount, deleteDepthStencilAttachments);
  }

  delete[] deleteFramebufferObjects;
}