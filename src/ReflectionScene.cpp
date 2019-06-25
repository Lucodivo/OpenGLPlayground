#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <glm/gtc/matrix_transform.hpp>

#include "Model.h"
#include "FileLocations.h"
#include "Util.h"

#include "ReflectionScene.h"

const glm::vec3 cubePositions[] = {
	glm::vec3(1.5f, 0.0f,  1.5f),
	glm::vec3(1.5f, 0.0f, -1.5f),
	glm::vec3(-1.5f, 0.0f, 1.5f),
	glm::vec3(-1.5f, 0.0f, -1.5f)
};

ReflectionScene::ReflectionScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
	: FirstPersonScene(window, initScreenHeight, initScreenWidth),
	cubeShader(skyboxReflectionVertexShaderFileLoc, skyboxReflectionFragmentShaderFileLoc),
	modelShader(posTexNormalVertexShader, modelFragmentShaderFileLoc),
	skyboxShader(skyboxVertexShaderFileLoc, skyboxFragmentShaderFileLoc) {}

void ReflectionScene::runScene() {

	uint32 cubeVAO, cubeVBO, cubeEBO;
	initializeCubePosNormAttBuffers(cubeVAO, cubeVBO, cubeEBO);

	uint32 skyboxVAO, skyboxVBO, skyboxEBO;
	initializeCubePositionAttBuffers(skyboxVAO, skyboxVBO, skyboxEBO);

	renderLoop(window, cubeVAO, skyboxVAO);

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &cubeEBO);

	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	glDeleteBuffers(1, &skyboxEBO);
}

void ReflectionScene::renderLoop(GLFWwindow* window, uint32& cubeVAO, uint32& skyboxVAO) {

	uint32 skyboxTextureId;
	initializeTextures(skyboxTextureId);

	// load models
	Model nanoSuitModel((char*)nanoSuitModelAbsoluteLoc);

	const glm::mat4 projectionMat = glm::perspective(glm::radians(camera.Zoom), (float32)viewportWidth / (float32)viewportHeight, 0.1f, 100.0f);

	const float32 cubRotAngle = 7.3f;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// background clear color
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

#if 0
	// draw in wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	cubeShader.use();
	cubeShader.setUniform("projection", projectionMat);
	cubeShader.setUniform("singleColor", glm::vec3(0.5f, 0.0f, 0.0f));

	//modelShader.use();
	//setConstantLightUniforms(modelShader);

	skyboxShader.use();
	skyboxShader.setUniform("projection", projectionMat);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// NOTE: render/game loop
	while (glfwWindowShouldClose(window) == GL_FALSE) {

		// check for input
		processKeyboardInput(window, this);
		processXInput(this);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float32 t = (float32)glfwGetTime();
		deltaTime = t - lastFrame;
		lastFrame = t;

		glm::mat4 viewMat = camera.GetViewMatrix(deltaTime);

		// draw cube
		cubeShader.use();

		glBindVertexArray(cubeVAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureId);
		cubeShader.setUniform("skybox", 0);
		cubeShader.setUniform("cameraPos", camera.Position);
		cubeShader.setUniform("view", viewMat);

		float32 angularSpeed = 7.3f;

		for (int i = 0; i < ArrayCount(cubePositions); i++) {
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, t * glm::radians(angularSpeed), glm::vec3(1.0f, 0.3f, 0.5f)); // rotate with time
			
			cubeShader.setUniform("model", model);
			glDrawElements(GL_TRIANGLES, // drawing mode
				cubePosTexNormNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
				GL_UNSIGNED_INT, // type of the indices
				0); // offset in the EB
		}

		glBindVertexArray(0);

		// draw skybox
		skyboxShader.use();

		glBindVertexArray(skyboxVAO);

		skyboxShader.setUniform("skybox", 0);

		glm::mat4 viewMinusTranslation = glm::mat4(glm::mat3(viewMat));
		skyboxShader.setUniform("view", viewMinusTranslation);

		glDrawElements(GL_TRIANGLES, // drawing mode
			36, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
			GL_UNSIGNED_INT, // type of the indices
			0); // offset in the EBO
		glBindVertexArray(0);

		// draw models
		float32 modelScale = 0.2f;
		{
			// Drawing the model
			cubeShader.use();

			cubeShader.setUniform("cameraPos", camera.Position);
			cubeShader.setUniform("view", viewMat);

			glm::mat4 model;
			model = glm::scale(model, glm::vec3(modelScale));	// it's a bit too big for our scene, so scale it down
			model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
			cubeShader.setUniform("model", model);
			nanoSuitModel.Draw(cubeShader);
		}

		glfwSwapBuffers(window); // swaps double buffers (call after all render commands are completed)
		glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
	}
}

void ReflectionScene::initializeTextures(uint32& skyboxTextureId)
{
	loadCubeMapTexture(skyboxFaceLocations, skyboxTextureId);
}