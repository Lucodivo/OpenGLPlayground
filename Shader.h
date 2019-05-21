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
    Shader(const char* vertexPath, const char* fragmentPath)
	{
		// retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		// vertex Shader
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vShaderCode, NULL);
		glCompileShader(vertexShader);

		// ensure that shader loaded successfully
		int vertexSuccess;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexSuccess);
		if (vertexSuccess != GL_TRUE) {
			char infoLog[512];
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// fragment Shader
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
		glCompileShader(fragmentShader);

		// ensure that fragment shader loaded successfully
		int fragmentSuccess;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentSuccess);
		if (fragmentSuccess != GL_TRUE) {
			char infoLog[512];
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// shader program
		this->ID = glCreateProgram(); // NOTE: returns 0 if error occurs when creating program
		glAttachShader(this->ID, vertexShader);
		glAttachShader(this->ID, fragmentShader);
		glLinkProgram(this->ID);

		int linkSuccess;
		glGetProgramiv(this->ID, GL_LINK_STATUS, &linkSuccess);
		if (!linkSuccess) {
			char infoLog[512];
			glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
			std::cout << "ERROR::PROGRAM::SHADER::LINK_FAILED\n" << infoLog << std::endl;
		}

		// delete the shaders
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

    // use/activate the shader
	void use() {
		glUseProgram(this->ID);
	}

	// utility uniform functions
	void setUniform(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void setUniform(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setUniform(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setUniform(const std::string& name, float value1, float value2) const {
		glUniform2f(glGetUniformLocation(ID, name.c_str()), value1, value2);
	}

	void setUniform(const std::string& name, float value1, float value2, float value3) const {
		glUniform3f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3);
	}

	void setUniform(const std::string& name, float value1, float value2, float value3, float value4) const {
		glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4);
	}

	void setUniform(const std::string& name, const glm::mat4& trans) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()),
			1, // count 
			GL_FALSE, // transpose: swap columns and rows (true or false)
			glm::value_ptr(trans)); // pointer to float values
	}

	void setUniform(const std::string& name, const glm::vec3& vector3) {
		setUniform(name, vector3.x, vector3.y, vector3.z);
	}
};

#endif