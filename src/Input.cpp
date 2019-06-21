#include "Input.h"

MouseMovementConsumer* movementConsumer;
MouseScrollConsumer* scrollConsumer;
FrameBufferSizeConsumer* frameBufferConsumer;

float32 lastX = VIEWPORT_INIT_WIDTH / 2;
float32 lastY = VIEWPORT_INIT_HEIGHT / 2;

void processKeyboardInput(GLFWwindow* window, KeyboardConsumer* consumer) {
	double currentTime = glfwGetTime();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	local_persist bool leftShiftWasDown = false;
	if (glfwGetMouseButton(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		if (!leftShiftWasDown) {
			leftShiftWasDown = true;
			consumer->key_LeftShift_pressed();
		}
	}
	else if (leftShiftWasDown) {
		leftShiftWasDown = false;
		consumer->key_LeftShift_released();
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		consumer->key_W();
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		consumer->key_S();
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		consumer->key_A();
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		consumer->key_D();
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		consumer->key_Space();

	local_persist bool leftMouseButtonWasDown = false;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (!leftMouseButtonWasDown) {
			leftMouseButtonWasDown = true;
			consumer->key_LeftMouseButton_pressed();
		}
	}
	else if (leftMouseButtonWasDown) {
		leftMouseButtonWasDown = false;
		consumer->key_LeftMouseButton_released();
	}

	local_persist bool altEnterWasDown = false;
	if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		if (!altEnterWasDown) {
			altEnterWasDown = true;
			consumer->key_AltEnter_pressed();
		}
	}
	else if (altEnterWasDown) {
		altEnterWasDown = false;
		consumer->key_AltEnter_released();
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		consumer->key_Up();
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		consumer->key_Down();
	}
}

void subscribeMouseMovement(GLFWwindow* window, MouseMovementConsumer* consumer) {
	movementConsumer = consumer;
	glfwSetCursorPosCallback(window, mouse_callback);
}

void subscribeMouseScroll(GLFWwindow* window, MouseScrollConsumer* consumer) {
	scrollConsumer = consumer;
	glfwSetScrollCallback(window, scroll_callback);
}

void subscribeFrameBufferSize(GLFWwindow* window, FrameBufferSizeConsumer* consumer) {
	frameBufferConsumer = consumer;
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

// Callback function for when user moves mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
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

	movementConsumer->mouseMovement(xOffset, yOffset);
}

// Callback function for when user scrolls with mouse wheel
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	scrollConsumer->mouseScroll((float32)yOffset);
}

// Callback for when screen changes size
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	frameBufferConsumer->frameBufferSize((uint32)width, (uint32)height);
}