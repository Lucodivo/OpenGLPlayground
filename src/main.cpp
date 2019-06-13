#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "main.h"

#include <iostream>

#define VIEWPORT_INIT_WIDTH 800
#define VIEWPORT_INIT_HEIGHT 600

#define local_persist static

// shader information
const char *cubeVertexShaderFile = "src/shaders/CubeVertexShader.glsl";
const char *cubeFragmentShaderFile = "src/shaders/CubeFragmentShader.glsl";
const char *lightVertexShaderFile = "src/shaders/LightSourceVertexShader.glsl";
const char *lightFragmentShaderFile = "src/shaders/LightSourceFragmentShader.glsl";
const char *modelVertexShaderFile = "src/shaders/ModelVertexShader.glsl";
const char *modelFragmentShaderFile = "src/shaders/ModelFragmentShader.glsl";
const char *depthFragmentShaderFile = "src/shaders/DepthBufferVisualizerFragmentShader.glsl";
const char *stencilFragmentShaderFile = "src/shaders/SingleColorFragmentShader.glsl";

// texture 
const char *diffuseTextureLoc = "src/data/diffuse_map_alpha_channel.png";
const char *specularTextureLoc = "src/data/specular_map.png";
const char *emissionTextureLoc = "src/data/emission_map.png";
const char* nanoSuitModelLoc = "C:/developer/repos/LearnOpenGL/LearnOpenGL/src/data/nanosuit/nanosuit.obj";

// frame rate
float32 deltaTime = 0.0f;	// Time between current frame and last frame
float32 lastFrame = 0.0f; // Time of last frame
float32 lastX = VIEWPORT_INIT_WIDTH / 2;
float32 lastY = VIEWPORT_INIT_HEIGHT / 2;

Camera camera = Camera();

bool flashLightOn = true;

int main() {
    loadGLFW();
    GLFWwindow* window = createWindow();
    glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    initializeGLAD();

    uint32 lightVAO, lightVBO, lightEBO;
    initializeLightBuffers(lightVAO, lightVBO, lightEBO);
	
    uint32 shapesVAO, shapesVBO, shapesEBO;
    initializeObjectBuffers(shapesVAO, shapesVBO, shapesEBO);

    renderLoop(window, shapesVAO, lightVAO);

    glDeleteVertexArrays(1, &shapesVAO);
    glDeleteBuffers(1, &shapesVBO);
    glDeleteBuffers(1, &shapesEBO);

	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &lightVBO);
	glDeleteBuffers(1, &lightEBO);
	glfwTerminate(); // clean up gl resources
	return 0;
}

void loadGLFW() {
    int loadSucceeded = glfwInit();
    if (loadSucceeded == GLFW_FALSE) {
        std::cout << "Failed to load GLFW" << std::endl;
        exit(-1);
    }
}

void initializeGLAD() {
	// intialize GLAD to help manage function pointers for OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(-1);
	}
}

GLFWwindow* createWindow() {
    // set what kind of window desired
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL version x._
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL version _.x
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment for OSX

    // create window
    GLFWwindow* window = glfwCreateWindow(VIEWPORT_INIT_WIDTH, // int Width
        VIEWPORT_INIT_HEIGHT, // int Height
        "LearnOpenGL", // const char* Title
        NULL, // GLFWmonitor* Monitor: Specified for which monitor for fullscreen, NULL for windowed mode
        NULL); // GLFWwindow* Share: window to share resources with
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    return window;
}

void initializeObjectBuffers(uint32 &VAO, uint32 &VBO, uint32 &EBO) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1,     // Num objects to generate 
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

void initializeLightBuffers(uint32 &VAO, uint32 &VBO, uint32 &EBO) {
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

void initializeTextures(uint32& diffTextureId, uint32& specTextureId) {
	loadTexture(diffuseTextureLoc, diffTextureId);
	loadTexture(specularTextureLoc, specTextureId);
}

void loadTexture(const char* imgLocation, uint32& textureId) {
	glGenTextures(1, &textureId);
	uint32 textureOffset = textureId - 1;
	glActiveTexture(GL_TEXTURE0 + textureOffset); // activate texture unit (by default it is bound to GL_TEXTURE0)
	glBindTexture(GL_TEXTURE_2D, textureId);

	// load image data
	int width, height, numChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(imgLocation, &width, &height, &numChannels, 0);
	if (data) {
		auto pixelFormat = (numChannels == 3) ? GL_RGB : GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, // target
			0, // level of detail (level n is the nth mipmap reduction image)
			pixelFormat, // kind of format we want to store the texture
			width, // width of texture
			height, // height of texture
			0, // border (legacy stuff, MUST BE 0)
			pixelFormat, // Specifies format of the pixel data
			GL_UNSIGNED_BYTE, // specifies data type of pixel data
			data); // pointer to the image data
		glGenerateMipmap(GL_TEXTURE_2D);

		// set texture options
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // disables bilinear filtering (creates sharp edges when magnifying texture)
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data); // free texture image memory
}

void processInput(GLFWwindow * window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	camera.MovementSpeed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? SPEED * 2 : SPEED;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(JUMP);

	static bool leftMouseButtonWasDown = false;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (!leftMouseButtonWasDown) {
			leftMouseButtonWasDown = true;
			flashLightOn = !flashLightOn;
		}
	}
	else {
		leftMouseButtonWasDown = false;
	}

	static bool windowMode = true;
	static double windowModeSwitchTimer = glfwGetTime();
	if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS &&
		glfwGetTime() - windowModeSwitchTimer > 1.5f) {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		if (windowMode) {
			glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
		}
		else {
			glfwSetWindowMonitor(window, NULL, (mode->width / 4), (mode->height / 4), VIEWPORT_INIT_WIDTH, VIEWPORT_INIT_HEIGHT, GLFW_DONT_CARE);
		}
		windowMode = !windowMode;
		windowModeSwitchTimer = glfwGetTime();
	}
}

// Callback function for when user resizes our window
void framebuffer_size_callback(GLFWwindow * window, int width, int height) {
	glViewport(0, 0, width, height);
}

// Callback function for when user moves mouse
void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	static bool firstMouse = true;
	if (firstMouse) {
		lastX = (float32)xpos;
		lastY = (float32)ypos;
		firstMouse = false;
	}

	float32 xoffset = (float32)xpos - lastX;
	float32 yoffset = lastY - (float32)ypos; // reversed since y-coordinates go from bottom to top

	lastX = (float32)xpos;
	lastY = (float32)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// Callback function for when user scrolls with mouse wheel
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float32)yoffset);
}

void renderLoop(GLFWwindow *window, uint32 &shapesVAO, uint32 &lightVAO) {
    Shader cubeShader = Shader(cubeVertexShaderFile, cubeFragmentShaderFile);
    Shader lightShader = Shader(lightVertexShaderFile, lightFragmentShaderFile);
	Shader modelShader = Shader(modelVertexShaderFile, modelFragmentShaderFile);
	Shader stencilShader = Shader(cubeVertexShaderFile, stencilFragmentShaderFile);

	uint32 diffTextureId;
	uint32 specTextureId;
    initializeTextures(diffTextureId, specTextureId);

	// load models
	Model nanoSuitModel((char*)nanoSuitModelLoc);

    const glm::mat4 projectionMat = glm::perspective(glm::radians(camera.Zoom), (float32)VIEWPORT_INIT_WIDTH / (float32)VIEWPORT_INIT_HEIGHT, 0.1f, 100.0f);

    const float32 lightOrbitSpeed = 20.0f;
    const glm::vec3 lightAxisRot(0.0f, 1.0f, 0.0f);
    const float32 lightScale = 0.2f;

    const float32 cubRotAngle = 7.3f;

    glm::vec3 directionalLightDir = glm::vec3(-0.0f, -1.0f, -3.0f);
    glm::vec3 directionalLightColor = glm::vec3(1.0f);

	// clear the background color
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // NOTE: render/game loop
    while (glfwWindowShouldClose(window) == GL_FALSE) {

		// check for input
		processInput(window);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glFrontFace(GL_CCW);

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
			cubeNumElements * 3, // number of elements to draw (6 vertices)
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
				cubeNumElements * 3, // number of elements to draw (6 vertices)
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
				cubeNumElements * 3, // number of elements to draw (6 vertices)
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

        glfwSwapBuffers(window); // swaps double buffers (call after all render commands are completed)
        glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
    }
}