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

class FrameBufferSizeConsumer {
public:
	virtual void frameBufferSize(int width, int height) = 0;
};

void processKeyboardInput(GLFWwindow* window, KeyboardConsumer* consumer);
void subscribeMouseMovement(GLFWwindow* window, MouseMovementConsumer* consumer);
void subscribeMouseScroll(GLFWwindow* window, MouseScrollConsumer* consumer);
void subscribeFrameBufferSize(GLFWwindow* window, FrameBufferSizeConsumer* consumer);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);