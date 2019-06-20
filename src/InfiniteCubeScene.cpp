#include "InfiniteCubeScene.h"
#include "FileLocations.h"
#include "ObjectData.h"
#include "Util.h"

InfiniteCubeScene::InfiniteCubeScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
	: FirstPersonScene(window, initScreenHeight, initScreenWidth), 
	cubeShader(cubeVertexShaderFileLoc, infiniteCubeFragmentShaderFileLoc),
	cubeOutlineShader(cubeVertexShaderFileLoc, discardAlphaFragmentShaderFileLoc),
	frameBufferShader(frameBufferVertexShaderFileLoc, frameBufferFragmentShaderFileLoc)  {}

void InfiniteCubeScene::runScene() {
	uint32 cubeVAO, cubeVBO, cubeEBO;
	initializeCubeBuffers(cubeVAO, cubeVBO, cubeEBO);

	uint32 quadVAO, quadVBO, quadEBO;
	initializeQuadBuffers(quadVAO, quadVBO, quadEBO);

	initializeFrameBuffer(frameBuffers[0].frameBuffer, frameBuffers[0].rbo, frameBuffers[0].frameBufferTexture, viewportWidth, viewportHeight);
	initializeFrameBuffer(frameBuffers[1].frameBuffer, frameBuffers[1].rbo, frameBuffers[1].frameBufferTexture, viewportWidth, viewportHeight);

	renderLoop(window, cubeVAO, quadVAO);
}

void InfiniteCubeScene::frameBufferSize(uint32 width, uint32 height) {
	FirstPersonScene::frameBufferSize(width, height);
	initializeFrameBuffer(frameBuffers[0].frameBuffer, frameBuffers[0].rbo, frameBuffers[0].frameBufferTexture, width, height);
	initializeFrameBuffer(frameBuffers[1].frameBuffer, frameBuffers[1].rbo, frameBuffers[1].frameBufferTexture, width, height);

	// start frame buffer with white background
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// bind custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[0].frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// bind custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[1].frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frameBuffers[0].frameBufferTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, frameBuffers[1].frameBufferTexture);
	glActiveTexture(GL_TEXTURE0);

	cubeShader.use();
	cubeShader.setUniform("texWidth", (float32)viewportWidth);
	cubeShader.setUniform("texHeight", (float32)viewportHeight);
}

void InfiniteCubeScene::renderLoop(GLFWwindow* window, uint32& cubeVAO, uint32& quadVAO) {
	uint32 outlineTexture;
	loadTexture(outlineTextureLoc, outlineTexture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frameBuffers[0].frameBufferTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, frameBuffers[1].frameBufferTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, outlineTexture);
	glActiveTexture(GL_TEXTURE0);

	// start frame buffer with white background
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// bind custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[0].frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// bind custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[1].frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	const glm::mat4 projectionMat = glm::perspective(glm::radians(camera.Zoom), (float32)viewportWidth / (float32)viewportHeight, 0.1f, 100.0f);

	const float32 cubRotAngle = 2.5f;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	uint32 colorIndex = 0;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frameBuffers[0].frameBufferTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, frameBuffers[1].frameBufferTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, outlineTexture);
	glActiveTexture(GL_TEXTURE0);

#if 0
	// draw in wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	uint32 currentFrameBufferIndex = 0;
	uint32 previousFrameBufferIndex = 1;
	uint32 outlineTextureIndex = 2;

	// set constant uniforms
	cubeShader.use();
	cubeShader.setUniform("projection", projectionMat);
	cubeShader.setUniform("texWidth", (float32)viewportWidth);
	cubeShader.setUniform("texHeight", (float32)viewportHeight);

	cubeOutlineShader.use();
	cubeOutlineShader.setUniform("projection", projectionMat);
	cubeOutlineShader.setUniform("diffTexture", outlineTextureIndex);

	while (glfwWindowShouldClose(window) == GL_FALSE) {
		// check for input
		processKeyboardInput(window, this);

		previousFrameBufferIndex = currentFrameBufferIndex;
		currentFrameBufferIndex = currentFrameBufferIndex == 0 ? 1 : 0;

		// bind default frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[currentFrameBufferIndex].frameBuffer);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		float32 t = (float32)glfwGetTime();
		deltaTime = t - lastFrame;
		lastFrame = t;

		local_persist float32 elapsedTime = 0;
		elapsedTime += deltaTime;
		if (elapsedTime > 0.50f) {
			elapsedTime = 0;

			// set background color
			colorIndex = (colorIndex + 1) % ArrayCount(colors);
			glClearColor(colors[colorIndex].x, colors[colorIndex].y, colors[colorIndex].z, 1.0f);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glm::mat4 viewMat = camera.GetViewMatrix(deltaTime);

		// draw cube
		cubeOutlineShader.use();
		glBindVertexArray(cubeVAO);

		// rotate with time
		glm::mat4 cubeModelMatrix = glm::mat4();
		cubeModelMatrix = glm::rotate(cubeModelMatrix, t * glm::radians(cubRotAngle), glm::vec3(1.0f, 0.3f, 0.5f));

		cubeOutlineShader.setUniform("model", cubeModelMatrix);
		cubeOutlineShader.setUniform("view", viewMat);

		glDrawElements(GL_TRIANGLES,
			cubeNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
			GL_UNSIGNED_INT,
			0);
		glBindVertexArray(0);

		cubeShader.use();
		glBindVertexArray(cubeVAO);
		cubeShader.setUniform("model", cubeModelMatrix);
		cubeShader.setUniform("view", viewMat);
		cubeShader.setUniform("diffTexture", previousFrameBufferIndex);
		glDrawElements(GL_TRIANGLES,
			cubeNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
			GL_UNSIGNED_INT,
			0);
		glBindVertexArray(0);

		// draw scene to quad
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		frameBufferShader.use();
		glBindVertexArray(quadVAO);
		frameBufferShader.setUniform("screenTexture", currentFrameBufferIndex);
		glDrawElements(GL_TRIANGLES, // drawing mode
			6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
			GL_UNSIGNED_INT, // type of the indices
			0); // offset in the EBO
		glBindVertexArray(0);

		glfwSwapBuffers(window); // swaps double buffers (call after all render commands are completed)
		glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
	}
}