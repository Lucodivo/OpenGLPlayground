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
}

void InfiniteCubeScene::renderLoop(GLFWwindow* window, uint32& cubeVAO, uint32& quadVAO) {
	Shader cubeShader = Shader(cubeVertexShaderFileLoc, infiniteCubeFragmentShaderFileLoc);
	Shader frameBufferShader = Shader(frameBufferVertexShaderFileLoc, frameBufferFragmentShaderFileLoc);
	Shader stencilShader = Shader(cubeVertexShaderFileLoc, stencilFragmentShaderFileLoc);

	const glm::mat4 projectionMat = glm::perspective(glm::radians(camera.Zoom), (float32)viewportWidth / (float32)viewportHeight, 0.1f, 100.0f);

	const float32 cubRotAngle = 2.5f;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// start frame buffer with white background
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// bind custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[0].frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// bind custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[1].frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glm::vec3 colors[] = {
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.75f, 0.25f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.0f),
		glm::vec3(0.25f, 0.75f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.75f, 0.25f),
		glm::vec3(0.0f, 0.5f, 0.5f),
		glm::vec3(0.0f, 0.25f, 0.75f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.25f, 0.0f, 0.75f),
		glm::vec3(0.5f, 0.0f, 0.5f),
		glm::vec3(0.75f, 0.0f, 0.25f)
	};

	uint32 colorIndex = 0;

#if 0
	// draw in wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	uint32 currentFrameBufferIndex = 0;
	uint32 previousFrameBufferIndex = 1;

	while (glfwWindowShouldClose(window) == GL_FALSE) {
		// check for input
		processKeyboardInput(window, this);

		previousFrameBufferIndex = currentFrameBufferIndex;
		currentFrameBufferIndex = currentFrameBufferIndex == 0 ? 1 : 0;

		// bind default frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[currentFrameBufferIndex].frameBuffer);

		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, // when stencil fails
			GL_KEEP, // when stencil passes but depth fails
			GL_REPLACE); // when stencil passes and depth passes
		glStencilMask(0xFF); // mask that is ANDed with the stencil value that is about to be written to stencil buffer
		glStencilFunc(GL_ALWAYS, // stencil function
			1, // reference value for stencil test
			0xFF); // mask that is ANDed with stencil value and reference value before the test compares them

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		float32 t = (float32)glfwGetTime();
		deltaTime = t - lastFrame;
		lastFrame = t;

		local_persist float32 elapsedTime = 0;
		elapsedTime += deltaTime;
		if (elapsedTime > 0.10f) {
			elapsedTime = 0;
			// set background color
			colorIndex = (colorIndex + 1) % ArrayCount(colors);
			glClearColor(colors[colorIndex].x, colors[colorIndex].y, colors[colorIndex].z, 1.0f);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glm::mat4 viewMat = camera.GetViewMatrix(deltaTime);

		// draw cube
		cubeShader.use();
		glBindVertexArray(cubeVAO);

		cubeShader.setUniform("ambientLightColor", glm::vec3(1.0f));
		glBindTexture(GL_TEXTURE_2D, frameBuffers[previousFrameBufferIndex].frameBufferTexture);

		// rotate with time
		glm::mat4 cubeModelMatrix = glm::mat4();
		//cubeModelMatrix = glm::rotate(cubeModelMatrix, t * glm::radians(cubRotAngle), glm::vec3(1.0f, 0.3f, 0.5f));

		cubeShader.setUniform("model", cubeModelMatrix);
		cubeShader.setUniform("view", viewMat);
		cubeShader.setUniform("projection", projectionMat);

		glDrawElements(GL_TRIANGLES,
			cubeNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
			GL_UNSIGNED_INT,
			0);
		glBindVertexArray(0);

		// draw stencil
		stencilShader.use();
		glBindVertexArray(cubeVAO);
		glm::mat4 stencilModelMatrix = glm::scale(cubeModelMatrix, glm::vec3(1.05f));
		stencilShader.setUniform("singleColor", glm::vec3(0.0f, 0.0f, 0.0f));
		stencilShader.setUniform("projection", projectionMat);
		stencilShader.setUniform("view", viewMat);
		stencilShader.setUniform("model", stencilModelMatrix);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		glDrawElements(GL_TRIANGLES,
			cubeNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
			GL_UNSIGNED_INT,
			0);
		glEnable(GL_DEPTH_TEST);
		glBindVertexArray(0);

		// draw scene to quad
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		frameBufferShader.use();
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, frameBuffers[currentFrameBufferIndex].frameBufferTexture);
		glDrawElements(GL_TRIANGLES, // drawing mode
			6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
			GL_UNSIGNED_INT, // type of the indices
			0); // offset in the EBO
		glBindVertexArray(0);

		glfwSwapBuffers(window); // swaps double buffers (call after all render commands are completed)
		glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
	}
}