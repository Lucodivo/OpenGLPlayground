#pragma once

void loadGLFW();
GLFWwindow * createWindow();
void initializeGLAD();
void renderLoop(GLFWwindow * window);
void processInput(GLFWwindow * window);
void framebuffer_size_callback(GLFWwindow * window, int width, int height);