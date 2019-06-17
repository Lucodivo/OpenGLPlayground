#include "Input.h"

void processInput(GLFWwindow* window, InputConsumer* consumer) {
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