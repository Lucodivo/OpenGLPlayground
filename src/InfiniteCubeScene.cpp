#include "InfiniteCubeScene.h"
#include "Shader.h"
#include "FileLocations.h"
#include "ObjectData.h"

InfiniteCubeScene::InfiniteCubeScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
	: FirstPersonScene(window, initScreenHeight, initScreenWidth) {}

void InfiniteCubeScene::runScene() {
	uint32 cubeVAO, cubeVBO, cubeEBO;
	initializeCubeBuffers(cubeVAO, cubeVBO, cubeEBO);

	uint32 quadVAO, quadVBO, quadEBO;
	initializeQuadBuffers(quadVAO, quadVBO, quadEBO);

	initializeFrameBuffer(frameBuffer, rbo, frameBufferTexture, viewportWidth, viewportHeight);

	renderLoop(window, cubeVAO, quadVAO);
}

void InfiniteCubeScene::frameBufferSize(uint32 width, uint32 height) {
	FirstPersonScene::frameBufferSize(width, height);
	initializeFrameBuffer(frameBuffer, rbo, frameBufferTexture, width, height);

	// bind custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	// start frame buffer with white background
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void InfiniteCubeScene::renderLoop(GLFWwindow* window, uint32& cubeVAO, uint32& quadVAO) {
	Shader cubeShader = Shader(cubeVertexShaderFileLoc, infiniteCubeFragmentShaderFileLoc);
	Shader frameBufferShader = Shader(frameBufferVertexShaderFileLoc, kernelFrameBufferFragmentShaderFileLoc);
	Shader stencilShader = Shader(cubeVertexShaderFileLoc, stencilFragmentShaderFileLoc);

	const glm::mat4 projectionMat = glm::perspective(glm::radians(camera.Zoom), (float32)viewportWidth / (float32)viewportHeight, 0.1f, 100.0f);

	const float32 cubRotAngle = 2.5f;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// bind custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	// start frame buffer with white background
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	while (glfwWindowShouldClose(window) == GL_FALSE) {
		// check for input
		processKeyboardInput(window, this);

		// bind default frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, // when stencil fails
			GL_KEEP, // when stencil passes but depth fails
			GL_REPLACE); // when stencil passes and depth passes
		glStencilMask(0xFF); // mask that is ANDed with the stencil value that is about to be written to stencil buffer
		glStencilFunc(GL_ALWAYS, // stencil function
			1, // reference value for stencil test
			0xFF); // mask that is ANDed with stencil value and reference value before the test compares them

		// set background color
		glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		float32 t = (float32)glfwGetTime();
		deltaTime = t - lastFrame;
		lastFrame = t;

		glm::mat4 viewMat = camera.GetViewMatrix(deltaTime);

		// draw cube
		cubeShader.use();

		cubeShader.setUniform("ambientLightColor", glm::vec3(1.0f));
		glBindTexture(GL_TEXTURE_2D, frameBufferTexture);

		// rotate with time
		glm::mat4 cubeModelMatrix = glm::mat4();
		cubeModelMatrix = glm::rotate(cubeModelMatrix, t * glm::radians(cubRotAngle), glm::vec3(1.0f, 0.3f, 0.5f));

		cubeShader.setUniform("model", cubeModelMatrix);
		cubeShader.setUniform("view", viewMat);
		cubeShader.setUniform("projection", projectionMat);

		glBindVertexArray(cubeVAO);
		glDrawElements(GL_TRIANGLES,
			cubeNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
			GL_UNSIGNED_INT,
			0);

		// draw stencil
		stencilShader.use();
		glm::mat4 stencilModelMatrix = glm::scale(cubeModelMatrix, glm::vec3(1.05f));
		stencilShader.setUniform("singleColor", glm::vec3(0.0f, 0.0f, 0.0f));
		stencilShader.setUniform("projection", projectionMat);
		stencilShader.setUniform("view", viewMat);
		stencilShader.setUniform("model", stencilModelMatrix);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		glBindVertexArray(cubeVAO);
		glDrawElements(GL_TRIANGLES,
			cubeNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
			GL_UNSIGNED_INT,
			0);

		glfwSwapBuffers(window); // swaps double buffers (call after all render commands are completed)
		glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
	}
}