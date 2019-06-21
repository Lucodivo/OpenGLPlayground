#pragma once

#include "LearnOpenGLPlatform.h"
#include "Camera.h"
#include "Input.h"

class Scene : public FrameBufferSizeConsumer {
public:
	Scene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth);
	void frameBufferSize(uint32 width, uint32 height);
	virtual void runScene() = 0;

protected:
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	GLFWwindow* window;
	uint32 viewportWidth;
	uint32 viewportHeight;
};

class FirstPersonScene : public Scene, protected KeyboardConsumer, MouseMovementConsumer, MouseScrollConsumer {
public:
	FirstPersonScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth);

	// Scene implementation // TODO: can we remove these in the header file?
	virtual void runScene() = 0;

	// InputConsumer implementation
	void key_LeftShift_pressed();
	void key_LeftShift_released();
	void key_W();
	void key_S();
	void key_A();
	void key_D();
	void key_Space();
	void key_LeftMouseButton_pressed();
	void key_LeftMouseButton_released();
	void key_Up();
	void key_Down();
	void key_AltEnter_pressed();
	void key_AltEnter_released();

	// MouseMovementConsumer implementation
	void mouseMovement(float32 xOffset, float32 yOffset);

	// MouseScrollConsumer implementation
	void mouseScroll(float32 yOffset);

protected:
	Camera camera = Camera();
};