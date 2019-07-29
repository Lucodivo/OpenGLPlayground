#include "FloorScene.h"
#include "FileLocations.h"
#include "Model.h"
#include "Util.h"
#include "ObjectData.h"

FloorScene::FloorScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
  : GodModeScene(window, initScreenHeight, initScreenWidth),
  fourPositionalLightShader(posNormTexVertexShaderFileLoc, blinnPhongLightingFragmentShaderFileLoc),
  lightShader(posVertexShaderFileLoc, singleColorFragmentShaderFileLoc) {}

void FloorScene::runScene()
{
  uint32 floorVAO, floorVBO, floorEBO;
  initializeQuadVertexAttBuffers(floorVAO, floorVBO, floorEBO);

  uint32 cubeVAO, cubeVBO, cubeEBO;
  initializeCubePosTexNormAttBuffers(cubeVAO, cubeVBO, cubeEBO);

  renderLoop(floorVAO, cubeVAO);

  glDeleteVertexArrays(1, &floorVAO);
  glDeleteBuffers(1, &floorVBO);
  glDeleteBuffers(1, &floorEBO);

  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteBuffers(1, &cubeVBO);
  glDeleteBuffers(1, &cubeEBO);
}


void FloorScene::renderLoop(uint32 floorVAO, uint32 cubeVAO)
{
  uint32 floorTextureId;
  load2DTexture(marbleTextureLoc, floorTextureId, false, true);

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

  glm::vec3 lightColor1(1.0f, 1.0f, 1.0f);
  glm::vec3 lightColor2(1.0f, 0.0f, 0.0f);
  glm::vec3 lightColor3(0.0f, 1.0f, 0.0f);
  glm::vec3 lightColor4(0.0f, 0.0f, 1.0f);
  const float32 lightScale = 0.2f;

  const float32 floorScale = 8.0f;
  const glm::vec3 floorPosition(0.0f, -3.0f, 0.0f);
  const float32 lightRadius = 4.0f;
  const float32 lightAmplitude = 2.0f;

  // Turn on gamma correction for entire scene
  //glEnable(GL_FRAMEBUFFER_SRGB);

  fourPositionalLightShader.use();

  // set material
  fourPositionalLightShader.setUniform("material.diffTexture1", 0);
  fourPositionalLightShader.setUniform("material.specTexture1", 0);
  fourPositionalLightShader.setUniform("material.shininess", 32.0f);
  fourPositionalLightShader.setUniform("attenuation.constant", 1.0f);
  fourPositionalLightShader.setUniform("attenuation.linear", 0.09f);
  fourPositionalLightShader.setUniform("attenuation.quadratic", 0.032f);

  // set lighting 1
  fourPositionalLightShader.setUniform("positionalLights[0].color.ambient", lightColor1 * 0.05f);
  fourPositionalLightShader.setUniform("positionalLights[0].color.diffuse", lightColor1 * 0.3f);
  fourPositionalLightShader.setUniform("positionalLights[0].color.specular", lightColor1);
  // set lighting 2
  fourPositionalLightShader.setUniform("positionalLights[1].color.ambient", lightColor2 * 0.05f);
  fourPositionalLightShader.setUniform("positionalLights[1].color.diffuse", lightColor2 * 0.3f);
  fourPositionalLightShader.setUniform("positionalLights[1].color.specular", lightColor2);
  // set lighting 3
  fourPositionalLightShader.setUniform("positionalLights[2].color.ambient", lightColor3 * 0.05f);
  fourPositionalLightShader.setUniform("positionalLights[2].color.diffuse", lightColor3 * 0.3f);
  fourPositionalLightShader.setUniform("positionalLights[2].color.specular", lightColor3);
  // set lighting 4
  fourPositionalLightShader.setUniform("positionalLights[3].color.ambient", lightColor4 * 0.05f);
  fourPositionalLightShader.setUniform("positionalLights[3].color.diffuse", lightColor4 * 0.3f);
  fourPositionalLightShader.setUniform("positionalLights[3].color.specular", lightColor4);

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

    fourPositionalLightShader.bindBlockIndex("globalBlockVS", globalVSBufferBindIndex);
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
    glBindVertexArray(cubeVAO);

    const int32 lightCount = 1;
    // draw light 1
    float lightPairOffset = 1.5f;
    glm::vec3 lightPosition1(sin(t) * lightRadius - lightPairOffset, -sin(t) * lightAmplitude, cos(t) * lightRadius - lightPairOffset);
    glm::mat4 lightModel1;
    lightModel1 = glm::translate(lightModel1, lightPosition1);
    lightModel1 = glm::scale(lightModel1, glm::vec3(lightScale));
    lightShader.setUniform("model", lightModel1);
    lightShader.setUniform("color", lightColor1);
    glDrawElements(GL_TRIANGLES, // drawing mode
      cubePosTexNormNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
      GL_UNSIGNED_INT, // type of the indices
      0); // offset in the EBO

    glm::vec3 lightPosition2;
    if (lightCount >= 2)
    {
      // draw light 2
      glm::mat4 lightModel2;
      lightPosition2 = glm::vec3(sin(t) * lightRadius + lightPairOffset, sin(t) * lightAmplitude, -cos(t) * lightRadius + lightPairOffset);
      lightModel2 = glm::translate(lightModel2, lightPosition2);
      lightModel2 = glm::scale(lightModel2, glm::vec3(lightScale));
      lightShader.setUniform("model", lightModel2);
      lightShader.setUniform("color", lightColor2);
      glDrawElements(GL_TRIANGLES, // drawing mode
        cubePosTexNormNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
        GL_UNSIGNED_INT, // type of the indices
        0); // offset in the EBO
    }

    glm::vec3 lightPosition3;
    if (lightCount >= 3)
    {
      // draw light 3
      glm::mat4 lightModel3;
      lightPosition3 = glm::vec3(-sin(t) * lightRadius + lightPairOffset, sin(t) * lightAmplitude, cos(t) * lightRadius + lightPairOffset);
      lightModel3 = glm::translate(lightModel3, lightPosition3);
      lightModel3 = glm::scale(lightModel3, glm::vec3(lightScale));
      lightShader.setUniform("model", lightModel3);
      lightShader.setUniform("color", lightColor3);
      glDrawElements(GL_TRIANGLES, // drawing mode
        cubePosTexNormNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
        GL_UNSIGNED_INT, // type of the indices
        0); // offset in the EBO
    }

    glm::vec3 lightPosition4;
    if (lightCount == 4)
    {
      // draw light 4
      glm::mat4 lightModel4;
      lightPosition4 = glm::vec3(-sin(t) * lightRadius - lightPairOffset, -sin(t) * lightAmplitude, -cos(t) * lightRadius - lightPairOffset);
      lightModel4 = glm::translate(lightModel4, lightPosition4);
      lightModel4 = glm::scale(lightModel4, glm::vec3(lightScale));
      lightShader.setUniform("model", lightModel4);
      lightShader.setUniform("color", lightColor4);
      glDrawElements(GL_TRIANGLES, // drawing mode
        cubePosTexNormNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
        GL_UNSIGNED_INT, // type of the indices
        0); // offset in the EBO
    }
    glBindVertexArray(0);

    fourPositionalLightShader.use();
    fourPositionalLightShader.setUniform("viewPos", camera.Position);
    fourPositionalLightShader.setUniform("positionalLights[0].position", lightPosition1);
    fourPositionalLightShader.setUniform("positionalLights[1].position", lightPosition2);
    fourPositionalLightShader.setUniform("positionalLights[2].position", lightPosition3);
    fourPositionalLightShader.setUniform("positionalLights[3].position", lightPosition4);
    fourPositionalLightShader.setUniform("positionalLightCount", 1);

    // draw floow
    glm::mat4 floorModel = glm::mat4();
    floorModel = glm::translate(floorModel, floorPosition);
    floorModel = glm::scale(floorModel, glm::vec3(floorScale, 1.0f, floorScale));
    floorModel = glm::rotate(floorModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    fourPositionalLightShader.setUniform("model", floorModel);
    glBindVertexArray(floorVAO);
    glDrawElements(GL_TRIANGLES, // drawing mode
      6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
      GL_UNSIGNED_INT, // type of the indices
      0); // offset in the EBO

    // draw center cube
    glm::mat4 cubeModel;
    cubeModel = glm::scale(cubeModel, glm::vec3(1.0f));
    fourPositionalLightShader.setUniform("model", cubeModel);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, // drawing mode
      cubePosTexNormNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
      GL_UNSIGNED_INT, // type of the indices
      0); // offset in the EBO
    glBindVertexArray(0);

    glfwSwapBuffers(window); // swaps double buffers
    glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
  }
}