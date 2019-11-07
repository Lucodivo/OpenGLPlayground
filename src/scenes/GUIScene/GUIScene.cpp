//
// Created by Connor on 10/24/2019.
//

#include "GUIScene.h"
#include "../../common/FileLocations.h"
#include "../../common/ObjectData.h"
#include "../../common/Util.h"

const glm::vec3 startingBoundingBoxMin = glm::vec3(-1.0f, -1.0f, -1.0f);
const glm::vec3 startingBoundingBoxMax = glm::vec3(1.0f, 1.0f, 1.0f);

GUIScene::GUIScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
      : FirstPersonScene(window, initScreenHeight, initScreenWidth),
        cubeShader(posVertexShaderFileLoc, singleColorFragmentShaderFileLoc),
        cursorMode(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {}

void GUIScene::runScene()
{
  uint32 cubeVAO, cubeVBO, cubeEBO;
  initializeCubePositionVertexAttBuffers(cubeVAO, cubeVBO, cubeEBO);

  renderLoop(cubeVAO);

  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteBuffers(1, &cubeVBO);
  glDeleteBuffers(1, &cubeEBO);
}

void GUIScene::renderLoop(uint32 cubeVAO)
{
  projectionMat = glm::perspective(glm::radians(camera.Zoom), (float32)viewportWidth / (float32)viewportHeight, 0.1f, 100.0f);
  const glm::vec3 cubeColor = glm::vec3(0.8f, 0.0f, 0.0f);
  const glm::vec3 wireFrameColor = glm::vec3(0.0f, 0.0f, 0.0f);

  cubes[0].wireframe = false;
  cubes[0].worldPos = glm::vec3(3.0f, 0.0f, 0.0f);
  cubes[0].boundingBoxMin = startingBoundingBoxMin + cubes[0].worldPos;
  cubes[0].boundingBoxMax = startingBoundingBoxMax + cubes[0].worldPos;
  cubes[1].wireframe = true;
  cubes[1].worldPos = glm::vec3(-3.0f, 0.0f, 0.0f);
  cubes[1].boundingBoxMin = startingBoundingBoxMin + cubes[1].worldPos;
  cubes[1].boundingBoxMax = startingBoundingBoxMax + cubes[1].worldPos;
  cubes[2].wireframe = false;
  cubes[2].worldPos = glm::vec3(0.0f, 0.0f, -3.0f);
  cubes[2].boundingBoxMin = startingBoundingBoxMin + cubes[2].worldPos;
  cubes[2].boundingBoxMax = startingBoundingBoxMax + cubes[2].worldPos;


  // set constant uniforms
  cubeShader.use();
  cubeShader.setUniform("projection", projectionMat);
  cubeShader.setUniform("color", cubeColor);

  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  camera.Position += glm::vec3(0.0f, 0.0f, 4.0f);

  // NOTE: render/game loop
  float32 startTime = (float32)glfwGetTime();
  while (glfwWindowShouldClose(window) == GL_FALSE)
  {
    float32 t = (float32) glfwGetTime() - startTime;
    deltaTime = t - lastFrame;
    lastFrame = t;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // check for input
    processKeyboardInput(window, this);

    if(displayMouseClick)
    {
      renderText(text, 25.0f, 25.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    }

    viewMat = camera.GetViewMatrix(deltaTime);

    cubeShader.use();
    cubeShader.setUniform("view", viewMat);

    for(uint8 i = 0; i < numCubes; i++) {
      glm::mat4 modelMat = glm::translate(glm::mat4(), cubes[i].worldPos);
      cubeShader.setUniform("model", modelMat);

      glBindVertexArray(cubeVAO);
      glDrawElements(GL_TRIANGLES, // drawing mode
                     36, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                     GL_UNSIGNED_INT, // type of the indices
                     0); // offset in the EBO

      if(cubes[i].wireframe) {
        glDisable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        cubeShader.setUniform("color", wireFrameColor);
        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, // drawing mode
                       36, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                       GL_UNSIGNED_INT, // type of the indices
                       0); // offset in the EBO
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        cubeShader.setUniform("color", cubeColor);
        glEnable(GL_DEPTH_TEST);
      }
    }

    glfwSwapBuffers(window); // swaps double buffers
    glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
  }
}

void GUIScene::key_Tab_pressed()
{
  cursorMode = !cursorMode;
  glfwSetInputMode(window, GLFW_CURSOR, cursorMode ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

void GUIScene::key_LeftMouseButton_pressed(float32 xPos, float32 yPos)
{
  displayMouseClick = true;
  text = "Left click at: X = " + std::to_string(xPos) + ", Y = " + std::to_string(yPos);
  checkMouseClickCollision(xPos, yPos);
}

void GUIScene::key_RightMouseButton_pressed(float32 xPos, float32 yPos)
{
  displayMouseClick = false;
}

void GUIScene::mouseMovement(float32 xOffset, float32 yOffset)
{
  if(!cursorMode) {
    FirstPersonScene::mouseMovement(xOffset, yOffset);
  } else {
    // TODO
  }
}

void GUIScene::checkMouseClickCollision(float32 mouseX, float32 mouseY)
{
  // to 3D Normal Device Coordinates
  float32 ndcX = ((2.0f * mouseX) / viewportWidth) - 1.0f;
  float32 ndcY = 1.0f - ((2.0f * mouseY) / viewportHeight);
  const float32 ndcZ = 1.0f;
  glm::vec3 rayNormalDeviceCoords = glm::vec3(ndcX, ndcY, ndcZ);

  // to 4D Homogeneous Clip Coordinates
  glm::vec4 rayClipCoord = glm::vec4(rayNormalDeviceCoords.x, rayNormalDeviceCoords.y, -1.0f, 1.0f);

  // to 4D Camera Coordinates
  glm::vec4 rayCameraCoord = glm::inverse(projectionMat) * rayClipCoord;
  // z should still point forward, and w should be 0.0f (representing a vector, NOT a point)
  rayCameraCoord.z = -1.0f;
  rayCameraCoord.w = 0.0f;

  // to 4D World Coordinates
  glm::vec3 rayWorldCoord = glm::vec3(glm::inverse(viewMat) * rayCameraCoord);
  rayWorldCoord = glm::normalize(rayWorldCoord);

  glm::vec3 rayOrigin = camera.Position;
  for(int i = 0; i < numCubes; i++) {
    if(checkCubeCollision(&rayWorldCoord, &rayOrigin, &cubes[i])) {
      cubes[i].wireframe = true;
    } else {
      cubes[i].wireframe = false;
    }
  }
}

bool GUIScene::checkCubeCollision(glm::vec3* worldRay, glm::vec3* rayOrigin, Cube* cube)
{
  float32 tmin = (cube->boundingBoxMin.x - rayOrigin->x) / worldRay->x;
  float32 tmax = (cube->boundingBoxMax.x - rayOrigin->x) / worldRay->x;

  if (tmin > tmax) swap(&tmin, &tmax);

  float32 tymin = (cube->boundingBoxMin.y - rayOrigin->y) / worldRay->y;
  float32 tymax = (cube->boundingBoxMax.y - rayOrigin->y) / worldRay->y;

  if (tymin > tymax) swap(&tymin, &tymax);

  if ((tmin > tymax) || (tymin > tmax))
    return false;

  if (tymin > tmin)
    tmin = tymin;

  if (tymax < tmax)
    tmax = tymax;

  float32 tzmin = (cube->boundingBoxMin.z - rayOrigin->z) / worldRay->z;
  float32 tzmax = (cube->boundingBoxMax.z - rayOrigin->z) / worldRay->z;

  if (tzmin > tzmax) swap(&tzmin, &tzmax);

  if ((tmin > tzmax) || (tzmin > tmax))
    return false;

  if (tzmin > tmin)
    tmin = tzmin;

  if (tzmax < tmax)
    tmax = tzmax;

  return true;
}
