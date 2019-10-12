#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "LearnOpenGLPlatform.h"

class Shader
{
public:
  // the program ID
  uint32 ID;

  // constructor reads and builds the shader
  Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = NULL)
  {

    std::string vertexCode;
    readShaderCodeAsString(vertexPath, &vertexCode);
    const char* vShaderCode = vertexCode.c_str();

    std::string fragmentCode;
    readShaderCodeAsString(fragmentPath, &fragmentCode);
    const char* fShaderCode = fragmentCode.c_str();

    // vertex Shader
    uint32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);

    // ensure that shader loaded successfully
    int32 vertexSuccess;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexSuccess);
    if (vertexSuccess != GL_TRUE)
    {
      char infoLog[512];
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment Shader
    uint32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);

    // ensure that fragment shader loaded successfully
    int32 fragmentSuccess;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentSuccess);
    if (fragmentSuccess != GL_TRUE)
    {
      char infoLog[512];
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    uint32 geometryShader;
    int32 geometrySuccess = GL_FALSE;
    if (geometryPath != NULL)
    {
      std::string geometryCode;
      readShaderCodeAsString(geometryPath, &geometryCode);
      const char* gShaderCode = geometryCode.c_str();

      // fragment Shader
      geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
      glShaderSource(geometryShader, 1, &gShaderCode, NULL);
      glCompileShader(geometryShader);

      // ensure that geometry shader loaded successfully
      glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &geometrySuccess);
      if (geometrySuccess != GL_TRUE)
      {
        char infoLog[512];
        glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
      }
    }


    // shader program
    this->ID = glCreateProgram(); // NOTE: returns 0 if error occurs when creating program
    glAttachShader(this->ID, vertexShader);
    glAttachShader(this->ID, fragmentShader);
    if (geometrySuccess == GL_TRUE) glAttachShader(this->ID, geometryShader);
    glLinkProgram(this->ID);

    int32 linkSuccess;
    glGetProgramiv(this->ID, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess)
    {
      char infoLog[512];
      glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
      std::cout << "ERROR::PROGRAM::SHADER::LINK_FAILED\n" << infoLog << std::endl;
    }

    // delete the shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if (geometrySuccess == GL_TRUE) glDeleteShader(geometryShader);
  }

  // use/activate the shader
  void use()
  {
    glUseProgram(this->ID);
  }

  // utility uniform functions
  void setUniform(const std::string & name, bool value) const
  {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
  }

  void setUniform(const std::string & name, int32 value) const
  {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  }

  void setUniform(const std::string & name, uint32 value) const
  {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  }

  void setUniform(const std::string & name, float32 value) const
  {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
  }

  void setUniform(const std::string & name, float32 value1, float32 value2) const
  {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), value1, value2);
  }

  void setUniform(const std::string & name, float32 value1, float32 value2, float32 value3) const
  {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3);
  }

  void setUniform(const std::string & name, float32 value1, float32 value2, float32 value3, float32 value4) const
  {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4);
  }

  void setUniform(const std::string & name, const glm::mat4 & mat) const
  {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()),
      1, // count 
      GL_FALSE, // transpose: swap columns and rows (true or false)
      glm::value_ptr(mat)); // pointer to float values
  }

  void setUniform(const std::string& name, const glm::mat4* matArray, const uint32 arraySize)
  {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()),
                       arraySize, // count 
                       GL_FALSE, // transpose: swap columns and rows (true or false)
                       (GLfloat*)matArray); // pointer to float values // TODO: glm::value_ptr(mats[0])
  }

  void setUniform(const std::string & name, const float* floatArray, const uint32 arraySize)
  {
    glUniform1fv(glGetUniformLocation(ID, name.c_str()), arraySize, floatArray);
  }

  void setUniform(const std::string & name, const glm::vec3 & vector3)
  {
    setUniform(name, vector3.x, vector3.y, vector3.z);
  }

  void bindBlockIndex(const std::string & name, uint32 index)
  {
    uint32 blockIndex = glGetUniformBlockIndex(ID, name.c_str());
    glUniformBlockBinding(ID, blockIndex, index);
  }

private:
    void readShaderCodeAsString(const char* shaderPath, std::string * shaderCode) {
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

};