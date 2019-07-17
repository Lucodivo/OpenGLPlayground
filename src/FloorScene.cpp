#include "FloorScene.h"
#include "FileLocations.h"
#include "Model.h"
#include "Util.h"
#include "ObjectData.h"

FloorScene::FloorScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
	: GodModeScene(window, initScreenHeight, initScreenWidth),
	floorShader(posNormTexVertexShaderFileLoc, blinnPhongLightingFragmentShaderFileLoc),
	lightShader(posVertexShaderFileLoc, singleColorFragmentShaderFileLoc) {}

void FloorScene::runScene()
{
	uint32 floorVAO, floorVBO, floorEBO;
	initializeQuadVertexAttBuffers(floorVAO, floorVBO, floorEBO);

	uint32 lightVAO, lightVBO, lightEBO;
	initializeCubePositionAttBuffers(lightVAO, lightVBO, lightEBO);

	renderLoop(floorVAO, lightVAO);

	glDeleteVertexArrays(1, &floorVAO);
	glDeleteBuffers(1, &floorVBO);
	glDeleteBuffers(1, &floorEBO);

	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &lightVBO);
	glDeleteBuffers(1, &lightEBO);
}


void FloorScene::renderLoop(uint32 floorVAO, uint32 lightVAO)
{
	uint32 floorTextureId;
	load2DTexture(marbleTextureLoc, floorTextureId);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, floorTextureId);

	const glm::mat4 projectionMat = glm::perspective(glm::radians(camera.Zoom), (float32)viewportWidth / (float32)viewportHeight, 0.1f, 100.0f);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

	// background clear color
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	const float32 lightScale = 0.2f;

	const float32 floorScale = 8.0f;
	const glm::vec3 floorPosition(0.0f, -3.0f, 0.0f);
  const float32 lightRadius = 4.0f;
  const float32 lightAmplitude = 2.0f;

	floorShader.use();

	// set material
	floorShader.setUniform("material.diffTexture1", 0);
	floorShader.setUniform("material.specTexture1", 0);
	floorShader.setUniform("material.shininess", 32.0f);

	// set lighting
	floorShader.setUniform("positionalLight.color.ambient", lightColor * 0.4f);
	floorShader.setUniform("positionalLight.color.diffuse", lightColor * 0.9f);
	floorShader.setUniform("positionalLight.color.specular", lightColor);
	floorShader.setUniform("positionalLight.attenuation.constant", 1.0f);
	floorShader.setUniform("positionalLight.attenuation.linear", 0.09f);
	floorShader.setUniform("positionalLight.attenuation.quadratic", 0.032f);

	uint32 globalVSUniformBuffer;
	uint32 globalVSBufferBindIndex = 0;
	uint32 globalVSBufferViewMatOffset = sizeof(glm::mat4);
	{
		glGenBuffers(1, &globalVSUniformBuffer);

		glBindBuffer(GL_UNIFORM_BUFFER, globalVSUniformBuffer);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);

		glBindBufferRange(GL_UNIFORM_BUFFER,		// target
			globalVSBufferBindIndex,	// index of binding point 
			globalVSUniformBuffer,	// buffer id
			0,						// starting offset into buffer object
			4 * 16);				// size: 4 vec3's, 16 bits alignments

		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projectionMat));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		floorShader.bindBlockIndex("globalBlockVS", globalVSBufferBindIndex);
		lightShader.bindBlockIndex("globalBlockVS", globalVSBufferBindIndex);
	}

	camera.Position += glm::vec3(0.0f, 0.0f, 10.0f);

	// NOTE: render/game loop
	while (glfwWindowShouldClose(window) == GL_FALSE)
	{
		// check for input
		processKeyboardInput(window, this);
		processXInput(this);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);           // OpenGL state-using function

		float32 t = (float32)glfwGetTime();
		deltaTime = t - lastFrame;
		lastFrame = t;

		glm::mat4 viewMat = camera.GetViewMatrix(deltaTime);

		glBindBuffer(GL_UNIFORM_BUFFER, globalVSUniformBuffer);
		// update global view matrix uniform
		glBufferSubData(GL_UNIFORM_BUFFER, globalVSBufferViewMatOffset, sizeof(glm::mat4), glm::value_ptr(viewMat));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// draw positional light
		lightShader.use();
		glBindVertexArray(lightVAO);


    glm::vec3 lightPosition(sin(t) * lightRadius, sin(t) * lightAmplitude, cos(t) * lightRadius);

		glm::mat4 lightModel;
		lightModel = glm::translate(lightModel, lightPosition);
		lightModel = glm::scale(lightModel, glm::vec3(lightScale));
		lightShader.setUniform("model", lightModel);
		lightShader.setUniform("color", lightColor);
		glDrawElements(GL_TRIANGLES, // drawing mode
			cubePosTexNormNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
			GL_UNSIGNED_INT, // type of the indices
			0); // offset in the EBO
		glBindVertexArray(0);

		floorShader.use();
		floorShader.setUniform("viewPos", camera.Position);
    floorShader.setUniform("positionalLight.position", lightPosition);
		glm::mat4 floorModel = glm::mat4();
		floorModel = glm::translate(floorModel, floorPosition);
		floorModel = glm::scale(floorModel, glm::vec3(floorScale, 1.0f, floorScale));
		floorModel = glm::rotate(floorModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		floorShader.setUniform("model", floorModel);

		glBindVertexArray(floorVAO);
		glDrawElements(GL_TRIANGLES, // drawing mode
			6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
			GL_UNSIGNED_INT, // type of the indices
			0); // offset in the EBO

		glfwSwapBuffers(window); // swaps double buffers
		glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
	}
}