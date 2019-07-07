#pragma once

#include <map>

#include "LearnOpenGLPlatform.h"
#include "Camera.h"
#include "Input.h"
#include "Shader.h"

struct Character
{
  GLuint     textureID;  // ID handle of the glyph texture
  glm::ivec2 size;       // Size of glyph
  glm::ivec2 bearing;    // Offset from baseline to left/top of glyph
  GLuint     advance;    // Offset to advance to next glyph
};

class Scene : public FrameBufferSizeConsumer
{
public:
  Scene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth);
  void frameBufferSize(uint32 width, uint32 height);
  virtual void runScene() = 0;

protected:
  GLFWwindow* window;
  uint32 viewportWidth;
  uint32 viewportHeight;

  void adjustWindowSize();
  void closeWindow();
  void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

private:
  std::map<GLchar, Character> Characters;
  Shader textDebugShader;
  uint32 textDebugVAO;
  uint32 textDebugVBO;
  glm::mat4 textDebugProjectionMat;

  void initDebugTextCharacters();
  void initDebugTextBuffers();
};

class FirstPersonScene : public Scene, protected KeyboardConsumer, protected ControllerConsumer, MouseMovementConsumer, MouseScrollConsumer
{
public:
  FirstPersonScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth);

  // Scene implementation // TODO: can we remove these in the header file?
  virtual void runScene() = 0;

  // ControllerConsumer implementation
  void leftAnalog(int16 stickX, int16 stickY);
  void rightAnalog(int16 stickX, int16 stickY);
  void button_A_pressed();
  void button_A_released();
  void button_B_pressed();
  void button_B_released();
  void button_X_pressed();
  void button_X_released();
  void button_Y_pressed();
  void button_Y_released();
  void button_dPadUp_pressed();
  void button_dPadDown_pressed();
  void button_dPadLeft_pressed();
  void button_dPadRight_pressed();
  void button_leftShoulder_pressed();
  void button_rightShoulder_pressed();
  void button_start_pressed();
  void button_select_pressed();
  void button_select_released();

  // KeyboardConsumer implementation
  void key_LeftShift_pressed();
  void key_LeftShift_released();
  void key_W();
  void key_S();
  void key_A();
  void key_D();
  void key_Space();
  void key_LeftMouseButton_pressed();
  void key_LeftMouseButton_released();
  void key_Up();
  void key_Down();
  void key_Left();
  void key_Right();
  void key_AltEnter_pressed();
  void key_AltEnter_released();

  // MouseMovementConsumer implementation
  void mouseMovement(float32 xOffset, float32 yOffset);

  // MouseScrollConsumer implementation
  void mouseScroll(float32 yOffset);

protected:
  Camera camera = Camera();
};