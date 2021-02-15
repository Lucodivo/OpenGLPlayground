#pragma once

#include <glad/glad.h>
#include <map>
#include <iostream>

#include "LearnOpenGLPlatform.h"
#include "ShaderProgram.h"
#include "common/FileLocations.h"

#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation
#include "stb/stb_truetype.h"

const uint32 tffBufferSize = 1 << 22; //4MB
const uint32 bitmapWidth = 512;
const uint32 bitmapHeight = 128;

class TextDebugShader {
public:
  TextDebugShader(Extent2D windowExtent);
  ~TextDebugShader();
  void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
  void updateWindowDimens(Extent2D windowExtent);

private:
  uint32 fontTextureID;
  stbtt_bakedchar cdata[129]; // ASCII 0...127 is 128 glyphs
  float32 characterHeight;

  ShaderProgram shader;
  uint32 vao, vbo, ebo;
  Extent2D windowExtent;
  glm::mat4 projectionMat;

  void initDebugTextCharacters();
  void initDebugTextVertexAttributes();
};

TextDebugShader::TextDebugShader(Extent2D windowExtent): shader(textVertexShaderFileLoc, textFragmentShaderFileLoc) {
  updateWindowDimens(windowExtent);
  initDebugTextCharacters();
  initDebugTextVertexAttributes();
}

TextDebugShader::~TextDebugShader() {
  shader.deleteShaderResources();
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
}

void TextDebugShader::initDebugTextCharacters()
{
  uint8* ttf_buffer = new uint8[tffBufferSize];
  uint8* temp_bitmap = new uint8[bitmapWidth * bitmapHeight];
  fread(ttf_buffer, 1, tffBufferSize, fopen("src/data/fonts/arial.ttf", "rb"));
  stbtt_BakeFontBitmap(ttf_buffer,0, 32.0, temp_bitmap, bitmapWidth, bitmapHeight, 0, 128, cdata); // no guarantee this fits!
  // can free ttf_buffer at this point
  glGenTextures(1, &fontTextureID);
  glBindTexture(GL_TEXTURE_2D, fontTextureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmapWidth, bitmapHeight, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);
  // can free temp_bitmap at this point
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  characterHeight = cdata['A'].y1 - cdata['A'].y0;

  delete[] ttf_buffer;
  delete[] temp_bitmap;
}

void TextDebugShader::initDebugTextVertexAttributes()
{
  const uint8 verticesPerQuad = 4;
  const uint8 floatsPerAttribute = 4; // 2 floats for screen coord, 2 floats for texture coord
  // These indices assume the vertex data is supplied in the order of { x0y0, x0y1, x1y0, x1y1 }
  // which will result in a CCW winding order
  const uint32 indexBuffer[] =  {0, 2, 1, 1, 2, 3 };

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glBufferData(GL_ARRAY_BUFFER, verticesPerQuad * floatsPerAttribute * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, floatsPerAttribute * sizeof(GLfloat), 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBuffer), indexBuffer, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void TextDebugShader::updateWindowDimens(Extent2D windowExtent)
{
  this->windowExtent = windowExtent;
  projectionMat = glm::ortho(0.0f, (float)windowExtent.width, 0.0f, (float)windowExtent.height);
  shader.use();
  shader.setUniform("projection", projectionMat);
}

/*
 * arguments x & y indicate lower left corner offset of text
 */
void TextDebugShader::renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
  // store original values before rendering text
  GLint originalSrcRGB, originalSrcAlpha, originalDstRGB, originalDstAlpha;
  GLint originalProgramID, originalBoundVertexArray, originalActiveTexture, originalTexture0;
  GLint originalArrayBuffer;
  GLint originalViewport[4];
  bool blendWasEnabled = glIsEnabled(GL_BLEND) == GL_TRUE;
  bool depthTestWasEnabled = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE;
  glGetIntegerv(GL_VIEWPORT, originalViewport);
  glGetIntegerv(GL_BLEND_SRC_RGB, &originalSrcRGB);
  glGetIntegerv(GL_BLEND_SRC_ALPHA, &originalSrcAlpha);
  glGetIntegerv(GL_BLEND_DST_RGB, &originalDstRGB);
  glGetIntegerv(GL_BLEND_DST_ALPHA, &originalDstAlpha);
  glGetIntegerv(GL_CURRENT_PROGRAM, &originalProgramID);
  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &originalBoundVertexArray);
  glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &originalArrayBuffer);
  glGetIntegerv(GL_ACTIVE_TEXTURE, &originalActiveTexture);
  glActiveTexture(GL_TEXTURE0); // We will only be using GL_TEXTURE0 to render text
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &originalTexture0);

  // start rendering text
  glViewport(0, 0, windowExtent.width, windowExtent.height);

  // turn on blend values to render text
  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Activate corresponding render state
  shader.use();
  shader.setUniform("textColor", color);
  glBindVertexArray(vao);

  // assume orthographic projection with units = screen pixels, origin at top left
  glBindTexture(GL_TEXTURE_2D, fontTextureID);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  float32 scaleOffsetOverstepX = (x * scale) - x;
  float32 scaleOffsetOverstepY = (y * scale) - y;
  y += characterHeight; // we want bottom left corner of text but stb_truetype wants upper left

  for(uint32 i = 0; i < text.length(); ++i) {
    uint32 c = text[i];
    if (c >= 0 && c < 128) {
      stbtt_aligned_quad characterQuad;
      stbtt_GetBakedQuad(cdata, bitmapWidth, bitmapHeight, c, &x, &y, &characterQuad, 1);//1=opengl & d3d10+,0=d3d9
      GLfloat vertices[4][4] = {
              /*screen coord*/                                                                                    /* texture coord */
              {characterQuad.x0 * scale - scaleOffsetOverstepX, characterQuad.y0 * scale - scaleOffsetOverstepY,  characterQuad.s0, characterQuad.t1},
              {characterQuad.x0 * scale - scaleOffsetOverstepX, characterQuad.y1 * scale - scaleOffsetOverstepY,  characterQuad.s0, characterQuad.t0},
              {characterQuad.x1 * scale - scaleOffsetOverstepX, characterQuad.y0 * scale - scaleOffsetOverstepY,  characterQuad.s1, characterQuad.t1},
              {characterQuad.x1 * scale - scaleOffsetOverstepX, characterQuad.y1 * scale - scaleOffsetOverstepY,  characterQuad.s1, characterQuad.t0},
      };

      // Update VBO for each character
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

      // Render quad
      glDrawElements(GL_TRIANGLES, // drawing mode
                     3 * 2, // 3 vertices per triangle * 2 triangles per quad
                     GL_UNSIGNED_INT, // type of the indices
                     0); // offset in the EBO
    }
  }

  // return to values to state before rendering text
  glBindBuffer(GL_ARRAY_BUFFER, originalArrayBuffer);
  glBindTexture(GL_TEXTURE_2D, originalTexture0);
  glBindVertexArray(originalBoundVertexArray);
  glActiveTexture(originalActiveTexture);
  glUseProgram(originalProgramID);
  // return blend values to original state
  if(!blendWasEnabled) { glDisable(GL_BLEND); }
  if(depthTestWasEnabled) { glEnable(GL_DEPTH_TEST); }
  glBlendFuncSeparate(originalSrcRGB, originalDstRGB, originalSrcAlpha, originalDstAlpha);
  glViewport(originalViewport[0], originalViewport[1], originalViewport[2], originalViewport[3]);
}