#pragma once

#include "Shader.h"

#include "common/WindowsFileHelper.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#define NO_SHADER 0

bool Shader::updateFragmentShaderIfOutdated() {
  uint32 lastWriteTime = getFileLastWriteTime(fragmentShaderPath);
  bool fileUpToDate = fragmentShaderFileTime == lastWriteTime;

  if(!fileUpToDate) {
    glDeleteShader(fragmentShader); // remove old fragment shader
    fragmentShader = loadShader(fragmentShaderPath, Fragment);

    glAttachShader(this->ID, vertexShader);
    glAttachShader(this->ID, fragmentShader);
    if(geometryShader != NO_SHADER) glAttachShader(this->ID, geometryShader);
    glLinkProgram(this->ID);

    int32 linkSuccess;
    glGetProgramiv(this->ID, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess)
    {
      char infoLog[512];
      glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
      std::cout << "ERROR::PROGRAM::SHADER::LINK_FAILED\n" << infoLog << std::endl;
    }

    glDetachShader(this->ID, vertexShader);
    glDetachShader(this->ID, fragmentShader);
    if (geometryShader != NO_SHADER) glDetachShader(this->ID, geometryShader);

    return true;
  }

  return false;
}

// constructor reads and builds the shader
Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
  fragmentShaderPath = fragmentPath;

  vertexShader = loadShader(vertexPath, Vertex);
  fragmentShader = loadShader(fragmentPath, Fragment);
  geometryShader = geometryPath != NULL ? loadShader(geometryPath, Geometry) : NO_SHADER;

  // shader program
  this->ID = glCreateProgram(); // NOTE: returns 0 if error occurs when creating program
  glAttachShader(this->ID, vertexShader);
  glAttachShader(this->ID, fragmentShader);
  if (geometryPath != NO_SHADER) glAttachShader(this->ID, geometryShader);
  glLinkProgram(this->ID);

  int32 linkSuccess;
  glGetProgramiv(this->ID, GL_LINK_STATUS, &linkSuccess);
  if (!linkSuccess)
  {
    char infoLog[512];
    glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
    std::cout << "ERROR::PROGRAM::SHADER::LINK_FAILED\n" << infoLog << std::endl;
  }

  glDetachShader(this->ID, vertexShader);
  glDetachShader(this->ID, fragmentShader);
  if (geometryPath != NO_SHADER) glDetachShader(this->ID, geometryShader);
}

void Shader::deleteShaderResources()
{
  // delete the shaders
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  if (geometryShader != NO_SHADER) glDeleteShader(geometryShader);
  glDeleteProgram(ID);
}

// use/activate the shader
void Shader::use()
{
  glUseProgram(this->ID);
}

// utility uniform functions
void Shader::setUniform(const std::string& name, bool value) const
{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setUniform(const std::string& name, int32 value) const
{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setUniform(const std::string& name, uint32 value) const
{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setUniform(const std::string& name, float32 value) const
{
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setUniform(const std::string& name, float32 value1, float32 value2) const
{
  glUniform2f(glGetUniformLocation(ID, name.c_str()), value1, value2);
}

void Shader::setUniform(const std::string& name, float32 value1, float32 value2, float32 value3) const
{
  glUniform3f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3);
}

void Shader::setUniform(const std::string& name, float32 value1, float32 value2, float32 value3, float32 value4) const
{
  glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4);
}

void Shader::setUniform(const std::string& name, const glm::mat4& mat) const
{
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()),
                     1, // count
                     GL_FALSE, // transpose: swap columns and rows (true or false)
                     glm::value_ptr(mat)); // pointer to float values
}

void Shader::setUniform(const std::string& name, const glm::mat4* matArray, const uint32 arraySize)
{
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()),
                     arraySize, // count
                     GL_FALSE, // transpose: swap columns and rows (true or false)
                     (GLfloat*)matArray); // pointer to float values // TODO: glm::value_ptr(mats[0])
}

void Shader::setUniform(const std::string& name, const float* floatArray, const uint32 arraySize)
{
  glUniform1fv(glGetUniformLocation(ID, name.c_str()), arraySize, floatArray);
}

void Shader::setUniform(const std::string& name, const glm::vec2& vector2)
{
  setUniform(name, vector2.x, vector2.y);
}

void Shader::setUniform(const std::string& name, const glm::vec3& vector3)
{
  setUniform(name, vector3.x, vector3.y, vector3.z);
}

void Shader::setUniform(const std::string& name, const glm::vec4& vector4)
{
  setUniform(name, vector4.x, vector4.y, vector4.z, vector4.w);
}

void Shader::bindBlockIndex(const std::string& name, uint32 index)
{
  uint32 blockIndex = glGetUniformBlockIndex(ID, name.c_str());
  glUniformBlockBinding(ID, blockIndex, index);
}

/*
 * parameters:
 * shaderType can be GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, or GL_GEOMETRY_SHADER
 */
uint32 Shader::loadShader(const char* shaderPath, ShaderType shaderType) {
  std::string shaderTypeStr;
  uint32 lastWriteTime = getFileLastWriteTime(shaderPath);
  if(shaderType == Vertex) {
    shaderTypeStr = "VERTEX";
  } else if(shaderType == Fragment){
    fragmentShaderFileTime = lastWriteTime;
    shaderTypeStr = "FRAGMENT";
  } else if(shaderType == Geometry) {
    shaderTypeStr = "GEOMETRY";
  }

  std::string shaderCode;
  readShaderCodeAsString(shaderPath, &shaderCode);
  const char* shaderCodeCStr = shaderCode.c_str();

  uint32 shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &shaderCodeCStr, NULL);
  glCompileShader(shader);

  int32 shaderSuccess;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderSuccess);
  if (shaderSuccess != GL_TRUE)
  {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::" << shaderTypeStr << "::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  return shader;
}

void Shader::readShaderCodeAsString(const char* shaderPath, std::string* shaderCode)
{
  try
  {
    std::ifstream file;
    // ensure ifstream objects can throw exceptions:
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    // open file
    file.open(shaderPath);
    std::stringstream shaderStream;
    // read file's buffer contents into streams
    shaderStream << file.rdbuf();
    // close file handler
    file.close();
    // convert stream into string
    *shaderCode = shaderStream.str();
  } catch (std::ifstream::failure e)
  {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }
}
