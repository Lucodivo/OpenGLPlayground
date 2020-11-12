#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <glm/gtc/matrix_transform.hpp>

#include "../../Model.h"
#include "../../common/FileLocations.h"
#include "../../common/Util.h"

#include "ReflectRefractScene.h"

const float32 startDist = 2.5f;
const float32 sqr2over2 = 0.70710678118f;

const glm::vec3 cubePositions[] = {
        glm::vec3(startDist, -0.5f, 0.0f),
        glm::vec3(-startDist, -0.5f, 0.0f),
        glm::vec3(0.0f, -0.5f, startDist),
        glm::vec3(0.0f, -0.5f, -startDist),
        glm::vec3(startDist * sqr2over2, -0.5f, startDist * sqr2over2),
        glm::vec3(startDist * sqr2over2, -0.5f, -startDist * sqr2over2),
        glm::vec3(-startDist * sqr2over2, -0.5f, startDist * sqr2over2),
        glm::vec3(-startDist * sqr2over2, -0.5f, -startDist * sqr2over2)
};

const glm::vec3 modelPosition = glm::vec3(0.0f, -4.0f, 0.0f);
const float32 modelScale = 0.7f;

const float32 refractionIndexValues[] = {
        1.33f,  // Water
        1.309f,  // Ice
        1.52f,  // Glass
        2.42f  // Diamond
};

const uint32 reflactiveValCount = ArrayCount(refractionIndexValues) + 1;
uint32 selectedReflactionIndex = ArrayCount(refractionIndexValues);
const uint32 reflectionIndex = selectedReflactionIndex;

enum Mode
{
  None = 0,
  Exploding = 1,
  NormalVisualization = 2
};
Mode currMode = None;

ReflectRefractScene::ReflectRefractScene() : FirstPersonScene()
{
  camera.Position = glm::vec3(0.0f, 0.0f, 9.0f);
}

const char* ReflectRefractScene::title()
{
  return "Reflect & Refract";
}

void ReflectRefractScene::init(uint32 windowWidth, uint32 windowHeight)
{
  FirstPersonScene::init(windowWidth, windowHeight);
  
  explodingReflectionShader = new Shader(posNormalVertexShaderFileLoc, skyboxReflectionFragmentShaderFileLoc, explodeGeometryShaderFileLoc);
  exploding10InstanceReflectionShader = new Shader(posNormal10InstanceVertexShaderFileLoc, skyboxReflectionFragmentShaderFileLoc, explodeGeometryShaderFileLoc);
  reflectionShader = new Shader(posNormalVertexShaderFileLoc, skyboxReflectionFragmentShaderFileLoc);
  reflection10InstanceShader = new Shader(posNormal10InstanceVertexShaderFileLoc, skyboxReflectionFragmentShaderFileLoc);
  explodingRefractionShader = new Shader(posNormalVertexShaderFileLoc, skyboxRefractionFragmentShaderFileLoc, explodeGeometryShaderFileLoc);
  refractionShader = new Shader(posNormalVertexShaderFileLoc, skyboxRefractionFragmentShaderFileLoc);
  skyboxShader = new Shader(skyboxVertexShaderFileLoc, skyboxFragmentShaderFileLoc);
  normalVisualizationShader = new Shader(normalVisualizerVertexShaderFileLoc, SingleColorFragmentShaderFileLoc, triangleNormalVisualizerGeometryShaderFileLoc);
  normalVisualization10InstanceShader = new Shader(normalVisualizer10InstanceVertexShaderFileLoc, SingleColorFragmentShaderFileLoc, triangleNormalVisualizerGeometryShaderFileLoc);

  cubeVertexAtt = initializeCubePosNormVertexAttBuffers();
  skyboxVertexAtt = initializeCubePositionVertexAttBuffers();

  loadCubeMapTexture(skyboxInterstellarFaceLocations, skyboxTextureId);

  // load models
  nanoSuitModel = new Model(starmanModelLoc);
  //nanoSuitModel = new Model(superMario64LogoModelLoc);

  const glm::mat4 projectionMat = glm::perspective(glm::radians(camera.Zoom), (float32)windowWidth / (float32)windowHeight, 0.1f, 100.0f);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  // background clear color
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

#if 0
  // draw in wireframe
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureId);

  nanoSuitModelMat = glm::scale(glm::mat4(), glm::vec3(modelScale));  // it's a bit too big for our scene, so scale it down
  nanoSuitModelMat = glm::translate(nanoSuitModelMat, modelPosition); // translate it down so it's at the center of the scene

  explodingReflectionShader->use();
  explodingReflectionShader->setUniform("projection", projectionMat);
  explodingReflectionShader->setUniform("skybox", 0);

  exploding10InstanceReflectionShader->use();
  exploding10InstanceReflectionShader->setUniform("projection", projectionMat);
  exploding10InstanceReflectionShader->setUniform("skybox", 0);

  reflectionShader->use();
  reflectionShader->setUniform("projection", projectionMat);
  reflectionShader->setUniform("skybox", 0);

  reflection10InstanceShader->use();
  reflection10InstanceShader->setUniform("projection", projectionMat);
  reflection10InstanceShader->setUniform("skybox", 0);

  explodingRefractionShader->use();
  explodingRefractionShader->setUniform("projection", projectionMat);
  explodingRefractionShader->setUniform("skybox", 0);

  refractionShader->use();
  refractionShader->setUniform("projection", projectionMat);
  refractionShader->setUniform("skybox", 0);

  skyboxShader->use();
  skyboxShader->setUniform("projection", projectionMat);
  skyboxShader->setUniform("skybox", 0);

  normalVisualizationShader->use();
  normalVisualizationShader->setUniform("projection", projectionMat);
  normalVisualizationShader->setUniform("color", glm::vec3(1.0f, 1.0f, 0.0f));

  normalVisualization10InstanceShader->use();
  normalVisualization10InstanceShader->setUniform("projection", projectionMat);
  normalVisualization10InstanceShader->setUniform("color", glm::vec3(1.0f, 1.0f, 0.0f));

  initTime = (float32)glfwGetTime();
}

void ReflectRefractScene::deinit()
{
  FirstPersonScene::deinit();
  
  explodingReflectionShader->deleteShaderResources();
  exploding10InstanceReflectionShader->deleteShaderResources();
  reflectionShader->deleteShaderResources();
  reflection10InstanceShader->deleteShaderResources();
  explodingRefractionShader->deleteShaderResources();
  refractionShader->deleteShaderResources();
  skyboxShader->deleteShaderResources();
  normalVisualizationShader->deleteShaderResources();
  normalVisualization10InstanceShader->deleteShaderResources();
  delete explodingReflectionShader;
  delete exploding10InstanceReflectionShader;
  delete reflectionShader;
  delete reflection10InstanceShader;
  delete explodingRefractionShader;
  delete refractionShader;
  delete skyboxShader;
  delete normalVisualizationShader;
  delete normalVisualization10InstanceShader;

  VertexAtt vertexAttributes[] = {cubeVertexAtt, skyboxVertexAtt };
  deleteVertexAtts(ArrayCount(vertexAttributes), vertexAttributes);

  glDeleteTextures(1, &skyboxTextureId);

  delete nanoSuitModel;
}

void ReflectRefractScene::drawFrame()
{
  FirstPersonScene::drawFrame();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  float32 currTime = (float32)glfwGetTime() - initTime;
  deltaTime = currTime - lastFrame;
  lastFrame = currTime;

  glm::mat4 viewMat = camera.GetViewMatrix(deltaTime);

  // draw cube
  Shader* cubeShader = currMode == Exploding ? exploding10InstanceReflectionShader : reflection10InstanceShader;
  cubeShader->use();

  glBindVertexArray(cubeVertexAtt.arrayObject);

  cubeShader->setUniform("cameraPos", camera.Position);
  cubeShader->setUniform("view", viewMat);
  cubeShader->setUniform("time", currTime);

  for (int i = 0; i < ArrayCount(cubePositions); i++)
  {
    glm::mat4 model = glm::rotate(glm::mat4(), currTime * glm::radians(angularSpeed), orbitAxis); // orbit with time
    model = glm::translate(model, cubePositions[i]);
    model = glm::rotate(model, currTime * glm::radians(angularSpeed), rotationAxis); // rotate with time

    const std::string instanceModelName = "models[" + std::to_string(i) + "]";
    cubeShader->setUniform(instanceModelName, model);

    if (currMode == NormalVisualization) // draw cube normal visualizations
    {
      normalVisualization10InstanceShader->use();
      normalVisualization10InstanceShader->setUniform(instanceModelName, model);
      cubeShader->use();
    }
  }
  glDrawElementsInstanced(GL_TRIANGLES, // drawing mode
                          cubePosNormTexNumElements * 3, // number of elements to be rendered
                          GL_UNSIGNED_INT, // type of values in the indices
                          0, // offset in the EB
                          ArrayCount(cubePositions)); // instance count

  if (currMode == NormalVisualization)
  {
    normalVisualization10InstanceShader->use();
    normalVisualization10InstanceShader->setUniform("view", viewMat);
    glDrawElementsInstanced(GL_TRIANGLES, // drawing mode
                            cubePosNormTexNumElements * 3, // number of elements to be rendered
                            GL_UNSIGNED_INT, // type of values in the indices
                            0, // offset in the EB
                            ArrayCount(cubePositions)); // instance count
  }

  // draw model
  Shader* modelShader;
  if (currMode == Exploding)
  {
    if (selectedReflactionIndex == reflectionIndex)
    {
      modelShader = explodingReflectionShader;
    } else
    {
      modelShader = explodingRefractionShader;
    }
  } else
  {
    if (selectedReflactionIndex == reflectionIndex)
    {
      modelShader = reflectionShader;
    } else
    {
      modelShader = refractionShader;
    }
  }

  modelShader->use();
  modelShader->setUniform("cameraPos", camera.Position);
  modelShader->setUniform("view", viewMat);
  modelShader->setUniform("refractiveIndex", refractionIndexValues[selectedReflactionIndex]);
  modelShader->setUniform("model", nanoSuitModelMat);
  modelShader->setUniform("time", currTime);
  nanoSuitModel->Draw(*modelShader);

  if (currMode == NormalVisualization)
  {
    normalVisualizationShader->use();
    normalVisualizationShader->setUniform("view", viewMat);
    normalVisualizationShader->setUniform("model", nanoSuitModelMat);
    nanoSuitModel->Draw(*normalVisualizationShader);
  }

  // draw skybox
  skyboxShader->use();
  glm::mat4 viewMinusTranslation = glm::mat4(glm::mat3(viewMat));
  skyboxShader->setUniform("view", viewMinusTranslation);
  glBindVertexArray(skyboxVertexAtt.arrayObject);
  glDrawElements(GL_TRIANGLES, // drawing mode
                 36, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                 GL_UNSIGNED_INT, // type of the indices
                 0); // offset in the EBO
}


void ReflectRefractScene::inputStatesUpdated() {
  FirstPersonScene::inputStatesUpdated();

  if(hotPress(KeyboardInput_Up)) {
    nextModelReflaction();
  }

  if(hotPress(KeyboardInput_Down)) {
    prevModelReflaction();
  }

  if(hotPress(KeyboardInput_Left)) {
    prevMode();
  }

  if(hotPress(KeyboardInput_Right)) {
    nextMode();
  }
}

// TODO: reintroduce when controller input is complete
//void ReflectRefractScene::button_dPadUp_pressed()
//{
//  nextModelReflaction();
//}
//
//void ReflectRefractScene::button_dPadDown_pressed()
//{
//  prevModelReflaction();
//}
//
//void ReflectRefractScene::button_dPadLeft_pressed()
//{
//  prevMode();
//}
//
//void ReflectRefractScene::button_dPadRight_pressed()
//{
//  nextMode();
//}

void ReflectRefractScene::nextModelReflaction()
{
  double currentTime = glfwGetTime();
  if (currentTime - reflactionModeSwitchTimer > 0.5f)
  {
    selectedReflactionIndex = (selectedReflactionIndex + 1) % reflactiveValCount;
    reflactionModeSwitchTimer = currentTime;
  }
}

void ReflectRefractScene::prevModelReflaction()
{
  double currentTime = glfwGetTime();
  if (currentTime - reflactionModeSwitchTimer > 0.5f)
  {
    selectedReflactionIndex = selectedReflactionIndex != 0 ? ((selectedReflactionIndex - 1) % reflactiveValCount) : (reflactiveValCount - 1);
    reflactionModeSwitchTimer = currentTime;
  }
}

void ReflectRefractScene::nextMode()
{
  double currentTime = glfwGetTime();
  if (currentTime - modeSwitchTimer > 0.5f)
  {
    if (currMode == None) currMode = Exploding;
    else if (currMode == Exploding) currMode = NormalVisualization;
    else if (currMode == NormalVisualization) currMode = None;
    modeSwitchTimer = currentTime;
  }
}

void ReflectRefractScene::prevMode()
{
  double currentTime = glfwGetTime();
  if (currentTime - modeSwitchTimer > 0.5f)
  {
    if (currMode == None) currMode = NormalVisualization;
    else if (currMode == Exploding) currMode = None;
    else if (currMode == NormalVisualization) currMode = Exploding;
    modeSwitchTimer = currentTime;
  }
}