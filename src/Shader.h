#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

#include "LearnOpenGLPlatform.h"

enum ShaderTypeFlags {
  VertexShaderFlag = 1 << 0,
  FragmentShaderFlag = 1 << 1,
  GeometryShaderFlag = 1 << 2
};

// TODO: Convert to a simple structure?
class Shader
{
public:
  // the program ID
  uint32 ID;

  // constructor reads and builds the shader
  Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = NULL);

  // This function takes in a bit flag of ShaderType enums
  // Returns true if shader was outdated
  // NOTE: This will require you to resupply any uniforms that aren't supplied in render loop
  bool updateShadersWhenOutdated(uint32 shaderTypeFlag);

  // use/activate the shader
  void use();

  // clean up resources when unneeded
  void deleteShaderResources();

  // utility uniform functions
  void setUniform(const std::string& name, bool value) const;
  void setUniform(const std::string& name, int32 value) const;
  void setUniform(const std::string& name, uint32 value) const;
  void setUniform(const std::string& name, float32 value) const;
  void setUniform(const std::string& name, float32 value1, float32 value2) const;
  void setUniform(const std::string& name, float32 value1, float32 value2, float32 value3) const;
  void setUniform(const std::string& name, float32 value1, float32 value2, float32 value3, float32 value4) const;
  void setUniform(const std::string& name, const glm::mat4& mat) const;
  void setUniform(const std::string& name, const glm::mat4* matArray, uint32 arraySize);
  void setUniform(const std::string& name, const float* floatArray, uint32 arraySize);
  void setUniform(const std::string& name, const glm::vec2& vector2);
  void setUniform(const std::string& name, const glm::vec3& vector3);
  void setUniform(const std::string& name, const glm::vec4& vector4);
  void bindBlockIndex(const std::string& name, uint32 index);

private:

  uint32 vertexShaderFileTime;
  GLuint vertexShader;
  const char* vertexShaderPath;

  uint32 fragmentShaderFileTime;
  GLuint fragmentShader;
  const char* fragmentShaderPath;

  uint32 geometryShaderFileTime;
  GLuint geometryShader;
  const char* geometryShaderPath;

  uint32 loadShader(const char* shaderPath, GLenum shaderType);
  bool updateShaderWhenOutdated(GLuint* shaderId, const char* shaderFileLocation, uint32* lastUpdated, GLenum shaderType);
  void readShaderCodeAsString(const char* shaderPath, std::string* shaderCode);
};