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

const glm::vec3 modelPosition = glm::vec3(0.0f, -2.0f, 0.0f);
const float32 modelScale = 0.3f;

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

ReflectRefractScene::ReflectRefractScene(GLFWwindow* window)
        : FirstPersonScene(window),
          explodingReflectionShader(posNormalVertexShaderFileLoc, skyboxReflectionFragmentShaderFileLoc, explodeGeometryShaderFileLoc),
          exploding10InstanceReflectionShader(posNormal10InstanceVertexShaderFileLoc, skyboxReflectionFragmentShaderFileLoc, explodeGeometryShaderFileLoc),
          reflectionShader(posNormalVertexShaderFileLoc, skyboxReflectionFragmentShaderFileLoc),
          reflection10InstanceShader(posNormal10InstanceVertexShaderFileLoc, skyboxReflectionFragmentShaderFileLoc),
          explodingRefractionShader(posNormalVertexShaderFileLoc, skyboxRefractionFragmentShaderFileLoc, explodeGeometryShaderFileLoc),
          refractionShader(posNormalVertexShaderFileLoc, skyboxRefractionFragmentShaderFileLoc),
          skyboxShader(skyboxVertexShaderFileLoc, skyboxFragmentShaderFileLoc),
          normalVisualizationShader(normalVisualizerVertexShaderFileLoc, singleColorFragmentShaderFileLoc, triangleNormalVisualizerGeometryShaderFileLoc),
          normalVisualization10InstanceShader(normalVisualizer10InstanceVertexShaderFileLoc, singleColorFragmentShaderFileLoc, triangleNormalVisualizerGeometryShaderFileLoc) {}

void ReflectRefractScene::init(uint32 windowWidth, uint32 windowHeight)
{
  FirstPersonScene::init(windowWidth, windowHeight);
}

void ReflectRefractScene::deinit()
{
  FirstPersonScene::deinit();
}

void ReflectRefractScene::drawFrame()
{
  FirstPersonScene::drawFrame();
}

void ReflectRefractScene::runScene()
{
  VertexAtt cubeVertexAtt = initializeCubePosNormVertexAttBuffers();
  VertexAtt skyboxVertexAtt = initializeCubePositionVertexAttBuffers();

  renderLoop(cubeVertexAtt.arrayObject, skyboxVertexAtt.arrayObject);

  deleteVertexAtt(cubeVertexAtt);
  deleteVertexAtt(skyboxVertexAtt);
}

void ReflectRefractScene::renderLoop(uint32& cubeVAO, uint32& skyboxVAO)
{

  uint32 skyboxTextureId;
  loadCubeMapTexture(skyboxInterstellarFaceLocations, skyboxTextureId);

  // load models
  Model nanoSuitModel((char*)nanoSuitModelLoc);
  //Model nanoSuitModel((char*)superMario64LogoModelLoc);

  const glm::mat4 projectionMat = glm::perspective(glm::radians(camera.Zoom), (float32)windowWidth / (float32)windowHeight, 0.1f, 100.0f);

  const float32 cubRotAngle = 7.3f;

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  // background clear color
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#if 0
  // draw in wireframe
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureId);

  explodingReflectionShader.use();
  explodingReflectionShader.setUniform("projection", projectionMat);
  explodingReflectionShader.setUniform("skybox", 0);

  exploding10InstanceReflectionShader.use();
  exploding10InstanceReflectionShader.setUniform("projection", projectionMat);
  exploding10InstanceReflectionShader.setUniform("skybox", 0);

  reflectionShader.use();
  reflectionShader.setUniform("projection", projectionMat);
  reflectionShader.setUniform("skybox", 0);

  reflection10InstanceShader.use();
  reflection10InstanceShader.setUniform("projection", projectionMat);
  reflection10InstanceShader.setUniform("skybox", 0);

  explodingRefractionShader.use();
  explodingRefractionShader.setUniform("projection", projectionMat);
  explodingRefractionShader.setUniform("skybox", 0);

  refractionShader.use();
  refractionShader.setUniform("projection", projectionMat);
  refractionShader.setUniform("skybox", 0);

  skyboxShader.use();
  skyboxShader.setUniform("projection", projectionMat);
  skyboxShader.setUniform("skybox", 0);

  normalVisualizationShader.use();
  normalVisualizationShader.setUniform("projection", projectionMat);
  normalVisualizationShader.setUniform("color", glm::vec3(1.0f, 1.0f, 0.0f));

  normalVisualization10InstanceShader.use();
  normalVisualization10InstanceShader.setUniform("projection", projectionMat);
  normalVisualization10InstanceShader.setUniform("color", glm::vec3(1.0f, 1.0f, 0.0f));

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  camera.Position += glm::vec3(0.0f, 0.0f, 7.0f);

  // NOTE: render/game loop
  float32 initTime = (float32)glfwGetTime();
  while (glfwWindowShouldClose(window) == GL_FALSE)
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float32 currTime = (float32)glfwGetTime() - initTime;
    deltaTime = currTime - lastFrame;
    lastFrame = currTime;

    glm::mat4 viewMat = camera.GetViewMatrix(deltaTime);

    // draw cube
    Shader* cubeShader = currMode == Exploding ? &exploding10InstanceReflectionShader : &reflection10InstanceShader;
    cubeShader->use();

    glBindVertexArray(cubeVAO);

    cubeShader->setUniform("cameraPos", camera.Position);
    cubeShader->setUniform("view", viewMat);
    cubeShader->setUniform("time", currTime);

    float32 angularSpeed = 7.3f;
    glm::vec3 orbitAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 rotationAxis = glm::vec3(1.0f, 0.3f, 0.5f);

    for (int i = 0; i < ArrayCount(cubePositions); i++)
    {
      glm::mat4 model = glm::rotate(glm::mat4(), currTime * glm::radians(angularSpeed), orbitAxis); // orbit with time
      model = glm::translate(model, cubePositions[i]);
      model = glm::rotate(model, currTime * glm::radians(angularSpeed), rotationAxis); // rotate with time

      const std::string instanceModelName = "models[" + std::to_string(i) + "]";
      cubeShader->setUniform(instanceModelName, model);

      if (currMode == NormalVisualization) // draw cube normal visualizations
      {
        normalVisualization10InstanceShader.use();
        normalVisualization10InstanceShader.setUniform(instanceModelName, model);
        cubeShader->use();
      }
    }

    glDrawElementsInstanced(GL_TRIANGLES, // drawing mode
                            cubePosNormTexNumElements * 3, // number of elements to be rendered
                            GL_UNSIGNED_INT, // type of values in the indices
                            0, // offset in the EB
                            8); // instance count

    if (currMode == NormalVisualization)
    {
      normalVisualization10InstanceShader.use();
      normalVisualization10InstanceShader.setUniform("view", viewMat);

      glDrawElementsInstanced(GL_TRIANGLES, // drawing mode
                              cubePosNormTexNumElements * 3, // number of elements to be rendered
                              GL_UNSIGNED_INT, // type of values in the indices
                              0, // offset in the EB
                              8); // instance count
    }
    glBindVertexArray(0);

    // draw model
    Shader* modelShader;
    if (currMode == Exploding)
    {
      if (selectedReflactionIndex == reflectionIndex)
      {
        modelShader = &explodingReflectionShader;
      } else
      {
        modelShader = &explodingRefractionShader;
      }
    } else
    {
      if (selectedReflactionIndex == reflectionIndex)
      {
        modelShader = &reflectionShader;
      } else
      {
        modelShader = &refractionShader;
      }
    }

    glm::mat4 model;
    model = glm::scale(model, glm::vec3(modelScale));  // it's a bit too big for our scene, so scale it down
    model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
//    model = glm::rotate(model, glm::radians(5.0f * sin(currTime)), glm::vec3(1.0f, 0.0f, 0.0f));
//    model = glm::rotate(model, glm::radians(5.0f * cos(currTime)), glm::vec3(0.0f, 1.0f, 0.0f));
//    model = glm::rotate(model, glm::radians(5.0f * -sin(currTime)), glm::vec3(0.0f, 0.0f, 1.0f));

    modelShader->use();
    modelShader->setUniform("cameraPos", camera.Position);
    modelShader->setUniform("view", viewMat);
    modelShader->setUniform("refractiveIndex", refractionIndexValues[selectedReflactionIndex]);
    modelShader->setUniform("model", model);
    modelShader->setUniform("time", currTime);
    nanoSuitModel.Draw(*modelShader);

    if (currMode == NormalVisualization)
    {
      normalVisualizationShader.use();
      normalVisualizationShader.setUniform("view", viewMat);
      normalVisualizationShader.setUniform("model", model);

      nanoSuitModel.Draw(normalVisualizationShader);
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

    uint32 numFrames = (uint32)(1 / deltaTime);
    renderText(std::to_string(numFrames) + " FPS", 25.0f, 25.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    glfwSwapBuffers(window); // swaps double buffers (call after all render commands are completed)
    glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
  }
}

void ReflectRefractScene::key_Up()
{
  nextModelReflaction();
}

void ReflectRefractScene::key_Down()
{
  prevModelReflaction();
}

void ReflectRefractScene::key_Left()
{
  prevMode();
}

void ReflectRefractScene::key_Right()
{
  nextMode();
}

void ReflectRefractScene::button_dPadUp_pressed()
{
  nextModelReflaction();
}

void ReflectRefractScene::button_dPadDown_pressed()
{
  prevModelReflaction();
}

void ReflectRefractScene::button_dPadLeft_pressed()
{
  prevMode();
}

void ReflectRefractScene::button_dPadRight_pressed()
{
  nextMode();
}

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