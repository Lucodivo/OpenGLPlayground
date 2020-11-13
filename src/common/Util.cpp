#include "Util.h"

#include <stb/stb_image.h>
#include <glad/glad.h>
#include <iostream>
#include <windows.h>
#include <time.h>

#define SNAPSHOT_NAME_FORMAT "build/SaveData/snapshot_%Y%m%d_%H%M%S.bmp"
#define SNAPSHOT_NAME_SIZE 44

void snapshot(int width, int height, uint32 frameBuffer)
{
  const uint32 bytesPerPixel = 3;
  uint32 bmpSize = width * height * bytesPerPixel;
  char* bmpBuffer = (char*)malloc(bmpSize);
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
  bitmapInfoHeader.biWidth = width;
  bitmapInfoHeader.biHeight = height;
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
  glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);
  glReadPixels((GLint)0, (GLint)0,
               (GLint)width, (GLint)height,
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

    if(width != NULL) *width = w;
    if(height != NULL) *height = h;
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

void swap(float32* a, float32* b)
{
  float32 tmp = *a;
  *a = *b;
  *b = tmp;
}

void toWindowedMode(GLFWwindow* window, const uint32 width, const uint32 height)
{
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  uint32 centeringUpperLeftX = (mode->width / 2) - (width / 2);
  uint32 centeringUpperLeftY = (mode->height / 2) - (height / 2);
  glfwSetWindowMonitor(window, NULL/*Null for windowed mode*/, centeringUpperLeftX, centeringUpperLeftY, width, height, GLFW_DONT_CARE);
}

void toFullScreenMode(GLFWwindow* window)
{
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
}