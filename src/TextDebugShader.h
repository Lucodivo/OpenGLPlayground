#pragma once

#include <glad/glad.h>
#include <glm/detail/type_mat.hpp>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>

#include "LearnOpenGLPlatform.h"
#include "ShaderProgram.h"
#include "common/FileLocations.h"

struct Glyph
{
  GLuint textureID;  // ID handle of the glyph texture
  glm::ivec2 size;       // Size of glyph
  glm::ivec2 bearing;    // Offset from baseline to left/top of glyph
  GLuint advance;    // Offset to advance to next glyph
};

class TextDebugShader {
public:
  TextDebugShader(Extent2D windowExtent);
  ~TextDebugShader();
  void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
  void updateWindowDimens(Extent2D windowExtent);

private:
  std::map<GLchar, Glyph> Characters;

  ShaderProgram shader;
  uint32 vao, vbo;
  Extent2D windowExtent;
  glm::mat4 projectionMat;

  void initDebugTextCharacters();
  void initDebugTextBuffers();
};

TextDebugShader::TextDebugShader(Extent2D windowExtent): shader(textVertexShaderFileLoc, textFragmentShaderFileLoc) {
  updateWindowDimens(windowExtent);
  initDebugTextCharacters();
  initDebugTextBuffers();
}

TextDebugShader::~TextDebugShader() {
  shader.deleteShaderResources();
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
}

void TextDebugShader::initDebugTextCharacters()
{
  FT_Library ft;
  if (FT_Init_FreeType(&ft))
    std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

  FT_Face face;
  if (FT_New_Face(ft, "src/fonts/arial.ttf", 0, &face))
    std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

  FT_Set_Pixel_Sizes(face, 0, 48);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

  for (GLubyte c = 0; c < 128; c++)
  {
    // Load character glyph
    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
      std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
      continue;
    }
    // Generate texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
    );
    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Now store character for later use
    Glyph glyph = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (GLuint)face->glyph->advance.x
    };
    Characters.insert(std::pair<GLchar, Glyph>(c, glyph));
  }

  FT_Done_Face(face);
  FT_Done_FreeType(ft);
}

void TextDebugShader::initDebugTextBuffers()
{
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void TextDebugShader::updateWindowDimens(Extent2D windowExtent)
{
  this->windowExtent = windowExtent;
  projectionMat = glm::ortho(0.0f, (float)windowExtent.width, 0.0f, (float)windowExtent.height);
  shader.use();
  shader.setUniform("projection", projectionMat);
}

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

  glViewport(0, 0, windowExtent.width, windowExtent.height);

  // turn on blend values to render text
  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Activate corresponding render state
  shader.use();
  shader.setUniform("textColor", color);
  glBindVertexArray(vao);

  // Iterate through all characters
  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++)
  {
    Glyph character = Characters[*c];

    GLfloat xpos = x + character.bearing.x * scale;
    GLfloat ypos = y - (character.size.y - character.bearing.y) * scale;

    GLfloat w = character.size.x * scale;
    GLfloat h = character.size.y * scale;
    // Update VBO for each character
    GLfloat vertices[6][4] = {
            {xpos,     ypos + h, 0.0, 0.0},
            {xpos,     ypos,     0.0, 1.0},
            {xpos + w, ypos,     1.0, 1.0},

            {xpos,     ypos + h, 0.0, 0.0},
            {xpos + w, ypos,     1.0, 1.0},
            {xpos + w, ypos + h, 1.0, 0.0}
    };
    // Render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, character.textureID);
    // Update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    // Render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
    x += (character.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
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