#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    void use();
    // utility uniform functions
    void setUniform(const std::string &name, bool value) const;
    
    void setUniform(const std::string &name, int value) const;
    
    void setUniform(const std::string &name, float value) const;
    void setUniform(const std::string &name, float value1, float value2) const;
    void setUniform(const std::string &name, float value1, float value2, float value3) const;
    void setUniform(const std::string &name, float value1, float value2, float value3, float value4) const;
    void setUniform(const std::string &name, const glm::mat4 &trans) const;
};

#endif