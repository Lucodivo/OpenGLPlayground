#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Camera.h"
#include "main.h"

#define VIEWPORT_INIT_WIDTH 800
#define VIEWPORT_INIT_HEIGHT 600

#define local_persist static

// shader information
const char *vertexShaderFile = "VertexShader.glsl";
const char *fragmentShaderFile = "FragmentShader.glsl";
const char *lightVertexShaderFile = "LightSourceVertexShader.glsl";
const char *lightFragmentShaderFile = "LightSourceFragmentShader.glsl";

// texture 
const char *diffuseTextureLoc = "data/diffuse_map.png";
const char *specularTextureLoc = "data/specular_map.png";
const char *emissionTextureLoc = "data/emission_map.png";

// frame rate
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float lastX = VIEWPORT_INIT_WIDTH / 2;
float lastY = VIEWPORT_INIT_HEIGHT / 2;

Camera camera = Camera();

bool flashLightOn = true;

int main() {
    loadGLFW();
    GLFWwindow* window = createWindow();
    glfwMakeContextCurrent(window);

    initializeGLAD();

    glViewport(0, // int x: left-x of viewport rect
        0, // int y: bottom-y of viewport rect
        VIEWPORT_INIT_WIDTH, // int width
        VIEWPORT_INIT_HEIGHT); // int height

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    unsigned int shapesVAO, shapesVBO, EBO;
    initializeObjectBuffers(shapesVAO, shapesVBO, EBO);

    unsigned int lightVAO, lightVBO;
    initializeLightBuffers(lightVAO, lightVBO, EBO);

    renderLoop(window, shapesVAO, lightVAO);

    glDeleteVertexArrays(1, &shapesVAO);
    glDeleteBuffers(1, &shapesVBO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &lightVBO);
    glDeleteBuffers(1, &EBO);
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

void initializeGLAD() {
    // intialize GLAD to help manage function pointers for OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
}

void initializeObjectBuffers(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO) {
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
        (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // normal attribute
    glVertexAttribPointer(2,
        3,
        GL_FLOAT,
        GL_FALSE,
        cubeVertexAttSizeInBytes,
        (void*)(5 * sizeof(float)));
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

void initializeLightBuffers(unsigned int &VAO, unsigned int &VBO, const unsigned int &EBO) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // we only need to bind to the VBO, the container's VBO's data already contains the correct data.
    glGenBuffers(1,     // Num objects to generate 
        &VBO);  // Out parameters to store IDs of gen objects
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind object to array buffer
    glBufferData(GL_ARRAY_BUFFER, // which buffer data is being entered in
        sizeof(cubeVertexAttributes), // size of data being placed in array buffer
        cubeVertexAttributes,        // data to store in array buffer       
        GL_STATIC_DRAW); // GL_STATIC_DRAW (most likely not change), GL_DYNAMIC_DRAW (likely to change), GL_STREAM_DRAW (changes every time drawn)
    // set the vertex attributes (only position data for our lamp)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, cubeVertexAttSizeInBytes * sizeof(float), (void*)0);
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

void renderLoop(GLFWwindow *window, unsigned int &shapesVAO, unsigned int &lightVAO) {
    Shader shapesShader = Shader(vertexShaderFile, fragmentShaderFile);
    Shader lightShader = Shader(lightVertexShaderFile, lightFragmentShaderFile);
    initializeTextures(shapesShader);

    glEnable(GL_DEPTH_TEST);

    const glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)VIEWPORT_INIT_WIDTH / (float)VIEWPORT_INIT_HEIGHT, 0.1f, 100.0f);

    const float lightOrbitSpeed = 20.0f;
    const glm::vec3 lightAxisRot(0.0f, 1.0f, 0.0f);
    const float lightScale = 0.2f;

    const float cubRotAngle = 7.3f;

    glm::vec3 lightDirection = glm::vec3(-1.0f, -1.0f, -1.0f);
    glm::vec3 directionalLightColor = glm::vec3(1.0f);

    // NOTE: render/game loop
    while (glfwWindowShouldClose(window) == GL_FALSE) {
        // check for input
        processInput(window);

        // clear the background
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);                         // OpenGL state-setting function
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);           // OpenGL state-using function

		// if flashlight is off, simply remove all color from light
        glm::vec3 flashLightColor = flashLightOn ? glm::vec3(0.93f, 0.84f, 0.72f) : glm::vec3(0.0f);

        float t = (float)glfwGetTime();
        deltaTime = t - lastFrame;
        lastFrame = t;
        float sineVal = sin(t);
        float lightR = (sin((t + 30) / 3) / 2) + 0.5;
        float lightG = (sin((t + 60) / 8) / 2) + 0.5;
        float lightB = (sin(t / 17) / 2) + 0.5;
        glm::vec3 positionalLightColor(lightR, lightG, lightB);

		// switch between two imagest over time
        float animSwitch = sin(8 * t) > 0;

        float emissionStrength = ((sin(t * 2) + 1.0f) / 4) + 0.15;

        glm::mat4 view = camera.GetViewMatrix(deltaTime);

        // Create light model to position the light in the world
        glm::mat4 lightModel;

		// oscillate with time
        const glm::vec3 lightPosition = glm::vec3(2.0f, 0.0f + sineVal, 2.0f);
		// orbit with time
        lightModel = glm::rotate(lightModel, t * glm::radians(lightOrbitSpeed), lightAxisRot);
        lightModel = glm::translate(lightModel, lightPosition);
        lightModel = glm::scale(lightModel, glm::vec3(lightScale));

        // draw light
        lightShader.use();
        glBindVertexArray(lightVAO);

        lightShader.setUniform("model", lightModel);
        lightShader.setUniform("view", view);
        lightShader.setUniform("projection", projection);
        lightShader.setUniform("lightColor", positionalLightColor);
        glDrawElements(GL_TRIANGLES, // drawing mode
            cubeNumElements * 3, // number of elements to draw (6 vertices)
            GL_UNSIGNED_INT, // type of the indices
            0); // offset in the EBO
        glBindVertexArray(0);

        // User fragment shaders to draw a triangle
        glm::vec4 worldLightPos = lightModel * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        glm::mat4 cubeModel;
        // translate to position in world
        cubeModel = glm::translate(cubeModel, cubePositions[0]);
        // rotate with time
        cubeModel = glm::rotate(cubeModel, t * glm::radians(cubRotAngle), glm::vec3(1.0f, 0.3f, 0.5f));

        // draw cube
        shapesShader.use();
        glBindVertexArray(shapesVAO);
        shapesShader.setUniform("view", view);
        shapesShader.setUniform("projection", projection);

		// positional light (orbiting light)
        shapesShader.setUniform("positionalLight.position", worldLightPos.x, worldLightPos.y, worldLightPos.z);
        shapesShader.setUniform("positionalLight.color.ambient", positionalLightColor * glm::vec3(0.2f));
        shapesShader.setUniform("positionalLight.color.diffuse", positionalLightColor * glm::vec3(0.5f));
        shapesShader.setUniform("positionalLight.color.specular", positionalLightColor * glm::vec3(1.0f));
		shapesShader.setUniform("positionalLight.attenuation.constant", 1.0f);
		shapesShader.setUniform("positionalLight.attenuation.linear", 0.09f);
		shapesShader.setUniform("positionalLight.attenuation.quadratic", 0.032f);

		// directional light
        shapesShader.setUniform("directionalLight.direction", lightDirection);
        shapesShader.setUniform("directionalLight.color.ambient", directionalLightColor * glm::vec3(0.2f));
        shapesShader.setUniform("directionalLight.color.diffuse", directionalLightColor * glm::vec3(0.4f));
        shapesShader.setUniform("directionalLight.color.specular", directionalLightColor * glm::vec3(0.5f));

		// flash light
        shapesShader.setUniform("spotLight.position", camera.Position);
        shapesShader.setUniform("spotLight.direction", camera.Front);
        shapesShader.setUniform("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        shapesShader.setUniform("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
        shapesShader.setUniform("spotLight.color.ambient", flashLightColor * glm::vec3(0.1f));
        shapesShader.setUniform("spotLight.color.diffuse", flashLightColor * glm::vec3(0.3f));
        shapesShader.setUniform("spotLight.color.specular", flashLightColor * glm::vec3(0.5f));

        shapesShader.setUniform("material.shininess", 32.0f);
        shapesShader.setUniform("animSwitch", animSwitch);
        shapesShader.setUniform("emissionStrength", emissionStrength);
        shapesShader.setUniform("viewPos", camera.Position);

        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model;
            // translate to position in world
            model = glm::translate(model, cubePositions[i]);
            // rotate with time
            float angle = 7.3f * (i + 1);
            model = glm::rotate(model, t * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shapesShader.setUniform("model", model);
            glDrawElements(GL_TRIANGLES, // drawing mode
                cubeNumElements * 3, // number of elements to draw (6 vertices)
                GL_UNSIGNED_INT, // type of the indices
                0); // offset in the EBO
                    //glDrawArrays(GL_TRIANGLES, 0, cubeNumElements * 3);
        }

        glDrawElements(GL_TRIANGLES, // drawing mode
            cubeNumElements * 3, // number of elements to draw (6 vertices)
            GL_UNSIGNED_INT, // type of the indices
            0); // offset in the EBO
        glBindVertexArray(0); // unbind shapesVAO

        glfwSwapBuffers(window); // swaps double buffers (call after all render commands are completed)
        glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
    }
}

void initializeTextures(Shader &shader) {
    shader.use(); // must activate/use the shader before setting the uniforms!
    loadTexture(diffuseTextureLoc, 0);
    shader.setUniform("material.diffTexture", 0);
    loadTexture(specularTextureLoc, 1);
    shader.setUniform("material.specTexture", 1);
    loadTexture(emissionTextureLoc, 2);
    shader.setUniform("material.emissionTexture", 2);
}

void loadTexture(const char* imgLocation, unsigned int textureOffset) {
    unsigned int texture; // stores id of generated texture
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0 + textureOffset); // activate texture unit (by default it is bound to GL_TEXTURE0)
    glBindTexture(GL_TEXTURE_2D, texture);

    // load image data
    int width, height, numChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(imgLocation, &width, &height, &numChannels, 0);
    if (data) {
        auto pixelFormat = (numChannels == 3) ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, // target
            0, // level of detail (level n is the nth mipmap reduction image)
            GL_RGB, // kind of format we want to store the texture
            width, // width of texture
            height, // height of texture
            0, // border (legacy stuff, MUST BE 0)
            pixelFormat, // Specifies format of the pixel data
            GL_UNSIGNED_BYTE, // specifies data type of pixel data
            data); // pointer to the image data
        glGenerateMipmap(GL_TEXTURE_2D);

        // set texture options
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // disables bilinear filtering (creates sharp edges when magnifying texture)
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data); // free texture image memory
}

void processInput(GLFWwindow *window) {
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
    } else {
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
        } else {
            glfwSetWindowMonitor(window, NULL, (mode->width / 4), (mode->height / 4), VIEWPORT_INIT_WIDTH, VIEWPORT_INIT_HEIGHT, GLFW_DONT_CARE);
        }
        windowMode = !windowMode;
        windowModeSwitchTimer = glfwGetTime();
    }
}

// Callback function for when user resizes our window
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Callback function for when user moves mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    static bool firstMouse = true;
    if (firstMouse) {
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
    }

    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos; // reversed since y-coordinates go from bottom to top

    lastX = (float)xpos;
    lastY = (float)ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// Callback function for when user scrolls with mouse wheel
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll((float)yoffset);
}