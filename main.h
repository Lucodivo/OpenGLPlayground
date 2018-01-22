#pragma once

void loadGLFW();
GLFWwindow * createWindow();
void initializeGLAD();
void initializeBuffers(unsigned int & VAO, unsigned int & VBO, unsigned int & EBO);
void renderLoop(GLFWwindow * window, unsigned int &VAO);
void initializeTextures(Shader & shader);
void loadTexture(const char * imgLocation, unsigned int textureOffset);
void processInput(GLFWwindow * window);
void framebuffer_size_callback(GLFWwindow * window, int width, int height);