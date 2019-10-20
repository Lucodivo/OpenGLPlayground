#include "AsteroidBeltScene.h"
#include "../../common/FileLocations.h"
#include "../../Model.h"
#include "../../common/Util.h"
#include "../../common/ObjectData.h"

AsteroidBeltScene::AsteroidBeltScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
  : FirstPersonScene(window, initScreenHeight, initScreenWidth),
  modelShader(posNormalVertexShaderFileLoc, skyboxReflectionFragmentShaderFileLoc),
  modelInstanceShader(AsteroidVertexShaderFileLoc, textureModelFragmentShaderFileLoc),
  reflectModelInstanceShader(AsteroidVertexShaderFileLoc, skyboxReflectionFragmentShaderFileLoc),
  skyboxShader(skyboxVertexShaderFileLoc, skyboxFragmentShaderFileLoc) {}

void AsteroidBeltScene::runScene()
{
  uint32 skyboxVAO, skyboxVBO, skyboxEBO;
  initializeCubePositionVertexAttBuffers(skyboxVAO, skyboxVBO, skyboxEBO);

  renderLoop(skyboxVAO);

  glDeleteVertexArrays(1, &skyboxVAO);
  glDeleteBuffers(1, &skyboxVBO);
  glDeleteBuffers(1, &skyboxEBO);
}

void AsteroidBeltScene::renderLoop(uint32 skyboxVAO)
{
  uint32 skyboxTextureId;
  loadCubeMapTexture(skyboxInterstellarFaceLocations, skyboxTextureId);

  uint32 skybox2TextureId;
  loadCubeMapTexture(skyboxSpaceLightBlueFaceLocations, skybox2TextureId);

  Model planetModel((char*)planetModelAbsoluteLoc);
  Model asteroidModel((char*)asteroidModelAbsoluteLoc);

  const glm::mat4 projectionMat = glm::perspective(glm::radians(camera.Zoom), (float32)viewportWidth / (float32)viewportHeight, 0.1f, 100.0f);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureId);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_CUBE_MAP, skybox2TextureId);

  modelShader.use();
  modelShader.setUniform("projection", projectionMat);
  modelShader.setUniform("skybox", 1);

  skyboxShader.use();
  skyboxShader.setUniform("projection", projectionMat);
  skyboxShader.setUniform("skybox", 0);

  modelInstanceShader.use();
  modelInstanceShader.setUniform("projection", projectionMat);

  reflectModelInstanceShader.use();
  reflectModelInstanceShader.setUniform("projection", projectionMat);
  reflectModelInstanceShader.setUniform("skybox", 0);

  uint32 numAsteroids = 5000;
  glm::mat4* modelMatrices;
  modelMatrices = new glm::mat4[numAsteroids];
  srand((uint32)glfwGetTime()); // initialize random seed	
  float32 radius = 30.0;
  auto randDisplacement = []() -> float32 { return ((rand() % 2000) / 100.0f) - 10.0f; };
  for (uint32 i = 0; i < numAsteroids; i++)
  {
    glm::mat4 model;

    // Displace along circle with 'radius' in range [-offset, offset]
    float32 angle = (float)i / (float)numAsteroids * 360.0f;
    float32 displacement = randDisplacement();
    float32 x = sin(angle) * radius + displacement;
    displacement = randDisplacement();
    float32 y = displacement * 0.2f; // keep height of field smaller compared to width of x and z
    displacement = randDisplacement();
    float32 z = cos(angle) * radius + displacement;
    model = glm::translate(model, glm::vec3(x, y, z));

    // scale
    float32 scale = ((rand() % 200) / 1000.0f) + 0.05f;
    model = glm::scale(model, glm::vec3(scale));

    // rotate
    float32 rotAngle = (float32)(rand() % 360);
    model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

    // 4. now add to list of matrices
    modelMatrices[i] = model;
  }

  // vertex buffer object to store all model matrices for the asteroids
  uint32 buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, numAsteroids * sizeof(glm::mat4), modelMatrices, GL_STATIC_DRAW);

  for (uint32 i = 0; i < asteroidModel.meshes.size(); i++)
  {
    uint32 VAO = asteroidModel.meshes[i].VAO;
    glBindVertexArray(VAO);
    // vertex Attributes
    GLsizei vec4Size = sizeof(glm::vec4);
    // a vec4 is the largest attribute pointer available with a single call
    // So we must individually assign the 4 vec4 attribute pointers to receiver a mat4x4 in the shader
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

    // Modifies the rate at which vertex attributes advance when rendering multiple instances
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
  }

  // background clear color
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  camera.Position += glm::vec3(0.0f, 0.0f, 50.0f);

  float32 planetRotationSpeed = 5.0f;

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

    modelShader.use();
    modelShader.setUniform("view", viewMat);
    modelShader.setUniform("cameraPos", camera.Position);

    // draw Planet
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
    model = glm::rotate(model, t * glm::radians(planetRotationSpeed), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
    modelShader.setUniform("model", model);
    planetModel.Draw(modelShader);

    // draw meteorites
    reflectModelInstanceShader.use();
    reflectModelInstanceShader.setUniform("view", viewMat);
    reflectModelInstanceShader.setUniform("cameraPos", camera.Position);
    reflectModelInstanceShader.setUniform("orbit", glm::rotate(glm::mat4(), t * glm::radians(-planetRotationSpeed), glm::vec3(0.0f, 1.0f, 0.0f)));
    for (uint32 i = 0; i < asteroidModel.meshes.size(); i++)
    {
      glBindVertexArray(asteroidModel.meshes[i].VAO);
      glDrawElementsInstanced(GL_TRIANGLES, asteroidModel.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, numAsteroids);
    }

    // draw skybox
    skyboxShader.use();

    glBindVertexArray(skyboxVAO);

    glm::mat4 viewMinusTranslation = glm::mat4(glm::mat3(viewMat));
    skyboxShader.setUniform("view", viewMinusTranslation);

    glDrawElements(GL_TRIANGLES, // drawing mode
      36, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
      GL_UNSIGNED_INT, // type of the indices
      0); // offset in the EBO
    glBindVertexArray(0);

    glDisable(GL_DEPTH_TEST);
    uint32 numFrames = (uint32)(1 / deltaTime);
    renderText(std::to_string(numFrames) + " FPS", 25.0f, 25.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    glEnable(GL_DEPTH_TEST);

    glfwSwapBuffers(window); // swaps double buffers
    glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
  }
}