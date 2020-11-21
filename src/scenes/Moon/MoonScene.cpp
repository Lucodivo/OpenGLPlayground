#include "MoonScene.h"
#include "../../common/FileLocations.h"
#include "../../common/Util.h"
#include "../../common/ObjectData.h"

const uint32 SHADOW_MAP_WIDTH = 2048;
const uint32 SHADOW_MAP_HEIGHT = 2048;

MoonScene::MoonScene() : FirstPersonScene()
{
  camera = Camera({-25.0f, 10.0f, -25.0f}, {0.0f, 1.0f, 0.0f}, 45.0f, -12.0f);
}

const char* MoonScene::title()
{
  return "Moon : Parallax : Bump";
}

void MoonScene::init(Extent2D windowExtent)
{
  FirstPersonScene::init(windowExtent);

  directionalLightShader = new ShaderProgram(lightSpaceVertexShaderFileLoc, directionalLightShadowMapFragmentShaderFileLoc, tbnGeometryShaderFileLoc);
  quadTextureShader = new ShaderProgram(billboardPosTexVertexShaderFileLoc, textureFragmentShaderFileLoc);
  depthMapShader = new ShaderProgram(simpleDepthVertexShaderFileLoc, emptyFragmentShaderFileLoc);

  floorVertexAtt = initializeQuadPosNormTexVertexAttBuffers();
  cubeVertexAtt = initializeCubePosNormTexVertexAttBuffers();
  
  load2DTexture(dungeonStoneAlbedoTextureLoc, floorAlbedoTextureId, false, true);
  load2DTexture(dungeonStoneNormalTextureLoc, floorNormalTextureId, false, false);
  load2DTexture(dungeonStoneHeightTextureLoc, floorHeightTextureId, false, false);
  
  load2DTexture(waterWornStoneAlbedoTextureLoc, cube1AlbedoTextureId, false, true);
  load2DTexture(waterWornStoneNormalTextureLoc, cube1NormalTextureId, false, false);
  load2DTexture(waterWornStoneHeightTextureLoc, cube1HeightTextureId, false, false);
  
  load2DTexture(copperRockAlbedoTextureLoc, cube2AlbedoTextureId, false, true);
  load2DTexture(copperRockNormalTextureLoc, cube2NormalTextureId, false, false);
  load2DTexture(copperRockHeightTextureLoc, cube2HeightTextureId, false, false);
  
  load2DTexture(whiteSpruceAlbedoTextureLoc, cube3AlbedoTextureId, false, true);
  load2DTexture(whiteSpruceNormalTextureLoc, cube3NormalTextureId, false, false);
  load2DTexture(whiteSpruceHeightTextureLoc, cube3HeightTextureId, false, false);
  
  load2DTexture(moonTextureAlbedoLoc, lightTextureId, false, true);

  generateDepthMap();
  drawFramebuffer = initializeFramebuffer(windowExtent);

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
  depthMap2DSamplerIndex = 13;

  const float32 aspectRatio = (float32)windowExtent.width / (float32)windowExtent.height;
  const glm::mat4 cameraProjMat = glm::perspective(glm::radians(camera.Zoom), aspectRatio, 0.1f, 120.0f);
  const float32 nearPlane = 1.0f;
  const float32 farPlane = 70.0f;
  const float32 projectionDimens = 12.0f;
  // Note: orthographic projection is used for directional lighting, as all light rays are parallel
  lightProjMat = glm::ortho(-projectionDimens, projectionDimens, -projectionDimens, projectionDimens, nearPlane, farPlane);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  // background clear color
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  quadTextureShader->use();
  quadTextureShader->setUniform("projection", cameraProjMat);
  quadTextureShader->setUniform("tex", 12);

  directionalLightShader->use();

  // set lighting 1
  directionalLightShader->setUniform("directionalLightColor.ambient", lightColor * 0.05f);
  directionalLightShader->setUniform("directionalLightColor.diffuse", lightColor * 0.2f);
  directionalLightShader->setUniform("directionalLightColor.specular", lightColor * 0.1f);
  directionalLightShader->setUniform("shadowMap", depthMap2DSamplerIndex);

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

    directionalLightShader->bindBlockIndex("globalBlockVS", globalVSBufferBindIndex);
    quadTextureShader->bindBlockIndex("globalBlockVS", globalVSBufferBindIndex);
  }

  // floor data
  floorModelMat = glm::mat4();
  floorModelMat = glm::translate(floorModelMat, floorPosition);
  floorModelMat = glm::scale(floorModelMat, glm::vec3(floorScale, floorScale, floorScale));
  floorModelMat = glm::rotate(floorModelMat, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);

  startTime = getTime();
}

void MoonScene::deinit()
{
  FirstPersonScene::deinit();

  directionalLightShader->deleteShaderResources();
  quadTextureShader->deleteShaderResources();
  depthMapShader->deleteShaderResources();
  delete directionalLightShader;
  delete quadTextureShader;
  delete depthMapShader;

  VertexAtt deleteVertexAttributes[] = { floorVertexAtt, cubeVertexAtt };
  deleteVertexAtts(ArrayCount(deleteVertexAttributes), deleteVertexAttributes);

  uint32 deleteTextures[] = { floorAlbedoTextureId, floorNormalTextureId, floorHeightTextureId,
                              cube1AlbedoTextureId, cube1NormalTextureId, cube1HeightTextureId,
                              cube2AlbedoTextureId, cube2NormalTextureId, cube2HeightTextureId,
                              cube3AlbedoTextureId, cube3NormalTextureId, cube3HeightTextureId,
                              lightTextureId,
                              depthMapFramebuffer.depthStencilAttachment };
  glDeleteTextures(ArrayCount(deleteTextures), deleteTextures);

  glDeleteBuffers(1, &globalVSUniformBuffer);
  glDeleteFramebuffers(1, &depthMapFramebuffer.id);
  deleteFramebuffer(&drawFramebuffer);
  depthMapFramebuffer = { 0, 0, 0, 0, 0 };
}

Framebuffer MoonScene::drawFrame()
{
  float32 t = getTime() - startTime;
  deltaTime = t - lastFrame;
  lastFrame = t;

  glm::mat4 viewMat = camera.UpdateViewMatrix(deltaTime, cameraMovementSpeed * 4.0f, false);

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


  depthMapShader->use();
  depthMapShader->setUniform("lightSpaceMatrix", lightSpaceMatrix);
  glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFramebuffer.id);
  glClear(GL_DEPTH_BUFFER_BIT);

  // depth map for floor
  depthMapShader->setUniform("model", floorModelMat);
  glBindVertexArray(floorVertexAtt.arrayObject);
  glDrawElements(GL_TRIANGLES, // drawing mode
                 6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
                 GL_UNSIGNED_INT, // type of the indices
                 0); // offset in the EBO

  // depth map for cubes
  depthMapShader->setUniform("model", cubeModelMat1);
  glBindVertexArray(cubeVertexAtt.arrayObject);
  glDrawElements(GL_TRIANGLES, // drawing mode
                 cubePosNormTexNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                 GL_UNSIGNED_INT, // type of the indices
                 0); // offset in the EBO

  depthMapShader->setUniform("model", cubeModelMat2);
  glDrawElements(GL_TRIANGLES, // drawing mode
                 cubePosNormTexNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                 GL_UNSIGNED_INT, // type of the indices
                 0); // offset in the EBO

  depthMapShader->setUniform("model", cubeModelMat3);
  glDrawElements(GL_TRIANGLES, // drawing mode
                 cubePosNormTexNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                 GL_UNSIGNED_INT, // type of the indices
                 0); // offset in the EBO

  // render scene using the depth map for shadows (using depth map)
  glBindFramebuffer(GL_FRAMEBUFFER, drawFramebuffer.id);
  glViewport(0, 0, windowExtent.width, windowExtent.height);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glActiveTexture(GL_TEXTURE0 + depthMap2DSamplerIndex);
  glBindTexture(GL_TEXTURE_2D, depthMapFramebuffer.depthStencilAttachment);

  // draw moon billboard (directional light representation)
  quadTextureShader->use();
  quadTextureShader->setUniform("view", viewMat);
  quadTextureShader->setUniform("model", lightModel);
  glBindVertexArray(floorVertexAtt.arrayObject);
  glDrawElements(GL_TRIANGLES, // drawing mode
                 6, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                 GL_UNSIGNED_INT, // type of the indices
                 0); // offset in the EBO

  glm::vec3 lightDir = glm::normalize(lightPosition);
//  if(directionalLightShader->updateShadersWhenOutdated(FragmentShaderFlag)) {
//    directionalLightShader->use();
//    directionalLightShader->setUniform("directionalLightColor.ambient", lightColor * 0.00f);
//    directionalLightShader->setUniform("directionalLightColor.diffuse", lightColor * 1.0f);
//    directionalLightShader->setUniform("directionalLightColor.specular", lightColor * 0.0f);
//    directionalLightShader->setUniform("shadowMap", depthMap2DSamplerIndex);
//    directionalLightShader->bindBlockIndex("globalBlockVS", globalVSBufferBindIndex);
//  }
  directionalLightShader->use();
  directionalLightShader->setUniform("viewPos", camera.Position);
  directionalLightShader->setUniform("lightSpaceMatrix", lightSpaceMatrix);
  directionalLightShader->setUniform("directionalLightDir", lightDir);

  // draw floor
  directionalLightShader->setUniform("model", floorModelMat);
  directionalLightShader->setUniform("material.diffuse", 0);
  directionalLightShader->setUniform("material.normal", 4);
  directionalLightShader->setUniform("material.height", 8);
  glDrawElements(GL_TRIANGLES, // drawing mode
                 6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
                 GL_UNSIGNED_INT, // type of the indices
                 0); // offset in the EBO

  // draw cubes
  glBindVertexArray(cubeVertexAtt.arrayObject);
  directionalLightShader->setUniform("model", cubeModelMat1);
  directionalLightShader->setUniform("material.diffuse", 1);
  directionalLightShader->setUniform("material.normal", 5);
  directionalLightShader->setUniform("material.height", 9);
  glDrawElements(GL_TRIANGLES, // drawing mode
                 cubePosNormTexNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                 GL_UNSIGNED_INT, // type of the indices
                 0); // offset in the EBO
  directionalLightShader->setUniform("model", cubeModelMat2);
  directionalLightShader->setUniform("material.diffuse", 2);
  directionalLightShader->setUniform("material.normal", 6);
  directionalLightShader->setUniform("material.height", 10);
  glDrawElements(GL_TRIANGLES, // drawing mode
                 cubePosNormTexNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                 GL_UNSIGNED_INT, // type of the indices
                 0); // offset in the EBO
  directionalLightShader->setUniform("model", cubeModelMat3);
  directionalLightShader->setUniform("material.diffuse", 3);
  directionalLightShader->setUniform("material.normal", 7);
  directionalLightShader->setUniform("material.height", 11);
  glDrawElements(GL_TRIANGLES, // drawing mode
                 cubePosNormTexNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                 GL_UNSIGNED_INT, // type of the indices
                 0); // offset in the EBO

   return drawFramebuffer;
}

void MoonScene::generateDepthMap()
{
  glGenFramebuffers(1, &depthMapFramebuffer.id);
  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFramebuffer.id);

  glGenTextures(1, &depthMapFramebuffer.depthStencilAttachment);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, depthMapFramebuffer.depthStencilAttachment);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // ensure that areas outside of the shadow map are NEVER determined to be in shadow
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  float32 borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapFramebuffer.depthStencilAttachment, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

  // The following two calls tell OpenGL that we are not trying to output any kind of color
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, drawFramebuffer.id);


  depthMapFramebuffer.colorAttachment = NO_FRAMEBUFFER_ATTACHMENT;
  depthMapFramebuffer.extent.width = SHADOW_MAP_WIDTH;
  depthMapFramebuffer.extent.height = SHADOW_MAP_HEIGHT;
}

void MoonScene::framebufferSizeChangeRequest(Extent2D windowExtent)
{
  Scene::framebufferSizeChangeRequest(windowExtent);

  deleteFramebuffer(&drawFramebuffer);
  drawFramebuffer = initializeFramebuffer(windowExtent);
  // NOTE: depth map framebuffer is fine as is
}
