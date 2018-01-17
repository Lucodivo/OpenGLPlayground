#pragma once

void loadGLFW();
GLFWwindow * createWindow();
void initializeGLAD();
void renderLoop(GLFWwindow * window);
GLuint loadVertexShader();
GLuint loadFragmentShader();
GLuint loadShaderProgram(GLuint vertexShader, GLuint fragmentShader);
void processInput(GLFWwindow * window);
void framebuffer_size_callback(GLFWwindow * window, int width, int height);