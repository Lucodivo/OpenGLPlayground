#include "MoonScene.h"
#include "../../common/FileLocations.h"
#include "../../common/Util.h"
#include "../../common/ObjectData.h"

const uint32 SHADOW_MAP_WIDTH = 2048;
const uint32 SHADOW_MAP_HEIGHT = 2048;

MoonScene::MoonScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
        : GodModeScene(window, initScreenHeight, initScreenWidth),
          directionalLightShader(lightSpaceVertexShaderFileLoc, directionalLightShadowMapFragmentShaderFileLoc, tbnGeometryShaderFileLoc),
          quadTextureShader(billboardPosTexVertexShaderFileLoc, textureFragmentShaderFileLoc),
          depthMapShader(simpleDepthVertexShaderFileLoc, emptyFragmentShaderFileLoc) {}

void MoonScene::runScene()
{
  uint32 floorVAO, floorVBO, floorEBO;
  initializeQuadPosNormTexVertexAttBuffers(floorVAO, floorVBO, floorEBO);

  uint32 cubeVAO, cubeVBO, cubeEBO;
  initializeCubePosNormTexVertexAttBuffers(cubeVAO, cubeVBO, cubeEBO);

  renderLoop(floorVAO, cubeVAO);

  glDeleteVertexArrays(1, &floorVAO);
  glDeleteBuffers(1, &floorVBO);
  glDeleteBuffers(1, &floorEBO);

  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteBuffers(1, &cubeVBO);
  glDeleteBuffers(1, &cubeEBO);
}


void MoonScene::renderLoop(uint32 quadVAO, uint32 cubeVAO)
{
  uint32 floorAlbedoTextureId, floorNormalTextureId, floorHeightTextureId;
  load2DTexture(dungeonStoneAlbedoTextureLoc, floorAlbedoTextureId, false, true);
  load2DTexture(dungeonStoneNormalTextureLoc, floorNormalTextureId, false, false);
  load2DTexture(dungeonStoneHeightTextureLoc, floorHeightTextureId, false, false);
  uint32 cube1AlbedoTextureId, cube1NormalTextureId, cube1HeightTextureId;
  load2DTexture(waterWornStoneAlbedoTextureLoc, cube1AlbedoTextureId, false, true);
  load2DTexture(waterWornStoneNormalTextureLoc, cube1NormalTextureId, false, false);
  load2DTexture(waterWornStoneHeightTextureLoc, cube1HeightTextureId, false, false);
  uint32 cube2AlbedoTextureId, cube2NormalTextureId, cube2HeightTextureId;
  load2DTexture(copperRockAlbedoTextureLoc, cube2AlbedoTextureId, false, true);
  load2DTexture(copperRockNormalTextureLoc, cube2NormalTextureId, false, false);
  load2DTexture(copperRockHeightTextureLoc, cube2HeightTextureId, false, false);
  uint32 cube3AlbedoTextureId, cube3NormalTextureId, cube3HeightTextureId;
  load2DTexture(whiteSpruceAlbedoTextureLoc, cube3AlbedoTextureId, false, true);
  load2DTexture(whiteSpruceNormalTextureLoc, cube3NormalTextureId, false, false);
  load2DTexture(whiteSpruceHeightTextureLoc, cube3HeightTextureId, false, false);
  uint32 lightTextureId;
  load2DTexture(moonTextureAlbedoLoc, lightTextureId, false, false);

  uint32 depthMapTextureId, depthMapFBO;
  generateDepthMap(depthMapTextureId, depthMapFBO);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, floorAlbedoTextureId);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, cube1AlbedoTextureId);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, cube2AlbedoTextureId);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, cube3AlbedoTextureId);
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, floorNormalTextureId);
  glActiveTexture(GL_TEXTURE5);
  glBindTexture(GL_TEXTURE_2D, cube1NormalTextureId);
  glActiveTexture(GL_TEXTURE6);
  glBindTexture(GL_TEXTURE_2D, cube2NormalTextureId);
  glActiveTexture(GL_TEXTURE7);
  glBindTexture(GL_TEXTURE_2D, cube3NormalTextureId);
  glActiveTexture(GL_TEXTURE8);
  glBindTexture(GL_TEXTURE_2D, floorHeightTextureId);
  glActiveTexture(GL_TEXTURE9);
  glBindTexture(GL_TEXTURE_2D, cube1HeightTextureId);
  glActiveTexture(GL_TEXTURE10);
  glBindTexture(GL_TEXTURE_2D, cube2HeightTextureId);
  glActiveTexture(GL_TEXTURE11);
  glBindTexture(GL_TEXTURE_2D, cube3HeightTextureId);
  glActiveTexture(GL_TEXTURE12);
  glBindTexture(GL_TEXTURE_2D, lightTextureId);
  const uint32 shadowMap2DSamplerIndex = 13;

  const glm::mat4 cameraProjMat = glm::perspective(glm::radians(camera.Zoom), (float32)windowWidth / (float32)windowHeight, 0.1f, 120.0f);

  const float32 nearPlane = 1.0f, farPlane = 70.0f, projectionDimens = 12.0f;
  // Note: orthographic projection is used for directional lighting, as all light rays are parallel
  const glm::mat4 lightProjMat = glm::ortho(-projectionDimens, projectionDimens, -projectionDimens, projectionDimens, nearPlane, farPlane);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  // background clear color
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  glm::vec3 lightColor(0.15f);
  const float32 lightScale = 8.0f;

  const float32 floorScale = 16.0f;
  const glm::vec3 floorPosition(0.0f, -3.0f, 0.0f);

  const float32 cubeScale2 = 3.0f;

  const float32 cubeScale3 = 1.7f;

  const float32 lightRadius = 48.0f;
  const float32 lightHeightOffset = 20.0f;
  const float32 lightHeightHalfVariance = 12.0f;
  const float32 lightCameraDistance = 32.0f;

  quadTextureShader.use();
  quadTextureShader.setUniform("projection", cameraProjMat);
  quadTextureShader.setUniform("tex", 12);

  directionalLightShader.use();

  // set lighting 1
  directionalLightShader.setUniform("directionalLightColor.ambient", lightColor * 0.05f);
  directionalLightShader.setUniform("directionalLightColor.diffuse", lightColor * 0.5f);
  directionalLightShader.setUniform("directionalLightColor.specular", lightColor * 0.1f);

  directionalLightShader.setUniform("shadowMap", shadowMap2DSamplerIndex);

  uint32 globalVSUniformBuffer;
  uint32 globalVSBufferBindIndex = 0;
  uint32 globalVSBufferViewMatOffset = sizeof(glm::mat4);
  {
    glGenBuffers(1, &globalVSUniformBuffer);

    glBindBuffer(GL_UNIFORM_BUFFER, globalVSUniformBuffer);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);

    glBindBufferRange(GL_UNIFORM_BUFFER,    // target
                      globalVSBufferBindIndex,  // index of binding point
                      globalVSUniformBuffer,  // buffer id
                      0,            // starting offset into buffer object
                      4 * 16);        // size: 4 vec3's, 16 bits alignments

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(cameraProjMat));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    directionalLightShader.bindBlockIndex("globalBlockVS", globalVSBufferBindIndex);
    quadTextureShader.bindBlockIndex("globalBlockVS", globalVSBufferBindIndex);
  }

  glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 10.0f);
  camera.Position = cameraPosition;

  // floor data
  glm::mat4 floorModelMat = glm::mat4();
  floorModelMat = glm::translate(floorModelMat, floorPosition);
  floorModelMat = glm::scale(floorModelMat, glm::vec3(floorScale, floorScale, floorScale));
  floorModelMat = glm::rotate(floorModelMat, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);

  // Turn on gamma correction for entire scene
  //glEnable(GL_FRAMEBUFFER_SRGB);

  // NOTE: render/game loop
  float32 startTime = (float32)glfwGetTime();
  while (glfwWindowShouldClose(window) == GL_FALSE)
  {
    // check for input
    processKeyboardInput(window, this);
    processXInput(this);

    float32 t = (float32)glfwGetTime() - startTime;
    deltaTime = t - lastFrame;
    lastFrame = t;

    glm::mat4 viewMat = camera.GetViewMatrix(deltaTime);

    glBindBuffer(GL_UNIFORM_BUFFER, globalVSUniformBuffer);
    // update global view matrix uniform
    glBufferSubData(GL_UNIFORM_BUFFER, globalVSBufferViewMatOffset, sizeof(glm::mat4), glm::value_ptr(viewMat));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // light data
    glm::vec3 lightPosition = glm::vec3(cos(t/20) * lightRadius, lightHeightOffset + sin(t / 10) * lightHeightHalfVariance, sin(t / 20) * lightRadius);
    glm::mat4 lightModel;
    lightModel = glm::translate(lightModel, lightPosition);
    lightModel = glm::scale(lightModel, glm::vec3(lightScale));
    glm::vec3 lightCameraPosition = glm::normalize(lightPosition) * lightCameraDistance;

    // cube data
    glm::mat4 cubeModelMat1 = glm::mat4();
    const float32 cubeScale1 = 4.0f * ((sin(t) + 1) / 2);
    const glm::vec3 cubePosition1 = glm::vec3(0.0f, floorPosition.y + (cubeScale1 / 2.0f), 0.0f);
    cubeModelMat1 = glm::translate(cubeModelMat1, cubePosition1);
    cubeModelMat1 = glm::scale(cubeModelMat1, glm::vec3(cubeScale1));
    glm::mat4 cubeModelMat2 = glm::mat4();
    const glm::vec3 cubePosition2 = glm::vec3(sin(t / 16.0f) * 8.0f, floorPosition.y + (cubeScale2 / 2) + 2.0f, cos(t / 16.0f) * 8.0f);
    cubeModelMat2 = glm::translate(cubeModelMat2, cubePosition2);
    cubeModelMat2 = glm::rotate(cubeModelMat2, glm::radians(t * 8.0f), glm::vec3(0.4f, 0.6f, 0.3f));
    cubeModelMat2 = glm::scale(cubeModelMat2, glm::vec3(cubeScale2));
    glm::mat4 cubeModelMat3 = glm::mat4();
    const glm::vec3 cubePosition3 = glm::vec3(cos(t / 8.0f) * 4.0f, floorPosition.y + (cubeScale3 / 2.0f) + 4.0f, sin(t / 8.0f) * 4.0f);
    cubeModelMat3 = glm::translate(cubeModelMat3, cubePosition3);
    cubeModelMat3 = glm::rotate(cubeModelMat3, glm::radians(t * 16.0f), glm::vec3(2.7f, -1.0f, 3.0f));
    cubeModelMat3 = glm::scale(cubeModelMat3, glm::vec3(cubeScale3));

    // render depth map from light's point of view
    glm::mat4 lightView = glm::lookAt(lightCameraPosition,
                                      glm::vec3(0.0f, 0.0f, 0.0f),
                                      glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjMat * lightView;


    depthMapShader.use();
    depthMapShader.setUniform("lightSpaceMatrix", lightSpaceMatrix);
    glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    // depth map for floor
    depthMapShader.setUniform("model", floorModelMat);
    glBindVertexArray(quadVAO);
    glDrawElements(GL_TRIANGLES, // drawing mode
                   6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
                   GL_UNSIGNED_INT, // type of the indices
                   0); // offset in the EBO


    // depth map for cubes
    depthMapShader.setUniform("model", cubeModelMat1);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, // drawing mode
                   cubePosNormTexNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                   GL_UNSIGNED_INT, // type of the indices
                   0); // offset in the EBO
    glBindVertexArray(0);

    depthMapShader.setUniform("model", cubeModelMat2);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, // drawing mode
                   cubePosNormTexNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                   GL_UNSIGNED_INT, // type of the indices
                   0); // offset in the EBO
    glBindVertexArray(0);

    depthMapShader.setUniform("model", cubeModelMat3);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, // drawing mode
                   cubePosNormTexNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                   GL_UNSIGNED_INT, // type of the indices
                   0); // offset in the EBO
    glBindVertexArray(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // render scene using the depth map for shadows (using depth map)
    glViewport(0, 0, windowWidth, windowHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0 + shadowMap2DSamplerIndex);
    glBindTexture(GL_TEXTURE_2D, depthMapTextureId);

    // draw directional light representation
    quadTextureShader.use();
    quadTextureShader.setUniform("view", viewMat);
    quadTextureShader.setUniform("model", lightModel);
    glBindVertexArray(quadVAO);
    glDrawElements(GL_TRIANGLES, // drawing mode
                   6, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                   GL_UNSIGNED_INT, // type of the indices
                   0); // offset in the EBO

    glm::vec3 lightDir = glm::normalize(lightPosition);
    directionalLightShader.use();
    directionalLightShader.setUniform("viewPos", camera.Position);
    directionalLightShader.setUniform("lightSpaceMatrix", lightSpaceMatrix);
    directionalLightShader.setUniform("directionalLightDir", lightDir);

    // draw floor
    directionalLightShader.setUniform("model", floorModelMat);
    directionalLightShader.setUniform("material.diffuse", 0);
    directionalLightShader.setUniform("material.normal", 4);
    directionalLightShader.setUniform("material.height", 8);
    glBindVertexArray(quadVAO);
    glDrawElements(GL_TRIANGLES, // drawing mode
                   6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
                   GL_UNSIGNED_INT, // type of the indices
                   0); // offset in the EBO

    // draw cubes
    glBindVertexArray(cubeVAO);
    directionalLightShader.setUniform("model", cubeModelMat1);
    directionalLightShader.setUniform("material.diffuse", 1);
    directionalLightShader.setUniform("material.normal", 5);
    directionalLightShader.setUniform("material.height", 9);
    glDrawElements(GL_TRIANGLES, // drawing mode
                   cubePosNormTexNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                   GL_UNSIGNED_INT, // type of the indices
                   0); // offset in the EBO
    directionalLightShader.setUniform("model", cubeModelMat2);
    directionalLightShader.setUniform("material.diffuse", 2);
    directionalLightShader.setUniform("material.normal", 6);
    directionalLightShader.setUniform("material.height", 10);
    glDrawElements(GL_TRIANGLES, // drawing mode
                   cubePosNormTexNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                   GL_UNSIGNED_INT, // type of the indices
                   0); // offset in the EBO
    directionalLightShader.setUniform("model", cubeModelMat3);
    directionalLightShader.setUniform("material.diffuse", 3);
    directionalLightShader.setUniform("material.normal", 7);
    directionalLightShader.setUniform("material.height", 11);
    glDrawElements(GL_TRIANGLES, // drawing mode
                   cubePosNormTexNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                   GL_UNSIGNED_INT, // type of the indices
                   0); // offset in the EBO
    glBindVertexArray(0);

    glfwSwapBuffers(window); // swaps double buffers
    glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
  }
}

void MoonScene::generateDepthMap(uint32& depthMapTextureId, uint32& depthMapFBO)
{
  glGenFramebuffers(1, &depthMapFBO);

  glGenTextures(1, &depthMapTextureId);
  glBindTexture(GL_TEXTURE_2D, depthMapTextureId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
               SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // ensure that areas outside of the shadow map are NEVER determined to be in shadow
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  float32 borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTextureId, 0);
  // The following two calls tell OpenGL that we are not trying to output any kind of color
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}