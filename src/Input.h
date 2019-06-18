#pragma once

#include <GLFW/glfw3.h>

#include "LearnOpenGlPlatform.h"

class KeyboardConsumer {
public:
	virtual void key_LeftShift_pressed() = 0;
	virtual void key_LeftShift_released() = 0;
	virtual void key_W() = 0;
	virtual void key_S() = 0;
	virtual void key_A() = 0;
	virtual void key_D() = 0;
	virtual void key_Space() = 0;
	virtual void key_LeftMouseButton_pressed() = 0;
	virtual void key_LeftMouseButton_released() = 0;
	virtual void key_Up() = 0;
	virtual void key_Down() = 0;
	virtual void key_AltEnter_pressed() = 0;
	virtual void key_AltEnter_released() = 0;
};

class MouseMovementConsumer {
public:
	virtual void mouseMovement(float32 xOffset, float32 yOffset) = 0;
};

class MouseScrollConsumer {
public:
	virtual void mouseScroll(float32 yOffset) = 0;
};

void processInput(GLFWwindow* window, KeyboardConsumer* consumer);
void subscribeMouseMovement(GLFWwindow* window, MouseMovementConsumer* movementCallback);
void subscribeMouseScroll(GLFWwindow* window, MouseScrollConsumer* scrollCallback);