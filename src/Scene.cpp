#include "Scene.h"

Scene::Scene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
	: window(window), viewportHeight(initScreenHeight), viewportWidth(initScreenWidth) {}

FirstPersonScene::FirstPersonScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
	: Scene(window, initScreenHeight, initScreenWidth) {
	subscribeMouseMovement(window, this);
	subscribeMouseScroll(window, this);
}

// +++ INPUT CONSUMER IMPLEMENTATION - START +++
void FirstPersonScene::key_LeftShift_pressed() {
	camera.MovementSpeed = SPEED * 2;
}

void FirstPersonScene::key_LeftShift_released() {
	camera.MovementSpeed = SPEED;
}

void FirstPersonScene::key_W() {
	camera.ProcessKeyboard(FORWARD);
}

void FirstPersonScene::key_S() {
	camera.ProcessKeyboard(BACKWARD);
}

void FirstPersonScene::key_A() {
	camera.ProcessKeyboard(LEFT);
}

void FirstPersonScene::key_D() {
	camera.ProcessKeyboard(RIGHT);
}

void FirstPersonScene::key_Space() {
	camera.ProcessKeyboard(JUMP);
}

void FirstPersonScene::key_LeftMouseButton_pressed() {
	// Do nothing
}

void FirstPersonScene::key_LeftMouseButton_released() {
	// Do nothing
}

void FirstPersonScene::key_Up() {
	// Do nothing
}

void FirstPersonScene::key_Down() {
	// Do nothing
}

void FirstPersonScene::key_AltEnter_pressed() {
	local_persist bool windowMode = true;
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	if (windowMode) {
		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
	}
	else {
		glfwSetWindowMonitor(window, NULL, (mode->width / 4), (mode->height / 4), VIEWPORT_INIT_WIDTH, VIEWPORT_INIT_HEIGHT, GLFW_DONT_CARE);
	}
	windowMode = !windowMode;
}

void FirstPersonScene::key_AltEnter_released() {
	// Do nothing
}
// +++ INPUT CONSUMER IMPLEMENTATION - END +++

// +++ MOUSE MOVEMENT CONSUMER IMPLEMENTATION - START +++
void FirstPersonScene::mouseMovement(float32 xOffset, float32 yOffset) {
	camera.ProcessMouseMovement(xOffset, yOffset);
}
// +++ MOUSE MOVEMENT IMPLEMENTATION - END +++

// +++ MOUSE SCROLL CONSUMER IMPLEMENTATION - START +++
void FirstPersonScene::mouseScroll(float32 yOffset) {
	camera.ProcessMouseScroll(yOffset);
}
// +++ MOUSE SCROLL CONSUMER IMPLEMENTATION - END +++