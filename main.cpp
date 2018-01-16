#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "main.h"

#define VIEWPORT_WIDTH 800
#define VIEWPORT_HEIGHT 600

int main() {
    int loadSucceeded = glfwInit();
    if (loadSucceeded == GL_FALSE) {
        std::cout << "Failed to load GLFW" << std::endl;
        return -1;
    }

    // set what kind of window desired
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL version x._
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL version _.x
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment for OSX

    // create window
    GLFWwindow* window = glfwCreateWindow(VIEWPORT_WIDTH, // int Width
                                          VIEWPORT_HEIGHT, // int Height
                                          "LearnOpenGL", // const char* Title
                                          NULL, // GLFWmonitor* Monitor: Specified for which monitor for fullscreen, NULL for windowed mode
                                          NULL); // GLFWwindow* Share: window to share resources with
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // intialize GLAD to help manage function pointers for OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, // int x: left-x of viewport rect
               0, // int y: bottom-y of viewport rect
               VIEWPORT_WIDTH, // int width
               VIEWPORT_HEIGHT); // int height

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // NOTE: render/game loop
    while (glfwWindowShouldClose(window) == GL_FALSE) {
        // check for input
        processInput(window);

        // rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   // OpenGL state-setting function
        glClear(GL_COLOR_BUFFER_BIT);           // OpenGL state-using function

        glfwSwapBuffers(window); // swaps double buffers (call after all render commands are completed)
        glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
    }

    glfwTerminate(); // clean up gl resources
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

// Callback function for when user resizes our window
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}