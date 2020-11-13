#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <glm/gtc/matrix_transform.hpp>

#include "../../common/FileLocations.h"
#include "../../common/Util.h"

#include "KernelScene.h"

// ===== cube values =====
const glm::vec3 cubePositions[] = {
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
};

const float32 cubeScales[] = {
        0.6f,
        0.65f,
        0.7f,
        0.75f,
        0.8f,
        0.85f,
        0.9f,
        0.95f,
        1.0f
};
// ===== cube values =====

KernelScene::KernelScene(): FirstPersonScene()
{
  camera.Position = glm::vec3(0.0f, 0.0f, 6.0f);
}

const char* KernelScene::title()
{
  return "Image Kernel";
}

void KernelScene::init(uint32 windowWidth, uint32 windowHeight)
{
  FirstPersonScene::init(windowWidth, windowHeight);

  glm::mat4 projectionMat = glm::perspective(glm::radians(camera.Zoom), (float32)windowWidth / (float32)windowHeight, 0.1f, 100.0f);

  cubeShader = new Shader(posNormTexVertexShaderFileLoc, nessCubeFragmentShaderFileLoc);
  lightShader = new Shader(posGlobalBlockVertexShaderFileLoc, SingleColorFragmentShaderFileLoc);
  modelShader = new Shader(posNormTexVertexShaderFileLoc, dirPosSpotLightModelFragmentShaderFileLoc);
  stencilShader = new Shader(posNormTexVertexShaderFileLoc, SingleColorFragmentShaderFileLoc);
  frameBufferShader = new Shader(frameBufferVertexShaderFileLoc, kernel5x5TextureFragmentShaderFileLoc);
  skyboxShader = new Shader(skyboxVertexShaderFileLoc, skyboxFragmentShaderFileLoc);

  lightVertexAtt = initializeCubePositionVertexAttBuffers();
  cubeVertexAtt = initializeCubePosNormTexVertexAttBuffers();
  quadVertexAtt = initializeFrameBufferQuadVertexAttBuffers();
  skyboxVertexAtt = initializeCubePositionVertexAttBuffers();

  initializeTextures(cubeDiffTextureId, cubeSpecTextureId, skyboxTextureId);

  framebuffer = initializeFrameBuffer(windowWidth, windowHeight);

  nanoSuitModel = new Model(nanoSuitModelLoc);

  glm::vec3 directionalLightDir = glm::vec3(1.0f, -0.5f, 1.0f);
  glm::vec3 directionalLightColor = glm::vec3(1.0f);

  // background clear color
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);

  auto setConstantLightUniforms = [&](Shader* shader)
  {
    // positional light constants
    shader->setUniform("positionalLight.attenuation.constant", 1.0f);
    shader->setUniform("positionalLight.attenuation.linear", 0.09f);
    shader->setUniform("positionalLight.attenuation.quadratic", 0.032f);

    // flashlight constants
    shader->setUniform("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    shader->setUniform("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
    shader->setUniform("spotLight.attenuation.constant", 1.0f);
    shader->setUniform("spotLight.attenuation.linear", 0.09f);
    shader->setUniform("spotLight.attenuation.quadratic", 0.032f);
  };

  glGenBuffers(1, &globalVSUniformBuffer);
  glBindBuffer(GL_UNIFORM_BUFFER, globalVSUniformBuffer);
  glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
  glBindBufferRange(GL_UNIFORM_BUFFER,    // target
                    globalVSBufferBindIndex,  // index of binding point
                    globalVSUniformBuffer,  // buffer id
                    0,            // starting offset into buffer object
                    4 * 16);        // size: 4 vec3's, 16 bits alignments
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projectionMat));

  cubeShader->bindBlockIndex("globalBlockVS", globalVSBufferBindIndex);
  modelShader->bindBlockIndex("globalBlockVS", globalVSBufferBindIndex);
  lightShader->bindBlockIndex("globalBlockVS", globalVSBufferBindIndex);
  stencilShader->bindBlockIndex("globalBlockVS", globalVSBufferBindIndex);

  glGenBuffers(1, &globalFSUniformBuffer);
  glBindBuffer(GL_UNIFORM_BUFFER, globalFSUniformBuffer);
  glBufferData(GL_UNIFORM_BUFFER, 64, NULL, GL_STATIC_DRAW);
  glBindBufferRange(GL_UNIFORM_BUFFER,    // target
                    globalFSBufferBindIndex,  // index of binding point
                    globalFSUniformBuffer,  // buffer id
                    0,            // starting offset into buffer object
                    4 * 16);          // size: 4 vec3's, 16 bits alignments
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(directionalLightDir));
  glBufferSubData(GL_UNIFORM_BUFFER, 16, sizeof(glm::vec3), glm::value_ptr(directionalLightColor * 0.1f)); // ambient
  glBufferSubData(GL_UNIFORM_BUFFER, 32, sizeof(glm::vec3), glm::value_ptr(directionalLightColor * 0.3f)); // diffuse
  glBufferSubData(GL_UNIFORM_BUFFER, 48, sizeof(glm::vec3), glm::value_ptr(directionalLightColor * 0.6f)); // specular

  cubeShader->bindBlockIndex("globalBlockFS", globalFSBufferBindIndex);
  modelShader->bindBlockIndex("globalBlockFS", globalFSBufferBindIndex);

  glBindBuffer(GL_UNIFORM_BUFFER, 0); // unbind uniform buffers

  nanoSuitModelMatrix = glm::scale(glm::mat4(), glm::vec3(modelScale));  // it's a bit too big for our scene, so scale it down
  nanoSuitModelMatrix = glm::translate(nanoSuitModelMatrix, glm::vec3(0.0f, -6.0f, 0.0f)); // translate it down so it's at the center of the scene

  cubeShader->use();
  setConstantLightUniforms(cubeShader);
  cubeShader->setUniform("material.shininess", 32.0f);

  modelShader->use();
  setConstantLightUniforms(modelShader);
  modelShader->setUniform("material.shininess", 32.0f);
  modelShader->setUniform("model", nanoSuitModelMatrix);

  skyboxShader->use();
  skyboxShader->setUniform("projection", projectionMat);

  frameBufferShader->use();
  frameBufferShader->setUniform("textureWidth", (float32)windowWidth);
  frameBufferShader->setUniform("textureHeight", (float32)windowHeight);
}

void KernelScene::initializeTextures(uint32& diffTextureId, uint32& specTextureId, uint32& skyboxTextureId)
{
  load2DTexture(diffuseTextureLoc, diffTextureId, true, true);
  load2DTexture(specularTextureLoc, specTextureId, true, false);
  loadCubeMapTexture(skyboxWaterFaceLocations, skyboxTextureId);
}

void KernelScene::deinit(){
  FirstPersonScene::deinit();

  cubeShader->deleteShaderResources();
  lightShader->deleteShaderResources();
  modelShader->deleteShaderResources();
  stencilShader->deleteShaderResources();
  frameBufferShader->deleteShaderResources();
  skyboxShader->deleteShaderResources();
  delete cubeShader;
  delete lightShader;
  delete modelShader;
  delete stencilShader;
  delete frameBufferShader;
  delete skyboxShader;

  VertexAtt deleteVertexAttributes[] = { lightVertexAtt, cubeVertexAtt, quadVertexAtt, skyboxVertexAtt };
  deleteVertexAtts(ArrayCount(deleteVertexAttributes), deleteVertexAttributes);

  uint32 deleteTextures[] = { cubeDiffTextureId, cubeSpecTextureId, skyboxTextureId };
  glDeleteTextures(ArrayCount(deleteTextures), deleteTextures);

  deleteFrameBuffer(framebuffer);

  delete nanoSuitModel;

  uint32 deleteBuffers[] = { globalVSUniformBuffer, globalFSUniformBuffer };
  glDeleteBuffers(ArrayCount(deleteBuffers), deleteBuffers);
}

void KernelScene::drawFrame(){
  FirstPersonScene::drawFrame();

  // bind our frame buffer
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.id);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glEnable(GL_STENCIL_TEST);
  glStencilOp(GL_KEEP, // when stencil fails
              GL_KEEP, // when stencil passes but depth fails
              GL_REPLACE); // when stencil passes and depth passes
  glStencilFunc(GL_ALWAYS, // stencil function
                1, // reference value for stencil test
                0xFF); // mask that is ANDed with stencil value and reference value before the test compares them
  glStencilMask(0xFF); // mask that is ANDed with the stencil value that is about to be written to stencil buffer

#if 0
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);           // OpenGL state-using function
#else
  // FUN MODE - WINDOWS XP
    glClear(GL_DEPTH_BUFFER_BIT);
#endif

  // if flashlight is off, simply remove all color from light
  glm::vec3 flashLightColor = flashLightOn ? glm::vec3(1.0f, 1.0f, 1.0f) : glm::vec3(0.0f);

  float32 t = (float32)glfwGetTime();
  deltaTime = t - lastFrame;
  lastFrame = t;
  float32 sineVal = sinf(t);
  float32 lightR = (sinf((t + 30.0f) / 3.0f) / 2.0f) + 0.5f;
  float32 lightG = (sinf((t + 60.0f) / 8.0f) / 2.0f) + 0.5f;
  float32 lightB = (sinf(t / 17.0f) / 2.0f) + 0.5f;
  glm::vec3 positionalLightColor(lightR, lightG, lightB);

  glm::mat4 viewMat = camera.UpdateViewMatrix(deltaTime, cameraMovementSpeed);

  glBindBuffer(GL_UNIFORM_BUFFER, globalVSUniformBuffer);
  // update global view matrix uniform
  glBufferSubData(GL_UNIFORM_BUFFER, globalVSBufferViewMatOffset, sizeof(glm::mat4), glm::value_ptr(viewMat));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  // oscillate with time
  const glm::vec3 lightPosition = glm::vec3(lightOrbitRadius * sinf(t * lightOrbitSpeed), sineVal, lightOrbitRadius * cosf(t * lightOrbitSpeed));
  // orbit with time
  glm::mat4 lightModelMat; // default constructor is identity matrix
  lightModelMat = glm::translate(lightModelMat, lightPosition);
  lightModelMat = glm::scale(lightModelMat, glm::vec3(lightScale));

  // draw positional light
  lightShader->use();
  glBindVertexArray(lightVertexAtt.arrayObject);

  lightShader->setUniform("model", lightModelMat);
  lightShader->setUniform("color", positionalLightColor);
  glDrawElements(GL_TRIANGLES, // drawing mode
                 cubePosNormTexNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                 GL_UNSIGNED_INT, // type of the indices
                 0); // offset in the EBO
  glBindVertexArray(0);

  auto setDynamicLightUniforms = [&](Shader* shader)
  {
    // positional light (orbiting light)
    shader->setUniform("positionalLight.position", lightPosition);
    shader->setUniform("positionalLight.color.ambient", positionalLightColor * 0.05f);
    shader->setUniform("positionalLight.color.diffuse", positionalLightColor * 0.3f);
    shader->setUniform("positionalLight.color.specular", positionalLightColor * 1.0f);

    // flash light
    shader->setUniform("spotLight.position", camera.Position);
    shader->setUniform("spotLight.direction", camera.Front);
    shader->setUniform("spotLight.color.ambient", flashLightColor * 0.05f);
    shader->setUniform("spotLight.color.diffuse", flashLightColor * 0.3f);
    shader->setUniform("spotLight.color.specular", flashLightColor * 0.5f);
  };

  // draw skybox
  skyboxShader->use();

  glBindVertexArray(skyboxVertexAtt.arrayObject);

  glEnable(GL_DEPTH_TEST);
  //glStencilMask(0xFF); // mask that is ANDed with the stencil value that is about to be written to stencil buffer
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureId);
  skyboxShader->setUniform("skybox", 0);

  glm::mat4 viewMinusTranslation = glm::mat4(glm::mat3(viewMat));
  skyboxShader->setUniform("view", viewMinusTranslation);

  glDrawElements(GL_TRIANGLES, // drawing mode
                 36, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                 GL_UNSIGNED_INT, // type of the indices
                 0); // offset in the EBO

  // draw cubes
  cubeShader->use();
  glBindVertexArray(cubeVertexAtt.arrayObject);

  setDynamicLightUniforms(cubeShader);

  // switch between two images over time
  bool animSwitch = sin(8 * t) > 0;

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, cubeDiffTextureId);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, cubeSpecTextureId);
  cubeShader->setUniform("material.diffTexture1", 0);
  cubeShader->setUniform("material.specTexture1", 1);
  cubeShader->setUniform("animSwitch", animSwitch);
  cubeShader->setUniform("viewPos", camera.Position);
  cubeShader->setUniform("view", viewMat);

  glm::mat4 cubeModelMat[ArrayCount(cubePositions)];
  for (uint32 i = 0; i < ArrayCount(cubeModelMat); i++)
  {
    float32 angularSpeed = 7.3f * (i + 1);

    // orbit around the specified axis from the translated distance
    cubeModelMat[i] = glm::rotate(glm::mat4(), t * glm::radians(angularSpeed), glm::vec3(50.0f - (i * 10), 100.0f, -50.0f + (i * 10)));
    // translate to position in world
    cubeModelMat[i] = glm::translate(cubeModelMat[i], cubePositions[i]);
    // rotate with time
    cubeModelMat[i] = glm::rotate(cubeModelMat[i], t * glm::radians(angularSpeed), glm::vec3(1.0f, 0.3f, 0.5f));
    // scale object
    cubeModelMat[i] = glm::scale(cubeModelMat[i], glm::vec3(cubeScales[i]));
  }

  // TODO: Sort cube models by distance to camera position and draw back to front
  for (uint32 i = 0; i < ArrayCount(cubeModelMat); i++)
  {
    cubeShader->setUniform("model", cubeModelMat[i]);
    glCullFace(GL_FRONT);
    glDrawElements(GL_TRIANGLES, // drawing mode
                   cubePosNormTexNumElements *
                   3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                   GL_UNSIGNED_INT, // type of the indices
                   0); // offset in the EBO
    glCullFace(GL_BACK);
    glDrawElements(GL_TRIANGLES, // drawing mode
                   cubePosNormTexNumElements *
                   3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                   GL_UNSIGNED_INT, // type of the indices
                   0); // offset in the EBO
  }

  // draw cube stencil outlines
  stencilShader->use();
  stencilShader->setUniform("color", glm::vec3(1.0f, 1.0f, 1.0f));
  stencilShader->setUniform("view", viewMat);
  for (uint32 i = 0; i < ArrayCount(cubeModelMat); i++)
  {
    stencilShader->setUniform("model", cubeModelMat[i]);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLES, // drawing mode
                   cubePosNormTexNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                   GL_UNSIGNED_INT, // type of the indices
                   0); // offset in the EBO
    glEnable(GL_DEPTH_TEST);
  }

  glStencilFunc(GL_ALWAYS, // stencil function
                1, // reference value for stencil test
                0xFF); // mask that is ANDed with stencil value and reference value before the test compares them
  glStencilMask(0xFF); // mask that is ANDed with the stencil value that is about to be written to stencil buffer
  glClear(GL_STENCIL_BUFFER_BIT); // NOTE: glClear(GL_STENCIL_BUFFER_BIT) counts as writing to the stencil buffer and will be directly ANDed with the stencil mask

  // draw models
  {
    // Drawing the model
    modelShader->use();
    setDynamicLightUniforms(modelShader);
    modelShader->setUniform("viewPos", camera.Position);
    modelShader->setUniform("view", viewMat);
    nanoSuitModel->Draw(*modelShader);

    //  Wall Hack Stencil For Model
    stencilShader->use();
    stencilShader->setUniform("color", glm::vec3(0.5f, 0.0f, 0.0f));
    stencilShader->setUniform("view", viewMat);
    stencilShader->setUniform("model", nanoSuitModelMatrix);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    nanoSuitModel->Draw(*stencilShader);
  }

  // bind default frame buffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT);

  frameBufferShader->use();
  glBindVertexArray(quadVertexAtt.arrayObject);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, framebuffer.colorAttachment);
  frameBufferShader->setUniform("tex", 2);
  frameBufferShader->setUniform("kernel", kernels5x5[selectedKernelIndex], ArrayCount(kernels5x5[selectedKernelIndex]));
  glDisable(GL_DEPTH_TEST);
  glDrawElements(GL_TRIANGLES, // drawing mode
                 6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
                 GL_UNSIGNED_INT, // type of the indices
                 (void*)0); // offset in the EBO
}

void KernelScene::inputStatesUpdated() {
  FirstPersonScene::inputStatesUpdated();

  if(hotPress(KeyboardInput_E))
  {
    nextImageKernel();
  }

  if(hotPress(KeyboardInput_Q))
  {
    prevImageKernel();
  }

  if(hotPress(MouseInput_Left))
  {
    toggleFlashlight();
  }

  if(isActive(WindowInput_SizeChange)) {
    Extent2D windowExtent = getWindowExtent();
    deleteFrameBuffer(framebuffer);
    framebuffer = initializeFrameBuffer(windowExtent.x, windowExtent.y);
    frameBufferShader->setUniform("textureWidth", (float32)windowExtent.x);
    frameBufferShader->setUniform("textureHeight", (float32)windowExtent.y);
  }
}

// TODO: reintroduce when controller input is added
//void KernelScene::key_LeftMouseButton_pressed(float32 xPos, float32 yPos)
//{
//  toggleFlashlight();
//}
//
//void KernelScene::button_X_pressed()
//{
//  toggleFlashlight();
//}
//
//void KernelScene::button_dPadUp_pressed()
//{
//  nextImageKernel();
//}
//
//void KernelScene::button_dPadDown_pressed()
//{
//  prevImageKernel();
//}

void KernelScene::toggleFlashlight()
{
  flashLightOn = !flashLightOn;
}

void KernelScene::nextImageKernel()
{
  double currentTime = glfwGetTime();
  if (currentTime - kernelModeSwitchTimer > 0.5f)
  {
    selectedKernelIndex = (selectedKernelIndex + 1) % kernelCount;
    kernelModeSwitchTimer = currentTime;
  }
}

void KernelScene::prevImageKernel()
{
  double currentTime = glfwGetTime();
  if (currentTime - kernelModeSwitchTimer > 0.5f)
  {
    selectedKernelIndex = selectedKernelIndex != 0 ? ((selectedKernelIndex - 1) % kernelCount) : (kernelCount - 1);
    kernelModeSwitchTimer = currentTime;
  }
}