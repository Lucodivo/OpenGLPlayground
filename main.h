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


// ===== cube values =====
const unsigned int cubeVertexAttSize = 5;
const unsigned int cubeNumElements = 12;
float cubeVertices[] = {
    // positions          // texture positions
    // face #1
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    // face #2
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    // face #3
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    // face #4
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    // face #5
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    // face #6
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
// ===== cube values =====

// ===== double triangle values =====
const unsigned int triangleVertexAttSize = 8;
const unsigned int triangleNumElements = 2;
float triangleVertices[] = {
    // First triangle
    // positions            // colors           // texture coords
    -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
    0.5f, -0.5f, 0.0f,      0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // bottom right
    0.0f, 0.5f, 0.0f,       0.0f, 1.0f, 0.0f,   0.5, 1.0f,  // top (for first triangle) / bottom (for second triangle)
                                                            // Second triangle
                                                            -0.25f, 0.75f, 0.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // top left
                                                            0.25f, 0.75f, 0.0f,     1.0f, 0.0f, 0.0f,   1.0f, 0.0f  // top right
};
unsigned int indices[]{
    0, 1, 2,    // first triangle
    2, 3, 4     // second triangle
};
// ===== double triangle values =====