#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "main.h"
#include "PlaygroundScene.h"

#include <iostream>

#define VIEWPORT_INIT_WIDTH 800
#define VIEWPORT_INIT_HEIGHT 600
float32 lastX = VIEWPORT_INIT_WIDTH / 2;
float32 lastY = VIEWPORT_INIT_HEIGHT / 2;

PlaygroundScene* playgroundScene;

int main() {
    loadGLFW();
    GLFWwindow* window = createWindow();
    glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    initializeGLAD();

	playgroundScene = new PlaygroundScene();
	playgroundScene->runScene(window, VIEWPORT_INIT_HEIGHT, VIEWPORT_INIT_WIDTH);

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

void initializeGLAD() {
	// intialize GLAD to help manage function pointers for OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
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
    GLFWwindow* window = glfwCreateWindow(VIEWPORT_INIT_WIDTH, // int Width
        VIEWPORT_INIT_HEIGHT, // int Height
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

// Callback function for when user resizes our window
void framebuffer_size_callback(GLFWwindow * window, int width, int height) {
	glViewport(0, 0, width, height);
	playgroundScene->initializeFrameBuffer(width, height);
}

// Callback function for when user moves mouse
void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	local_persist bool firstMouse = true;
	if (firstMouse) {
		lastX = (float32)xpos;
		lastY = (float32)ypos;
		firstMouse = false;
	}

	float32 xOffset = (float32)xpos - lastX;
	float32 yOffset = lastY - (float32)ypos; // reversed since y-coordinates go from bottom to top

	lastX = (float32)xpos;
	lastY = (float32)ypos;

	playgroundScene->mouseMove(xOffset, yOffset);
}

// Callback function for when user scrolls with mouse wheel
void scroll_callback(GLFWwindow * window, double xOffset, double yOffset)
{
	playgroundScene->mouseScroll((float32)yOffset);
}