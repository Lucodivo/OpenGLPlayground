#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "FileHelper.h"
#include "main.h"

#define VIEWPORT_WIDTH 800
#define VIEWPORT_HEIGHT 600

const char *vertexShaderFile = "VertexShader.glsl";
const char *fragmentShaderFile = "FragmentShader.glsl";

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
};

int main() {
    loadGLFW();
    GLFWwindow* window = createWindow();
    glfwMakeContextCurrent(window);

    initializeGLAD();

    glViewport(0, // int x: left-x of viewport rect
               0, // int y: bottom-y of viewport rect
               VIEWPORT_WIDTH, // int width
               VIEWPORT_HEIGHT); // int height

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    renderLoop(window);

    glfwTerminate(); // clean up gl resources
    return 0;
}

void loadGLFW() {
    int loadSucceeded = glfwInit();
    if (loadSucceeded == GLFW_FALSE) {
        std::cout << "Failed to load GLFW" << std::endl;
        exit(-1);
    }
}

GLFWwindow* createWindow() {
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
        exit(-1);
    }

    return window;
}

void initializeGLAD() {
    // intialize GLAD to help manage function pointers for OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
}

void renderLoop(GLFWwindow *window) {
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1,     // Num objects to generate 
                 &VBO);  // Out parameters to store IDs of gen objects
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind object to array buffer
    glBufferData(GL_ARRAY_BUFFER, // which buffer data is being entered in
                sizeof(vertices), // size of data being placed in array buffer
                vertices,        // data to store in array buffer       
                GL_STATIC_DRAW); // GL_STATIC_DRAW (most likely not change), GL_DYNAMIC_DRAW (likely to change), GL_STREAM_DRAW (changes every time drawn)

    GLuint vertexShader = loadVertexShader();
    GLuint fragmentShader = loadFragmentShader();
    GLuint shaderProgram = loadShaderProgram(vertexShader, fragmentShader);
    // Once we've linked the shaders into the program object, we no longer need them
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glVertexAttribPointer(0, // position vertex attribute (used for location = 0 of Vertex Shader false) 
                          3, // size of vertex attribute (we're using vec3)
                          GL_FLOAT, // type of data being passed 
                          GL_FALSE, // whether the data needs to be normalized
                          3 * sizeof(float), // stride: space between consecutive vertex attribute sets
                          (void*)0); // offset of where the data starts in the array
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // NOTE: render/game loop
    while (glfwWindowShouldClose(window) == GL_FALSE) {
        // check for input
        processInput(window);

        // == RENDERING COMMANDS ==
        // clear the background
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   // OpenGL state-setting function
        glClear(GL_COLOR_BUFFER_BIT);           // OpenGL state-using function

        // User fragment shaders to draw a triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, // drawing mode: type of primitive to render
            0, // starting index in the enabled arrays
            3); // number of vertices to render

        glfwSwapBuffers(window); // swaps double buffers (call after all render commands are completed)
        glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
    }
}

// load/compile vertex shader from file
GLuint loadVertexShader() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string vertexShaderString = readFile(vertexShaderFile);
    const char *vertexShaderSource = vertexShaderString.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // ensure that shader loaded successfully
    GLint vertexSuccess;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexSuccess);
    if (vertexSuccess != GL_TRUE) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return vertexShader;
}

// load/compile fragment shader from file
GLuint loadFragmentShader() {
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragmentShaderString = readFile(fragmentShaderFile);
    const char *fragmentShaderSource = fragmentShaderString.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // ensure that fragment shader loaded successfully
    GLint fragmentSuccess;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentSuccess);
    if (fragmentSuccess != GL_TRUE) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return fragmentShader;
}

GLuint loadShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint shaderProgram = glCreateProgram(); // NOTE: returns 0 if error occurs when creating program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint linkSuccess;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::SHADER::LINK_FAILED\n" << infoLog << std::endl;
    }

    return shaderProgram;
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