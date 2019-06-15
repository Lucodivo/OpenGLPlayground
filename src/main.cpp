#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "main.h"
#include "PlaygroundScene.h"

#include <iostream>

#define VIEWPORT_INIT_WIDTH 800
#define VIEWPORT_INIT_HEIGHT 600
float32 lastX = VIEWPORT_INIT_WIDTH / 2;
float32 lastY = VIEWPORT_INIT_HEIGHT / 2;

int main() {
    loadGLFW();
    GLFWwindow* window = createWindow();
    glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    initializeGLAD();

	runPlaygroundScene(window, VIEWPORT_INIT_HEIGHT, VIEWPORT_INIT_WIDTH);

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

void processInput(GLFWwindow * window) {
	double currentTime = glfwGetTime();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	local_persist bool leftShiftWasDown = false;
	if (glfwGetMouseButton(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		if (!leftShiftWasDown) {
			leftShiftWasDown = true;
			key_LeftShift_pressed();
		}
	}
	else if(leftShiftWasDown) {
		leftShiftWasDown = false; 
		key_LeftShift_released();
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		key_W();
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		key_S();
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		key_A();
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		key_D();
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		key_Space();

	local_persist bool leftMouseButtonWasDown = false;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (!leftMouseButtonWasDown) {
			leftMouseButtonWasDown = true;
			key_LeftMouseButton_pressed();
		}
	}
	else if (leftMouseButtonWasDown) {
		leftMouseButtonWasDown = false;
		key_LeftMouseButton_released();
	}

	local_persist bool windowMode = true;
	local_persist double windowModeSwitchTimer = currentTime;
	if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS &&
		currentTime - windowModeSwitchTimer > 1.5f) {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		if (windowMode) {
			glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
		}
		else {
			glfwSetWindowMonitor(window, NULL, (mode->width / 4), (mode->height / 4), VIEWPORT_INIT_WIDTH, VIEWPORT_INIT_HEIGHT, GLFW_DONT_CARE);
		}
		windowMode = !windowMode;
		windowModeSwitchTimer = currentTime;
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		key_Up();
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		key_Down();
	}
}

// Callback function for when user resizes our window
void framebuffer_size_callback(GLFWwindow * window, int width, int height) {
	glViewport(0, 0, width, height);
	initializeFrameBuffer(width, height);
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

	mouseMove(xOffset, yOffset);
}

// Callback function for when user scrolls with mouse wheel
void scroll_callback(GLFWwindow * window, double xOffset, double yOffset)
{
	mouseScroll((float32)yOffset);
}