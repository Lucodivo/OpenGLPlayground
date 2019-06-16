#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Kernels.h"
#include "FileLocations.h"
#include "Util.h"

#include "PlaygroundScene.h"
#include "main.h"

// frame rate
float32 deltaTime = 0.0f;	// Time between current frame and last frame
float32 lastFrame = 0.0f; // Time of last frame

Camera camera = Camera();

bool flashLightOn = true;

uint32 framebuffer;
uint32 frameBufferTexture;
uint32 rbo;

uint32 selectedKernelIndex = 0;

uint32 VIEWPORT_INIT_WIDTH;
uint32 VIEWPORT_INIT_HEIGHT;

double kernelModeSwitchTimer = 0.0f;
uint32 kernelCount = ArrayCount(kernels);

void runPlaygroundScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth) {
	VIEWPORT_INIT_HEIGHT = initScreenHeight;
	VIEWPORT_INIT_WIDTH = initScreenWidth;
	
	uint32 lightVAO, lightVBO, lightEBO;
	initializeLightBuffers(lightVAO, lightVBO, lightEBO);

	uint32 shapesVAO, shapesVBO, shapesEBO;
	initializeObjectBuffers(shapesVAO, shapesVBO, shapesEBO);

	uint32 quadVAO, quadVBO, quadEBO;
	initializeQuadBuffers(quadVAO, quadVBO, quadEBO);

	renderLoop(window, shapesVAO, lightVAO, quadVAO);

	glDeleteVertexArrays(1, &shapesVAO);
	glDeleteBuffers(1, &shapesVBO);
	glDeleteBuffers(1, &shapesEBO);

	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &lightVBO);
	glDeleteBuffers(1, &lightEBO);
}

void renderLoop(GLFWwindow* window, uint32& shapesVAO, uint32& lightVAO, uint32& quadVAO) {
	Shader cubeShader = Shader(cubeVertexShaderFileLoc, cubeFragmentShaderFileLoc);
	Shader lightShader = Shader(lightVertexShaderFileLoc, lightFragmentShaderFileLoc);
	Shader modelShader = Shader(modelVertexShaderFileLoc, modelFragmentShaderFileLoc);
	Shader stencilShader = Shader(cubeVertexShaderFileLoc, stencilFragmentShaderFileLoc);
	Shader frameBufferShader = Shader(frameBufferVertexShaderFileLoc, kernelFrameBufferFragmentShaderFileLoc);

	uint32 diffTextureId;
	uint32 specTextureId;
	initializeTextures(diffTextureId, specTextureId);

	initializeFrameBuffer(VIEWPORT_INIT_WIDTH, VIEWPORT_INIT_HEIGHT);

	// load models
	Model nanoSuitModel((char*)nanoSuitModelLoc);

	const glm::mat4 projectionMat = glm::perspective(glm::radians(camera.Zoom), (float32)VIEWPORT_INIT_WIDTH / (float32)VIEWPORT_INIT_HEIGHT, 0.1f, 100.0f);

	const float32 lightOrbitSpeed = 20.0f;
	const glm::vec3 lightAxisRot(0.0f, 1.0f, 0.0f);
	const float32 lightScale = 0.2f;

	const float32 cubRotAngle = 7.3f;

	glm::vec3 directionalLightDir = glm::vec3(-0.0f, -1.0f, -3.0f);
	glm::vec3 directionalLightColor = glm::vec3(1.0f);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// background clear color
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

#if 0
	// draw in wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	// NOTE: render/game loop
	while (glfwWindowShouldClose(window) == GL_FALSE) {

		// check for input
		processInput(window); // TODO: Break out input processing. Remove circular dependency between main and PlaygroundScene.

		// bind our frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, // when stencil fails
			GL_KEEP, // when stencil passes but depth fails
			GL_REPLACE); // when stencil passes and depth passes
		glStencilMask(0xFF); // mask that is ANDed with the stencil value that is about to be written to stencil buffer
		glStencilFunc(GL_ALWAYS, // stencil function
			1, // reference value for stencil test
			0xFF); // mask that is ANDed with stencil value and reference value before the test compares them

#if 1
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);           // OpenGL state-using function
#else
		// FUN MODE - WINDOWS XP
		glClear(GL_DEPTH_BUFFER_BIT);
#endif

		// if we want to make sure we do not update the stencil buffer while drawing something
		//glStencilMask(0x00);
		//normalShader.use();
		//DrawSomething()

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// if flashlight is off, simply remove all color from light
		glm::vec3 flashLightColor = flashLightOn ? glm::vec3(0.93f, 0.84f, 0.72f) : glm::vec3(0.0f);

		float32 t = (float32)glfwGetTime();
		deltaTime = t - lastFrame;
		lastFrame = t;
		float32 sineVal = sin(t);
		float32 lightR = (sin((t + 30) / 3) / 2) + 0.5f;
		float32 lightG = (sin((t + 60) / 8) / 2) + 0.5f;
		float32 lightB = (sin(t / 17) / 2) + 0.5f;
		glm::vec3 positionalLightColor(lightR, lightG, lightB);

		glm::mat4 viewMat = camera.GetViewMatrix(deltaTime);

		// oscillate with time
		const glm::vec3 lightPosition = glm::vec3(2.0f, 0.0f + sineVal, 2.0f);
		// orbit with time
		glm::mat4 lightModel; // default constructor is identity matrix
		lightModel = glm::rotate(lightModel, t * glm::radians(lightOrbitSpeed), lightAxisRot);
		lightModel = glm::translate(lightModel, lightPosition);
		lightModel = glm::scale(lightModel, glm::vec3(lightScale));

		// draw positional light
		lightShader.use();
		glBindVertexArray(lightVAO);

		lightShader.setUniform("model", lightModel);
		lightShader.setUniform("view", viewMat);
		lightShader.setUniform("projection", projectionMat);
		lightShader.setUniform("lightColor", positionalLightColor);
		glDrawElements(GL_TRIANGLES, // drawing mode
			cubeNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
			GL_UNSIGNED_INT, // type of the indices
			0); // offset in the EBO
		glBindVertexArray(0);

		// draw cubes
		cubeShader.use();

		// User fragment shaders to draw a triangle
		glm::vec4 worldLightPos = lightModel * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		// rotate with time
		glm::mat4 cubeModel = glm::rotate(glm::mat4(), t * glm::radians(cubRotAngle), glm::vec3(1.0f, 0.3f, 0.5f));
		// switch between two images over time
		bool animSwitch = sin(8 * t) > 0;
		// emission strength fluctuating over time
		float32 emissionStrength = ((sin(t * 2) + 1.0f) / 4) + 0.15f;

		auto setLightUniforms = [&](Shader shader) {
			// positional light (orbiting light)
			shader.setUniform("positionalLight.position", worldLightPos.x, worldLightPos.y, worldLightPos.z);
			shader.setUniform("positionalLight.color.ambient", positionalLightColor * glm::vec3(0.05f));
			shader.setUniform("positionalLight.color.diffuse", positionalLightColor * glm::vec3(0.5f));
			shader.setUniform("positionalLight.color.specular", positionalLightColor * glm::vec3(1.0f));
			shader.setUniform("positionalLight.attenuation.constant", 1.0f);
			shader.setUniform("positionalLight.attenuation.linear", 0.09f);
			shader.setUniform("positionalLight.attenuation.quadratic", 0.032f);

			// directional light
			shader.setUniform("directionalLight.direction", directionalLightDir);
			shader.setUniform("directionalLight.color.ambient", directionalLightColor * glm::vec3(0.1f));
			shader.setUniform("directionalLight.color.diffuse", directionalLightColor * glm::vec3(0.4f));
			shader.setUniform("directionalLight.color.specular", directionalLightColor * glm::vec3(0.5f));

			// flash light
			shader.setUniform("spotLight.position", camera.Position);
			shader.setUniform("spotLight.direction", camera.Front);
			shader.setUniform("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
			shader.setUniform("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
			shader.setUniform("spotLight.color.ambient", flashLightColor * glm::vec3(0.05f));
			shader.setUniform("spotLight.color.diffuse", flashLightColor * glm::vec3(0.3f));
			shader.setUniform("spotLight.color.specular", flashLightColor * glm::vec3(0.5f));
			shader.setUniform("spotLight.attenuation.constant", 1.0f);
			shader.setUniform("spotLight.attenuation.linear", 0.09f);
			shader.setUniform("spotLight.attenuation.quadratic", 0.032f);

			// emission light
			shader.setUniform("emissionStrength", emissionStrength);
		};

		// bind shapesVAO
		glBindVertexArray(shapesVAO);

		glActiveTexture(GL_TEXTURE0 + diffTextureId);
		glBindTexture(GL_TEXTURE_2D, diffTextureId);
		glActiveTexture(GL_TEXTURE0 + specTextureId);
		glBindTexture(GL_TEXTURE_2D, specTextureId);

		setLightUniforms(cubeShader);

		cubeShader.setUniform("material.diffTexture1", diffTextureId);
		cubeShader.setUniform("material.specTexture1", specTextureId);
		cubeShader.setUniform("material.shininess", 32.0f);

		cubeShader.setUniform("animSwitch", animSwitch);
		cubeShader.setUniform("alphaDiscard", true);

		cubeShader.setUniform("viewPos", camera.Position);
		cubeShader.setUniform("view", viewMat);
		cubeShader.setUniform("projection", projectionMat);
		// draw cubes
		for (uint32 i = 0; i < numCubes; i++) {
			glm::mat4 model;
			float32 angularSpeed = 7.3f * (i + 1);

			// orbit around the specified axis from the translated distance
			model = glm::rotate(model, t * glm::radians(angularSpeed), glm::vec3(50.0f - (i * 10), 100.0f, -50.0f + (i * 10)));
			// translate to position in world
			model = glm::translate(model, cubePositions[i]);
			// rotate with time
			model = glm::rotate(model, t * glm::radians(angularSpeed), glm::vec3(1.0f, 0.3f, 0.5f));
			// scale object
			model = glm::scale(model, glm::vec3(cubeScales[i]));
			cubeShader.setUniform("model", model);
			glDrawElements(GL_TRIANGLES, // drawing mode
				cubeNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
				GL_UNSIGNED_INT, // type of the indices
				0); // offset in the EBO
		}

		// draw cube stencil outlines
		for (uint32 i = 0; i < numCubes; i++) {
			float32 angularSpeed = 7.3f * (i + 1);

			glm::mat4 model;
			// orbit around the specified axis from the translated distance
			model = glm::rotate(model, t * glm::radians(angularSpeed), glm::vec3(50.0f - (i * 10), 100.0f, -50.0f + (i * 10)));
			// translate to position in world
			model = glm::translate(model, cubePositions[i]);
			// rotate with time
			model = glm::rotate(model, t * glm::radians(angularSpeed), glm::vec3(1.0f, 0.3f, 0.5f));
			// scale object
			model = glm::scale(model, glm::vec3(cubeScales[i] + 0.05f));
			stencilShader.use();
			stencilShader.setUniform("singleColor", glm::vec3(1.0f, 1.0f, 1.0f));
			stencilShader.setUniform("projection", projectionMat);
			stencilShader.setUniform("view", viewMat);
			stencilShader.setUniform("model", model);
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			glDisable(GL_DEPTH_TEST);
			glDrawElements(GL_TRIANGLES, // drawing mode
				cubeNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
				GL_UNSIGNED_INT, // type of the indices
				0); // offset in the EBO
			glEnable(GL_DEPTH_TEST);
		}

		// unbind shapesVAO
		glBindVertexArray(0);

		// draw models
		float32 modelScale = 0.2f;
		{
			glStencilFunc(GL_ALWAYS, // stencil function
				1, // reference value for stencil test
				0xFF); // mask that is ANDed with stencil value and reference value before the test compares them
			glStencilMask(0xFF); // mask that is ANDed with the stencil value that is about to be written to stencil buffer
			glClear(GL_STENCIL_BUFFER_BIT); // NOTE: glClear(GL_STENCIL_BUFFER_BIT) counts as writing to the stencil buffer and will be directly ANDed with the stencil mask

			// Drawing the model
			modelShader.use();
			setLightUniforms(modelShader);

			modelShader.setUniform("material.shininess", 32.0f);

			modelShader.setUniform("viewPos", camera.Position);
			modelShader.setUniform("projection", projectionMat);
			modelShader.setUniform("view", viewMat);

			glm::mat4 model;
			model = glm::scale(model, glm::vec3(modelScale));	// it's a bit too big for our scene, so scale it down
			model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
			modelShader.setUniform("model", model);
			nanoSuitModel.Draw(modelShader);

			//  Wall Hack Stencil For Model
			stencilShader.use();
			stencilShader.setUniform("singleColor", glm::vec3(0.5f, 0.0f, 0.0f));
			stencilShader.setUniform("projection", projectionMat);
			stencilShader.setUniform("view", viewMat);
			stencilShader.setUniform("model", model);
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			glDisable(GL_DEPTH_TEST);
			nanoSuitModel.Draw(stencilShader);
		}

		// bind default frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
		glClear(GL_COLOR_BUFFER_BIT);

		frameBufferShader.use();
		glUniform1fv(glGetUniformLocation(frameBufferShader.ID, "kernel"), 9, kernels[selectedKernelIndex]);
		glBindVertexArray(quadVAO);
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
		glDrawElements(GL_TRIANGLES, // drawing mode
			6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
			GL_UNSIGNED_INT, // type of the indices
			0); // offset in the EBO

		glfwSwapBuffers(window); // swaps double buffers (call after all render commands are completed)
		glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
	}
}

void initializeObjectBuffers(uint32& VAO, uint32& VBO, uint32& EBO) {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, // Num objects to generate 
		&VBO);  // Out parameters to store IDs of gen objects
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind object to array buffer
	glBufferData(GL_ARRAY_BUFFER, // which buffer data is being entered in
		sizeof(cubeVertexAttributes), // size of data being placed in array buffer
		cubeVertexAttributes,        // data to store in array buffer       
		GL_STATIC_DRAW); // GL_STATIC_DRAW (most likely not change), GL_DYNAMIC_DRAW (likely to change), GL_STREAM_DRAW (changes every time drawn)

	// position attribute
	glVertexAttribPointer(0, // position vertex attribute (used for location = 0 of Vertex Shader) 
		3, // size of vertex attribute (we're using vec3)
		GL_FLOAT, // type of data being passed 
		GL_FALSE, // whether the data needs to be normalized
		cubeVertexAttSizeInBytes, // stride: space between consecutive vertex attribute sets
		(void*)0); // offset of where the data starts in the array
	glEnableVertexAttribArray(0);

	// texture coords attribute
	glVertexAttribPointer(1,
		2,
		GL_FLOAT,
		GL_FALSE,
		cubeVertexAttSizeInBytes,
		(void*)(3 * sizeof(float32)));
	glEnableVertexAttribArray(1);

	// normal attribute
	glVertexAttribPointer(2,
		3,
		GL_FLOAT,
		GL_FALSE,
		cubeVertexAttSizeInBytes,
		(void*)(5 * sizeof(float32)));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

	// unbind VBO, VAO, & EBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// Must unbind EBO AFTER unbinding VAO, since VAO stores all glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _) calls
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void initializeLightBuffers(uint32& VAO, uint32& VBO, uint32& EBO) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1,     // Num objects to generate 
		&VBO);  // Out parameters to store IDs of gen objects
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind object to array buffer
	glBufferData(GL_ARRAY_BUFFER, // which buffer data is being entered in
		sizeof(cubeVertexAttributes), // size of data being placed in array buffer
		cubeVertexAttributes,        // data to store in array buffer       
		GL_STATIC_DRAW); // GL_STATIC_DRAW (most likely not change), GL_DYNAMIC_DRAW (likely to change), GL_STREAM_DRAW (changes every time drawn)

	// set the vertex attributes (only position data for our lamp)
	// position attribute
	glVertexAttribPointer(0, // position vertex attribute (used for location = 0 of Vertex Shader) 
		3, // size of vertex attribute (we're using vec3)
		GL_FLOAT, // type of data being passed 
		GL_FALSE, // whether the data needs to be normalized
		cubeVertexAttSizeInBytes, // stride: space between consecutive vertex attribute sets
		(void*)0); // offset of where the data starts in the array
	glEnableVertexAttribArray(0);

	// bind element buffer object to give indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

	// unbind VBO, VAO, & EBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// Must unbind EBO AFTER unbinding VAO, since VAO stores all glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _) calls
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void initializeQuadBuffers(uint32& VAO, uint32& VBO, uint32& EBO) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1,
		&VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(quadVertexAttributes),
		quadVertexAttributes,
		GL_STATIC_DRAW);

	// set the vertex attributes (position and texture)
	// position attribute
	glVertexAttribPointer(0,
		2,
		GL_FLOAT,
		GL_FALSE,
		quadVertexAttSizeInBytes,
		(void*)0);
	glEnableVertexAttribArray(0);

	// texture attribute
	glVertexAttribPointer(1,
		2,
		GL_FLOAT,
		GL_FALSE,
		quadVertexAttSizeInBytes,
		(void*)(2 * sizeof(float32)));
	glEnableVertexAttribArray(1);

	// bind element buffer object to give indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

	// unbind VBO, VAO, & EBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// Must unbind EBO AFTER unbinding VAO, since VAO stores all glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _) calls
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void initializeTextures(uint32& diffTextureId, uint32& specTextureId)
{
	loadTexture(diffuseTextureLoc, diffTextureId);
	loadTexture(specularTextureLoc, specTextureId);
}

void initializeFrameBuffer(uint32 width, uint32 height)
{
	glDeleteFramebuffers(1, &framebuffer);
	glDeleteRenderbuffers(1, &rbo);
	glDeleteTextures(1, &frameBufferTexture);

	// creating frame buffer
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// creating frame buffer texture
	glGenTextures(1, &frameBufferTexture);
	glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0); // unbind

	// attach texture w/ color to frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, // frame buffer we're tageting (draw, read, or both)
		GL_COLOR_ATTACHMENT0, // type of attachment
		GL_TEXTURE_2D, // type of texture
		frameBufferTexture, // texture 
		0); // mipmap level

	// creating render buffer to be depth/stencil buffer
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0); // unbind
	// attach render buffer w/ depth & stencil to frame buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, // frame buffer target
		GL_DEPTH_STENCIL_ATTACHMENT, // attachment point of frame buffer
		GL_RENDERBUFFER, // render buffer target
		rbo);  // render buffer

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void key_LeftShift_pressed() {
	camera.MovementSpeed = SPEED * 2;
}

void key_LeftShift_released() {
	camera.MovementSpeed = SPEED;
}

void key_W() {
	camera.ProcessKeyboard(FORWARD);
}

void key_S() {
	camera.ProcessKeyboard(BACKWARD);
}

void key_A() {
	camera.ProcessKeyboard(LEFT);
}

void key_D() {
	camera.ProcessKeyboard(RIGHT);
}

void key_Space() {
	camera.ProcessKeyboard(JUMP);
}

void key_LeftMouseButton_pressed() {
	flashLightOn = !flashLightOn;
}

void key_LeftMouseButton_released() {
	// Do nothing
}

void key_Up() {
	double currentTime = glfwGetTime();
	if (currentTime - kernelModeSwitchTimer > 0.5f) {
		selectedKernelIndex = (selectedKernelIndex + 1) % kernelCount;
		kernelModeSwitchTimer = currentTime;
	}
}

void key_Down() {
	double currentTime = glfwGetTime();
	if (currentTime - kernelModeSwitchTimer > 0.5f) {
		selectedKernelIndex = selectedKernelIndex != 0 ? ((selectedKernelIndex - 1) % kernelCount) : (kernelCount - 1);
		kernelModeSwitchTimer = currentTime;
	}
}

void mouseMove(float32 xOffset, float32 yOffset) {
	camera.ProcessMouseMovement(xOffset, yOffset);
}

void mouseScroll(float32 yOffset) {
	camera.ProcessMouseScroll(yOffset);
}