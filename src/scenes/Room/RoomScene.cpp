#include "RoomScene.h"
#include "../../common/FileLocations.h"
#include "../../common/Util.h"
#include "../../common/ObjectData.h"

const uint32 SHADOW_MAP_WIDTH = 2048;
const uint32 SHADOW_MAP_HEIGHT = 2048;

RoomScene::RoomScene() : FirstPersonScene()
{
  camera.Position = glm::vec3(0.0f, 5.0f, 30.0f);
}

const char* RoomScene::title()
{
  return "Room w/ Dynamic Point Light";
}

void RoomScene::init(uint32 windowWidth, uint32 windowHeight)
{
  FirstPersonScene::init(windowWidth, windowHeight);
  
  positionalLightShader = new Shader(posNormTexVertexShaderFileLoc, positionalLightShadowMapFragmentShaderFileLoc);
  singleColorShader = new Shader(posGlobalBlockVertexShaderFileLoc, SingleColorFragmentShaderFileLoc);
  depthCubeMapShader = new Shader(modelMatVertexShaderFileLoc, linearDepthMapFragmentShaderFileLoc, cubeMapGeometryShaderFileLoc);
  
  cubeVertexAtt = initializeCubePosNormTexVertexAttBuffers();
  invertedNormCubeVertexAtt = initializeCubePosNormTexVertexAttBuffers(true);

  load2DTexture(hardwoodTextureLoc, wallpaperTextureId, false, true);
  load2DTexture(cementAlbedoTextureLoc, cubeTextureId, false, true);
  generateDepthCubeMap();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, wallpaperTextureId);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, cubeTextureId);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMapId);

  float32 cameraAspectRatio = (float32)windowWidth / (float32)windowHeight;
  const float32 cameraNearPlane = 0.1f;
  const float32 cameraFarPlane = 120.f;
  const glm::mat4 cameraProjMat = glm::perspective(glm::radians(camera.Zoom), cameraAspectRatio, cameraNearPlane, cameraFarPlane);

  const float32 lightAspectRatio = (float32)SHADOW_MAP_WIDTH / (float32)SHADOW_MAP_HEIGHT;
  const float32 lightNearPlane = 1.0f;
  const float32 lightFarPlane = 40.0f;
  lightProjMat = glm::perspective(glm::radians(90.0f), lightAspectRatio, lightNearPlane, lightFarPlane);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  // background clear color
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

  // Turn on free gamma correction for entire scene, only affects color attachments
  glEnable(GL_FRAMEBUFFER_SRGB);

  // set constant uniforms
  positionalLightShader->use();
  // set light attenuation
  positionalLightShader->setUniform("attenuation.constant", 1.0f);
  positionalLightShader->setUniform("attenuation.linear", 0.02f);
  positionalLightShader->setUniform("attenuation.quadratic", 0.008f);
  // set lighting
  positionalLightShader->setUniform("positionalLight.color.ambient", lightColor * 0.05f);
  positionalLightShader->setUniform("positionalLight.color.diffuse", lightColor * 0.3f);
  positionalLightShader->setUniform("positionalLight.color.specular", lightColor);
  positionalLightShader->setUniform("lightFarPlane", lightFarPlane);
  // set shadow cube map texture location
  positionalLightShader->setUniform("shadowCubeMap", depthMap2DSamplerIndex);

  depthCubeMapShader->use();
  depthCubeMapShader->setUniform("lightFarPlane", lightFarPlane);

  // initialize global uniform buffer with camera projection matrix
  glGenBuffers(1, &globalVSUniformBuffer);
  glBindBuffer(GL_UNIFORM_BUFFER, globalVSUniformBuffer);
  glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
  glBindBufferRange(GL_UNIFORM_BUFFER,    // target
                    globalVSBufferBindIndex,  // index of binding point
                    globalVSUniformBuffer,  // buffer id
                    0,            // starting offset into buffer object
                    4 * 16);        // size: 4 vec3's, 16 bits alignments
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(cameraProjMat));

  positionalLightShader->bindBlockIndex("globalBlockVS", globalVSBufferBindIndex);
  singleColorShader->bindBlockIndex("globalBlockVS", globalVSBufferBindIndex);

  // room data
  roomModelMat = glm::translate(glm::mat4(), roomPosition);
  roomModelMat = glm::scale(roomModelMat, glm::vec3(roomScale));

  // cube data
  cubeModelMat[0] = glm::translate(glm::mat4(), cubePositions[0]);
  cubeModelMat[0] = glm::scale(cubeModelMat[0], glm::vec3(cubeScales[0]));
  cubeModelMat[1] = glm::translate(glm::mat4(), cubePositions[1]);
  cubeModelMat[1] = glm::scale(cubeModelMat[1], glm::vec3(cubeScales[1]));
  cubeModelMat[2] = glm::translate(glm::mat4(), cubePositions[2]);
  cubeModelMat[2] = glm::scale(cubeModelMat[2], glm::vec3(cubeScales[2]));

  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);
}

void RoomScene::deinit()
{
  FirstPersonScene::deinit();

  positionalLightShader->deleteShaderResources();
  singleColorShader->deleteShaderResources();
  depthCubeMapShader->deleteShaderResources();
  delete positionalLightShader;
  delete singleColorShader;
  delete depthCubeMapShader;
  
  VertexAtt deleteVertexAttributes[] = { cubeVertexAtt, invertedNormCubeVertexAtt };
  deleteVertexAtts(ArrayCount(deleteVertexAttributes), deleteVertexAttributes);
  
  uint32 deleteTextures[] = { wallpaperTextureId, cubeTextureId, depthCubeMapId };
  glDeleteTextures(ArrayCount(deleteTextures), deleteTextures);

  glDeleteBuffers(1, &globalVSUniformBuffer);

  glDeleteFramebuffers(1, &depthMapFBO);

  glDisable(GL_FRAMEBUFFER_SRGB);
}

void RoomScene::drawFrame()
{
  FirstPersonScene::drawFrame();
  float32 t = (float32)glfwGetTime();
  deltaTime = t - lastFrame;
  lastFrame = t;

  glm::mat4 viewMat = camera.UpdateViewMatrix(deltaTime, cameraMovementSpeed * 4.0f, false);

  // update global view matrix uniform
  glBufferSubData(GL_UNIFORM_BUFFER, globalVSBufferViewMatOffset, sizeof(glm::mat4), glm::value_ptr(viewMat));

  // light data
  glm::vec3 lightPosition = glm::vec3(sin(t) * lightRadius, sin(1.5f * t) * lightAmplitude, cos(t) * lightRadius);
  glm::mat4 lightModel;
  lightModel = glm::translate(lightModel, lightPosition);
  lightModel = glm::scale(lightModel, glm::vec3(lightScale));

  glm::mat4 shadowMapTransMats[] = {
          (lightProjMat * glm::lookAt(lightPosition, lightPosition + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0))), // right
          (lightProjMat * glm::lookAt(lightPosition, lightPosition + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0))), // left
          (lightProjMat * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0))), // top
          (lightProjMat * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0))), // bottom
          (lightProjMat * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0))), // far
          (lightProjMat * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0))) // near
  };

  depthCubeMapShader->use();
  depthCubeMapShader->setUniform("cubeMapTransMats", shadowMapTransMats, 6);
  depthCubeMapShader->setUniform("lightPos", lightPosition);

  glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glClear(GL_DEPTH_BUFFER_BIT);

  // depth map for room
  glCullFace(GL_FRONT);
  glBindVertexArray(invertedNormCubeVertexAtt.arrayObject);
  depthCubeMapShader->setUniform("model", roomModelMat);
  glDrawElements(GL_TRIANGLES, // drawing mode
                 cubePosNormTexNumElements * 3, // number of elements to draw * 3 vertices per triangle
                 GL_UNSIGNED_INT, // type of the indices
                 0); // offset in the EBO

  // depth map for cubes
  glCullFace(GL_BACK);
  glBindVertexArray(cubeVertexAtt.arrayObject);
  depthCubeMapShader->setUniform("model", cubeModelMat[0]);
  glDrawElements(GL_TRIANGLES, // drawing mode
                 cubePosNormTexNumElements * 3, // number of elements to draw * 3 vertices per triangle
                 GL_UNSIGNED_INT, // type of the indices
                 0); // offset in the EBO
  depthCubeMapShader->setUniform("model", cubeModelMat[1]);
  glDrawElements(GL_TRIANGLES, // drawing mode
                 cubePosNormTexNumElements * 3, // number of elements to draw * 3 vertices per triangle
                 GL_UNSIGNED_INT, // type of the indices
                 0); // offset in the EBO
  depthCubeMapShader->setUniform("model", cubeModelMat[2]);
  glDrawElements(GL_TRIANGLES, // drawing mode
                 cubePosNormTexNumElements * 3, // number of elements to draw * 3 vertices per triangle
                 GL_UNSIGNED_INT, // type of the indices
                 0); // offset in the EBO

  // bind default frame buffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  // render scene using the depth cube map for shadows
  glViewport(0, 0, windowWidth, windowHeight);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // draw positional light
  singleColorShader->use();
  singleColorShader->setUniform("model", lightModel);
  singleColorShader->setUniform("color", lightColor);
  glDrawElements(GL_TRIANGLES, // drawing mode
                 cubePosNormTexNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                 GL_UNSIGNED_INT, // type of the indices
                 0); // offset in the EBO

  positionalLightShader->use();
  positionalLightShader->setUniform("viewPos", camera.Position);
  positionalLightShader->setUniform("positionalLight.position", lightPosition);

  // draw room
  glCullFace(GL_FRONT);
  glBindVertexArray(invertedNormCubeVertexAtt.arrayObject);
  positionalLightShader->setUniform("model", roomModelMat);
  positionalLightShader->setUniform("material.diffTexture1", 0);
  positionalLightShader->setUniform("material.specTexture1", 0);
  glDrawElements(GL_TRIANGLES, // drawing mode
                 cubePosNormTexNumElements * 3, // number of elements to draw * 3 vertices per triangle
                 GL_UNSIGNED_INT, // type of the indices
                 0); // offset in the EBO

  // draw cubes
  glCullFace(GL_BACK);
  glBindVertexArray(cubeVertexAtt.arrayObject);
  positionalLightShader->setUniform("model", cubeModelMat[0]);
  positionalLightShader->setUniform("material.diffTexture1", 1);
  positionalLightShader->setUniform("material.specTexture1", 1);
  glDrawElements(GL_TRIANGLES, // drawing mode
                 cubePosNormTexNumElements * 3, // number of elements to draw * 3 vertices per triangle
                 GL_UNSIGNED_INT, // type of the indices
                 0); // offset in the EBO
  positionalLightShader->setUniform("model", cubeModelMat[1]);
  glDrawElements(GL_TRIANGLES, // drawing mode
                 cubePosNormTexNumElements * 3, // number of elements to draw * 3 vertices per triangle
                 GL_UNSIGNED_INT, // type of the indices
                 0); // offset in the EBO
  positionalLightShader->setUniform("model", cubeModelMat[2]);
  glDrawElements(GL_TRIANGLES, // drawing mode
                 cubePosNormTexNumElements * 3, // number of elements to draw * 3 vertices per triangle
                 GL_UNSIGNED_INT, // type of the indices
                 0); // offset in the EBO
}

void RoomScene::generateDepthCubeMap()
{
  glGenFramebuffers(1, &depthMapFBO);

  glGenTextures(1, &depthCubeMapId);
  glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMapId);
  for (uint32 i = 0; i < 6; ++i)
  {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                 SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  }

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