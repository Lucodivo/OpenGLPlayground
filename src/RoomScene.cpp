#include "RoomScene.h"
#include "FileLocations.h"
#include "Model.h"
#include "Util.h"
#include "ObjectData.h"

const uint32 SHADOW_MAP_WIDTH = 2048;
const uint32 SHADOW_MAP_HEIGHT = 2048;

RoomScene::RoomScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
  : GodModeScene(window, initScreenHeight, initScreenWidth),
  positionalLightShader(posNormTexVertexShaderFileLoc, positionalLightShadowMapFragmentShaderFileLoc),
  singleColorShader(posVertexShaderFileLoc, singleColorFragmentShaderFileLoc),
  depthCubeMapShader(modelMatVertexShaderFileLoc, linearDepthMapFragmentShaderFileLoc, cubeMapGeometryShaderFileLoc)
{}

void RoomScene::runScene()
{
  uint32 cubeVAO, cubeVBO, cubeEBO;
  initializeCubePosTexNormAttBuffers(cubeVAO, cubeVBO, cubeEBO);

  uint32 invertedNormCubeVAO, invertedNormCubeVBO, invertedNormCubeEBO;
  initializeCubePosTexNormAttBuffers(invertedNormCubeVAO, invertedNormCubeVBO, invertedNormCubeEBO, true);

  renderLoop(cubeVAO, invertedNormCubeVAO);

  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteBuffers(1, &cubeVBO);
  glDeleteBuffers(1, &cubeEBO);

  glDeleteVertexArrays(1, &invertedNormCubeVAO);
  glDeleteBuffers(1, &invertedNormCubeVBO);
  glDeleteBuffers(1, &invertedNormCubeEBO);
}

void RoomScene::renderLoop(uint32 cubeVAO, uint32 invertedNormCubeVAO)
{
  uint32 wallpaperTextureId, cubeTextureId;
  load2DTexture(hardwoodTextureLoc, wallpaperTextureId, false, true);
  load2DTexture(cementTextureLoc, cubeTextureId, false, true);

  unsigned int depthCubeMapTextureId, depthMapFBO;
  generateDepthMap(depthCubeMapTextureId, depthMapFBO);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, wallpaperTextureId);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, cubeTextureId);

  const glm::mat4 cameraProjMat = glm::perspective(glm::radians(camera.Zoom), (float32)viewportWidth / (float32)viewportHeight, 0.1f, 120.0f);

  const float aspect_ratio = (float)SHADOW_MAP_WIDTH / (float)SHADOW_MAP_HEIGHT;
  const float nearPlane = 1.0f;
  const float farPlane = 40.0f;
  const glm::mat4 lightProjMat = glm::perspective(glm::radians(90.0f), aspect_ratio, nearPlane, farPlane);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  // background clear color
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  glm::vec3 lightColor(0.5f, 0.5f, 0.5f);
  const float32 lightScale = 0.3f;

  const float32 roomScale = 32.0f;
  const glm::vec3 roomPosition(0.0f, -3.0f, 0.0f);

  const float32 cubeScale1 = 3.0f;
  const glm::vec3 cubePosition1 = glm::vec3(0.0f, roomPosition.y + (cubeScale1 / 2.0f), 0.0f);

  const float32 cubeScale2 = 2.2f;
  const glm::vec3 cubePosition2 = glm::vec3(8.0f, roomPosition.y + (cubeScale2 / 2) - 8.0f, 8.0f);

  const float32 cubeScale3 = 1.7f;
  const glm::vec3 cubePosition3 = glm::vec3(2.0f, roomPosition.y + (cubeScale3 / 2.0f) + 4.0f, 3.0f);

  const float32 lightRadius = 8.0f;
  const float32 lightAmplitude = 8.0f;

  // Turn on gamma correction for entire scene
  //glEnable(GL_FRAMEBUFFER_SRGB);

  positionalLightShader.use();

  // set material
  positionalLightShader.setUniform("attenuation.constant", 1.0f);
  positionalLightShader.setUniform("attenuation.linear", 0.04f);
  positionalLightShader.setUniform("attenuation.quadratic", 0.016f);

  // set lighting 1
  positionalLightShader.setUniform("positionalLight.color.ambient", lightColor * 0.05f);
  positionalLightShader.setUniform("positionalLight.color.diffuse", lightColor * 0.3f);
  positionalLightShader.setUniform("positionalLight.color.specular", lightColor);

  const uint32 shadowCubeMapSamplerIndex = 2;
  positionalLightShader.setUniform("shadowCubeMap", shadowCubeMapSamplerIndex);

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

    positionalLightShader.bindBlockIndex("globalBlockVS", globalVSBufferBindIndex);
    singleColorShader.bindBlockIndex("globalBlockVS", globalVSBufferBindIndex);
  }

  camera.Position += glm::vec3(0.0f, 10.0f, 30.0f);

  // room data
  glm::mat4 roomModelMat = glm::mat4();
  roomModelMat = glm::translate(roomModelMat, roomPosition);
  roomModelMat = glm::scale(roomModelMat, glm::vec3(roomScale));

  // cube data
  glm::mat4 cubeModelMat1 = glm::mat4();
  cubeModelMat1 = glm::translate(cubeModelMat1, cubePosition1);
  cubeModelMat1 = glm::scale(cubeModelMat1, glm::vec3(cubeScale1));
  glm::mat4 cubeModelMat2 = glm::mat4();
  cubeModelMat2 = glm::translate(cubeModelMat2, cubePosition2);
  cubeModelMat2 = glm::scale(cubeModelMat2, glm::vec3(cubeScale2));
  glm::mat4 cubeModelMat3 = glm::mat4();
  cubeModelMat3 = glm::translate(cubeModelMat3, cubePosition3);
  cubeModelMat3 = glm::scale(cubeModelMat3, glm::vec3(cubeScale3));

  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);

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
    glm::vec3 lightPosition = glm::vec3(sin(t) * lightRadius, sin(1.5f * t) * lightAmplitude, cos(t) * lightRadius);
    glm::mat4 lightModel;
    lightModel = glm::translate(lightModel, lightPosition);
    lightModel = glm::scale(lightModel, glm::vec3(lightScale));

    glm::mat4 shadowMapTransMats[6];
    shadowMapTransMats[0] = (lightProjMat * glm::lookAt(lightPosition, lightPosition + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0))); // right
    shadowMapTransMats[1] = (lightProjMat * glm::lookAt(lightPosition, lightPosition + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0))); // left
    shadowMapTransMats[2] = (lightProjMat * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0))); // top
    shadowMapTransMats[3] = (lightProjMat * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0))); // bottom
    shadowMapTransMats[4] = (lightProjMat * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0))); // far
    shadowMapTransMats[5] = (lightProjMat * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0))); // near

    depthCubeMapShader.use();
    // TODO: don't?
    depthCubeMapShader.setUniform("cubeMapTransMats[0]", shadowMapTransMats[0]);
    depthCubeMapShader.setUniform("cubeMapTransMats[1]", shadowMapTransMats[1]);
    depthCubeMapShader.setUniform("cubeMapTransMats[2]", shadowMapTransMats[2]);
    depthCubeMapShader.setUniform("cubeMapTransMats[3]", shadowMapTransMats[3]);
    depthCubeMapShader.setUniform("cubeMapTransMats[4]", shadowMapTransMats[4]);
    depthCubeMapShader.setUniform("cubeMapTransMats[5]", shadowMapTransMats[5]);
    depthCubeMapShader.setUniform("lightPos", lightPosition);
    depthCubeMapShader.setUniform("farPlane", farPlane);

    glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    // depth map for room
    glCullFace(GL_FRONT);
    glBindVertexArray(invertedNormCubeVAO);
    depthCubeMapShader.setUniform("model", roomModelMat);
    glDrawElements(GL_TRIANGLES, // drawing mode
      cubePosTexNormNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
      GL_UNSIGNED_INT, // type of the indices
      0); // offset in the EBO

    // depth map for cubes
    glCullFace(GL_BACK);
    glBindVertexArray(cubeVAO);
    depthCubeMapShader.setUniform("model", cubeModelMat1);
    glDrawElements(GL_TRIANGLES, // drawing mode
      cubePosTexNormNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
      GL_UNSIGNED_INT, // type of the indices
      0); // offset in the EBO

    depthCubeMapShader.setUniform("model", cubeModelMat2);
    glDrawElements(GL_TRIANGLES, // drawing mode
      cubePosTexNormNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
      GL_UNSIGNED_INT, // type of the indices
      0); // offset in the EBO

    depthCubeMapShader.setUniform("model", cubeModelMat3);
    glDrawElements(GL_TRIANGLES, // drawing mode
      cubePosTexNormNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
      GL_UNSIGNED_INT, // type of the indices
      0); // offset in the EBO
    glBindVertexArray(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // render scene using the depth map for shadows (using depth map)
    glViewport(0, 0, viewportWidth, viewportHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0 + shadowCubeMapSamplerIndex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMapTextureId);

    // draw positional light
    singleColorShader.use();
    glBindVertexArray(cubeVAO);
    singleColorShader.setUniform("model", lightModel);
    singleColorShader.setUniform("color", lightColor);
    glDrawElements(GL_TRIANGLES, // drawing mode
      cubePosTexNormNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
      GL_UNSIGNED_INT, // type of the indices
      0); // offset in the EBO

    glm::vec3 lightDir = glm::normalize(lightPosition);
    positionalLightShader.use();
    positionalLightShader.setUniform("viewPos", camera.Position);
    positionalLightShader.setUniform("positionalLight.position", lightPosition);
    positionalLightShader.setUniform("farPlane", farPlane);

    // draw room
    glCullFace(GL_FRONT);
    glBindVertexArray(invertedNormCubeVAO);
    positionalLightShader.setUniform("model", roomModelMat);
    positionalLightShader.setUniform("material.diffTexture1", 0);
    positionalLightShader.setUniform("material.specTexture1", 0);
    glDrawElements(GL_TRIANGLES, // drawing mode
      cubePosTexNormNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
      GL_UNSIGNED_INT, // type of the indices
      0); // offset in the EBO

    // draw cubes
    glCullFace(GL_BACK);
    glBindVertexArray(cubeVAO);
    positionalLightShader.setUniform("model", cubeModelMat1);
    positionalLightShader.setUniform("material.diffTexture1", 1);
    positionalLightShader.setUniform("material.specTexture1", 1);
    glDrawElements(GL_TRIANGLES, // drawing mode
      cubePosTexNormNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
      GL_UNSIGNED_INT, // type of the indices
      0); // offset in the EBO
    positionalLightShader.setUniform("model", cubeModelMat2);
    glDrawElements(GL_TRIANGLES, // drawing mode
      cubePosTexNormNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
      GL_UNSIGNED_INT, // type of the indices
      0); // offset in the EBO
    positionalLightShader.setUniform("model", cubeModelMat3);
    glDrawElements(GL_TRIANGLES, // drawing mode
      cubePosTexNormNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
      GL_UNSIGNED_INT, // type of the indices
      0); // offset in the EBO
    glBindVertexArray(0);

    glfwSwapBuffers(window); // swaps double buffers
    glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
  }
}

void RoomScene::generateDepthMap(uint32& depthCubeMapId, uint32& depthMapFBO)
{
  glGenFramebuffers(1, &depthMapFBO);

  glGenTextures(1, &depthCubeMapId);
  glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMapId);
  for(unsigned int i = 0; i < 6; ++i)
  {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                 SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  }

  // TODO: Remove
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //// ensure that areas outside of the shadow map are NEVER determined to be in shadow
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  //float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
  //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMapId, 0);
  // The following two calls tell OpenGL that we are not trying to output any kind of color
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}