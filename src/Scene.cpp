#include "Scene.h"

#include "FileLocations.h"

#include <ft2build.h>
#include FT_FREETYPE_H

Scene::Scene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
  : window(window), 
  viewportHeight(initScreenHeight), 
  viewportWidth(initScreenWidth),
  textDebugShader(textVertexShaderFileLoc, textFragmentShaderFileLoc)
{
  subscribeFrameBufferSize(window, this);
  initDebugTextCharacters();
  initDebugTextBuffers();
  textDebugProjectionMat = glm::ortho(0.0f, (float)viewportWidth, 0.0f, (float)viewportWidth);
  textDebugShader.use();
  textDebugShader.setUniform("projection", textDebugProjectionMat);
}

// Callback function for when user resizes our window
void Scene::frameBufferSize(uint32 width, uint32 height)
{
  glViewport(0, 0, width, height);
  viewportHeight = height;
  viewportWidth = width;
  textDebugProjectionMat = glm::ortho(0.0f, (float)viewportWidth, 0.0f, (float)viewportWidth);
  textDebugShader.use();
  textDebugShader.setUniform("projection", textDebugProjectionMat);
}

void Scene::adjustWindowSize()
{
  local_persist bool windowMode = true;
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  if(windowMode)
  {
    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
  } else
  {
    glfwSetWindowMonitor(window, NULL, (mode->width / 4), (mode->height / 4), VIEWPORT_INIT_WIDTH, VIEWPORT_INIT_HEIGHT, GLFW_DONT_CARE);
  }
  windowMode = !windowMode;
}

void Scene::closeWindow()
{
  glfwSetWindowShouldClose(window, GL_TRUE);
}

void Scene::initDebugTextCharacters()
{
  FT_Library ft;
  if(FT_Init_FreeType(&ft))
    std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

  FT_Face face;
  if(FT_New_Face(ft, "src/fonts/arial.ttf", 0, &face))
    std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

  FT_Set_Pixel_Sizes(face, 0, 48);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

  for(GLubyte c = 0; c < 128; c++)
  {
    // Load character glyph 
    if(FT_Load_Char(face, c, FT_LOAD_RENDER))
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
    Character character = {
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        face->glyph->advance.x
    };
    Characters.insert(std::pair<GLchar, Character>(c, character));
  }

  FT_Done_Face(face);
  FT_Done_FreeType(ft);
}


void Scene::initDebugTextBuffers()
{
  glGenVertexArrays(1, &textDebugVAO);
  glGenBuffers(1, &textDebugVBO);
  glBindVertexArray(textDebugVAO);
  glBindBuffer(GL_ARRAY_BUFFER, textDebugVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Scene::renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
  textDebugShader.use();

  // Activate corresponding render state	
  textDebugShader.setUniform("textColor", color);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(textDebugVAO);

  // Iterate through all characters
  std::string::const_iterator c;
  for(c = text.begin(); c != text.end(); c++)
  {
    Character ch = Characters[*c];

    GLfloat xpos = x + ch.bearing.x * scale;
    GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

    GLfloat w = ch.size.x * scale;
    GLfloat h = ch.size.y * scale;
    // Update VBO for each character
    GLfloat vertices[6][4] = {
        { xpos,     ypos + h,   0.0, 0.0 },
        { xpos,     ypos,       0.0, 1.0 },
        { xpos + w, ypos,       1.0, 1.0 },

        { xpos,     ypos + h,   0.0, 0.0 },
        { xpos + w, ypos,       1.0, 1.0 },
        { xpos + w, ypos + h,   1.0, 0.0 }
    };
    // Render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.textureID);
    // Update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, textDebugVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
    x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

FirstPersonScene::FirstPersonScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
  : Scene(window, initScreenHeight, initScreenWidth)
{
  subscribeMouseMovement(window, this);
  subscribeMouseScroll(window, this);
}

// +++ CONTROLLER CONSUMER IMPLEMETNATION - START +++
void FirstPersonScene::leftAnalog(int16 stickX, int16 stickY)
{
  camera.ProcessLeftAnalog(stickX, stickY);
}

void FirstPersonScene::rightAnalog(int16 stickX, int16 stickY)
{
  camera.ProcessRightAnalog(stickX, stickY);
}

void FirstPersonScene::button_A_pressed() { camera.ProcessKeyboard(JUMP); }

void FirstPersonScene::button_A_released() {}

void FirstPersonScene::button_B_pressed()
{
  camera.MovementSpeed = SPEED * 2;
}

void FirstPersonScene::button_B_released()
{
  camera.MovementSpeed = SPEED;
}

void FirstPersonScene::button_X_pressed() {}

void FirstPersonScene::button_X_released() {}

void FirstPersonScene::button_Y_pressed() {}

void FirstPersonScene::button_Y_released() {}

void FirstPersonScene::button_dPadUp_pressed()
{
  camera.ProcessKeyboard(FORWARD);
}

void FirstPersonScene::button_dPadDown_pressed()
{
  camera.ProcessKeyboard(BACKWARD);
}

void FirstPersonScene::button_dPadLeft_pressed()
{
  camera.ProcessKeyboard(LEFT);
}

void FirstPersonScene::button_dPadRight_pressed()
{
  camera.ProcessKeyboard(RIGHT);
}

void FirstPersonScene::button_leftShoulder_pressed() {}

void FirstPersonScene::button_rightShoulder_pressed() {}

void FirstPersonScene::button_start_pressed()
{
  closeWindow();
}

void FirstPersonScene::button_select_pressed()
{
  adjustWindowSize();
}

void FirstPersonScene::button_select_released() {}
// +++ CONTROLLER CONSUMER IMPLEMETNATION - START +++

// +++ KEYBOARD CONSUMER IMPLEMENTATION - START +++
void FirstPersonScene::key_LeftShift_pressed()
{
  camera.MovementSpeed = SPEED * 2;
}

void FirstPersonScene::key_LeftShift_released()
{
  camera.MovementSpeed = SPEED;
}

void FirstPersonScene::key_W()
{
  camera.ProcessKeyboard(FORWARD);
}

void FirstPersonScene::key_S()
{
  camera.ProcessKeyboard(BACKWARD);
}

void FirstPersonScene::key_A()
{
  camera.ProcessKeyboard(LEFT);
}

void FirstPersonScene::key_D()
{
  camera.ProcessKeyboard(RIGHT);
}

void FirstPersonScene::key_Space()
{
  camera.ProcessKeyboard(JUMP);
}

void FirstPersonScene::key_LeftMouseButton_pressed()
{
  // Do nothing
}

void FirstPersonScene::key_LeftMouseButton_released()
{
  // Do nothing
}

void FirstPersonScene::key_Up()
{
  // Do nothing
}

void FirstPersonScene::key_Down()
{
  // Do nothing
}

void FirstPersonScene::key_Left()
{
  // Do nothing
}

void FirstPersonScene::key_Right()
{
  // Do nothing
}

void FirstPersonScene::key_AltEnter_pressed()
{
  adjustWindowSize();
}

void FirstPersonScene::key_AltEnter_released()
{
  // Do nothing
}
// +++ INPUT CONSUMER IMPLEMENTATION - END +++

// +++ MOUSE MOVEMENT CONSUMER IMPLEMENTATION - START +++
void FirstPersonScene::mouseMovement(float32 xOffset, float32 yOffset)
{
  camera.ProcessMouseMovement(xOffset, yOffset);
}
// +++ MOUSE MOVEMENT IMPLEMENTATION - END +++

// +++ MOUSE SCROLL CONSUMER IMPLEMENTATION - START +++
void FirstPersonScene::mouseScroll(float32 yOffset)
{
  camera.ProcessMouseScroll(yOffset);
}
// +++ MOUSE SCROLL CONSUMER IMPLEMENTATION - END +++