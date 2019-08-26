#include "FloorScene.h"
#include "FileLocations.h"
#include "Model.h"
#include "Util.h"
#include "ObjectData.h"

FloorScene::FloorScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
  : GodModeScene(window, initScreenHeight, initScreenWidth),
  fourPositionalLightShader(lightSpaceVertexShaderFileLoc, blinnPhongLightingFragmentShaderFileLoc),
  lightShader(posVertexShaderFileLoc, singleColorFragmentShaderFileLoc),
  depthShader(simpleDepthVertexShaderFileLoc, emptyFragmentShaderFileLoc) {}

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
  uint32 floorTextureId, cubeTextureId;
  load2DTexture(marbleTextureLoc, floorTextureId, false, true);
  load2DTexture(cementTextureLoc, cubeTextureId, false, true);

  unsigned int depthMapFBO;
  glGenFramebuffers(1, &depthMapFBO);

  const uint32 SHADOW_WIDTH = 1024;
  const uint32 SHADOW_HEIGHT = 1024;

  unsigned int depthMapTextureId;
  glGenTextures(1, &depthMapTextureId);
  glBindTexture(GL_TEXTURE_2D, depthMapTextureId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
               SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // ensure that areas outside of the shadow map are never determined to be in shadow
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTextureId, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, floorTextureId);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, cubeTextureId);

  const glm::mat4 cameraProjMat = glm::perspective(glm::radians(camera.Zoom), (float32)viewportWidth / (float32)viewportHeight, 0.1f, 100.0f);

  const float near_plane = 1.0f, far_plane = 14.0f, projectionDimens = 10.0f;
  // Note: orthographic projection is used for directional lighting, as all light rays are parallel
  const glm::mat4 lightProjMat = glm::ortho(-projectionDimens, projectionDimens, -projectionDimens, projectionDimens, near_plane, far_plane);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  // background clear color
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
  glm::vec3 lightColor2(1.0f, 0.0f, 0.0f);
  glm::vec3 lightColor3(0.0f, 1.0f, 0.0f);
  glm::vec3 lightColor4(0.0f, 0.0f, 1.0f);
  const float32 lightScale = 0.5f;

  const float32 floorScale = 32.0f;
  const glm::vec3 floorPosition(0.0f, -3.0f, 0.0f);

  const float32 cubeScale = 3.0f;
  const glm::vec3 cubePosition = glm::vec3(0.0f, -3.0f + (cubeScale / 2.0f), 0.0f);
  
  const float32 lightRadius = 4.0f;
  const float32 lightAmplitude = 2.0f;
  const float32 lightHeightOffset = 6.0f;

  // Turn on gamma correction for entire scene
  //glEnable(GL_FRAMEBUFFER_SRGB);

  fourPositionalLightShader.use();

  // set material
  fourPositionalLightShader.setUniform("material.shininess", 32.0f);
  fourPositionalLightShader.setUniform("attenuation.constant", 1.0f);
  fourPositionalLightShader.setUniform("attenuation.linear", 0.04f);
  fourPositionalLightShader.setUniform("attenuation.quadratic", 0.016f);

  // set lighting 1
  fourPositionalLightShader.setUniform("positionalLight.color.ambient", lightColor * 0.05f);
  fourPositionalLightShader.setUniform("positionalLight.color.diffuse", lightColor * 0.3f);
  fourPositionalLightShader.setUniform("positionalLight.color.specular", lightColor);

  const uint32 shadowMap2DSamplerIndex = 2;
  fourPositionalLightShader.setUniform("shadowMap", shadowMap2DSamplerIndex);

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

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(cameraProjMat));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    fourPositionalLightShader.bindBlockIndex("globalBlockVS", globalVSBufferBindIndex);
    lightShader.bindBlockIndex("globalBlockVS", globalVSBufferBindIndex);
  }

  camera.Position += glm::vec3(0.0f, 10.0f, 20.0f);

  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glFrontFace(GL_BACK);

  // NOTE: render/game loop
  while (glfwWindowShouldClose(window) == GL_FALSE)
  {
    // check for input
    processKeyboardInput(window, this);
    processXInput(this);

    float32 t = (float32)glfwGetTime();
    deltaTime = t - lastFrame;
    lastFrame = t;

    glm::mat4 viewMat = camera.GetViewMatrix(deltaTime);

    glBindBuffer(GL_UNIFORM_BUFFER, globalVSUniformBuffer);
    // update global view matrix uniform
    glBufferSubData(GL_UNIFORM_BUFFER, globalVSBufferViewMatOffset, sizeof(glm::mat4), glm::value_ptr(viewMat));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // light data
    glm::vec3 lightPosition = glm::vec3(sin(t) * lightRadius, lightHeightOffset, cos(t) * lightRadius);
    glm::mat4 lightModel;
    lightModel = glm::translate(lightModel, lightPosition);
    lightModel = glm::scale(lightModel, glm::vec3(lightScale));

    // floor data
    glm::mat4 floorModel = glm::mat4();
    floorModel = glm::translate(floorModel, floorPosition);
    floorModel = glm::scale(floorModel, glm::vec3(floorScale, 1.0f, floorScale));
    floorModel = glm::rotate(floorModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // cube data
    glm::mat4 cubeModel = glm::mat4();
    cubeModel = glm::translate(cubeModel, cubePosition);
    cubeModel = glm::scale(cubeModel, glm::vec3(cubeScale));

    // render depth map from light's point of view
    glm::mat4 lightView = glm::lookAt(lightPosition,
                                      cubePosition,
                                      glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjMat * lightView;


    depthShader.use();
    depthShader.setUniform("lightSpaceMatrix", lightSpaceMatrix);
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    // depth map for floor
    depthShader.setUniform("model", floorModel);
    glBindVertexArray(floorVAO);
    glDrawElements(GL_TRIANGLES, // drawing mode
                   6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
                   GL_UNSIGNED_INT, // type of the indices
                   0); // offset in the EBO

    // depth map for cube
    depthShader.setUniform("model", cubeModel);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, // drawing mode
                   cubePosTexNormNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                   GL_UNSIGNED_INT, // type of the indices
                   0); // offset in the EBO
    glBindVertexArray(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // render scene using the depth map for shadows (using depth map)
    glViewport(0, 0, viewportWidth, viewportHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0 + shadowMap2DSamplerIndex);
    glBindTexture(GL_TEXTURE_2D, depthMapTextureId);

    // draw positional light
    lightShader.use();
    glBindVertexArray(cubeVAO);
    lightShader.setUniform("model", lightModel);
    lightShader.setUniform("color", lightColor);
    glDrawElements(GL_TRIANGLES, // drawing mode
                   cubePosTexNormNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                   GL_UNSIGNED_INT, // type of the indices
                   0); // offset in the EBO

    fourPositionalLightShader.use();
    fourPositionalLightShader.setUniform("viewPos", camera.Position);
    fourPositionalLightShader.setUniform("lightSpaceMatrix", lightSpaceMatrix);
    fourPositionalLightShader.setUniform("positionalLight.position", lightPosition);

    // draw floor
    fourPositionalLightShader.setUniform("model", floorModel);
    fourPositionalLightShader.setUniform("material.diffTexture1", 0);
    fourPositionalLightShader.setUniform("material.specTexture1", 0);
    glBindVertexArray(floorVAO);
    glDrawElements(GL_TRIANGLES, // drawing mode
      6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
      GL_UNSIGNED_INT, // type of the indices
      0); // offset in the EBO

    // draw center cube
    fourPositionalLightShader.setUniform("model", cubeModel);
    fourPositionalLightShader.setUniform("material.diffTexture1", 1);
    fourPositionalLightShader.setUniform("material.specTexture1", 1);
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