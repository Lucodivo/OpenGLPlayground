//
// Created by Connor on 10/24/2019.
//

#include "GUIScene.h"
#include "../../common/FileLocations.h"
#include "../../common/ObjectData.h"
#include "../../common/Util.h"

const glm::vec3 startingBoundingBoxMin = glm::vec3(-1.0f, -1.0f, -1.0f);
const glm::vec3 startingBoundingBoxMax = glm::vec3(1.0f, 1.0f, 1.0f);

GUIScene::GUIScene(GLFWwindow* window)
      : FirstPersonScene(window) {
  camera.Position += glm::vec3(0.0f, 0.0f, 4.0f);
}

void GUIScene::init(uint32 windowWidth, uint32 windowHeight)
{
  FirstPersonScene::init(windowWidth, windowHeight);

  // TODO: rethink this cursor logic?
  originalCursorMode = glfwGetInputMode(window, GLFW_CURSOR);
  glfwSetInputMode(window, GLFW_CURSOR, cursorMode ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
  
  cubeShader = new Shader(posVertexShaderFileLoc, singleColorFragmentShaderFileLoc);

  cubeVertexAtt = initializeCubePositionVertexAttBuffers();
  
  projectionMat = glm::perspective(glm::radians(camera.Zoom), (float32)windowWidth / (float32)windowHeight, 0.1f, 100.0f);

  cubes[0].worldPos = glm::vec3(3.0f, 0.0f, 0.0f);
  cubes[0].boundingBoxMin = startingBoundingBoxMin + cubes[0].worldPos;
  cubes[0].boundingBoxMax = startingBoundingBoxMax + cubes[0].worldPos;
  cubes[1].worldPos = glm::vec3(-3.0f, 0.0f, 0.0f);
  cubes[1].boundingBoxMin = startingBoundingBoxMin + cubes[1].worldPos;
  cubes[1].boundingBoxMax = startingBoundingBoxMax + cubes[1].worldPos;
  cubes[2].worldPos = glm::vec3(0.0f, 0.0f, -3.0f);
  cubes[2].boundingBoxMin = startingBoundingBoxMin + cubes[2].worldPos;
  cubes[2].boundingBoxMax = startingBoundingBoxMax + cubes[2].worldPos;

  cubeModelMats[0] = glm::translate(glm::mat4(), cubes[0].worldPos);
  cubeModelMats[1] = glm::translate(glm::mat4(), cubes[1].worldPos);
  cubeModelMats[2] = glm::translate(glm::mat4(), cubes[2].worldPos);

  // set constant uniforms
  cubeShader->use();
  cubeShader->setUniform("projection", projectionMat);

  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glLineWidth(3.0f);
  
  glBindVertexArray(cubeVertexAtt.arrayObject);

  startTime = (float32)glfwGetTime();
}

void GUIScene::deinit()
{
  FirstPersonScene::deinit();
  
  cubeShader->deleteShaderResources();
  delete cubeShader;
  
  deleteVertexAtt(cubeVertexAtt);

  glfwSetInputMode(window, GLFW_CURSOR, originalCursorMode);
}

void GUIScene::drawFrame()
{
  FirstPersonScene::drawFrame();
  
  float32 t = (float32) glfwGetTime() - startTime;
  deltaTime = t - lastFrame;
  lastFrame = t;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  viewMat = camera.GetViewMatrix(deltaTime);

  // draw cubes
  cubeShader->use();
  cubeShader->setUniform("view", viewMat);
  cubeShader->setUniform("color", cubeColor);
  for(uint8 i = 0; i < numCubes; i++)
  {
    cubeShader->setUniform("model", cubeModelMats[i]);

    glDrawElements(GL_TRIANGLES, // drawing mode
                   36, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                   GL_UNSIGNED_INT, // type of the indices
                   0); // offset in the EBO
  }

  // draw wireframes on top
  cubeShader->setUniform("color", wireFrameColor);
  glDisable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  for(uint32 i = 0; i < numCubes; i++)
  {
    if(cubes[i].wireframe) {
      cubeShader->setUniform("model", cubeModelMats[i]);
      glBindVertexArray(cubeVertexAtt.arrayObject);
      glDrawElements(GL_TRIANGLES, // drawing mode
                     36, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                     GL_UNSIGNED_INT, // type of the indices
                     0); // offset in the EBO
    }
  }
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);
}

void GUIScene::key_E_pressed()
{
  cursorMode = !cursorMode;
  // TODO: Cannot turn off input mode when I am trying to select a scene, think of alternative
  glfwSetInputMode(window, GLFW_CURSOR, cursorMode ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

void GUIScene::key_LeftMouseButton_pressed(float32 xPos, float32 yPos)
{
  checkMouseClickCollision(xPos, yPos);
}

void GUIScene::mouseMovement(float32 xOffset, float32 yOffset)
{
  if(!cursorMode) {
    FirstPersonScene::mouseMovement(xOffset, yOffset);
  }
}

void GUIScene::checkMouseClickCollision(float32 mouseX, float32 mouseY)
{
  // to 3D Normal Device Coordinates
  float32 ndcX = ((2.0f * mouseX) / windowWidth) - 1.0f;
  float32 ndcY = 1.0f - ((2.0f * mouseY) / windowHeight);
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
  // At what time, t, will the world ray intersect the cube's planes parallel to the x-axis
  float32 txmin = (cube->boundingBoxMin.x - rayOrigin->x) / worldRay->x;
  float32 txmax = (cube->boundingBoxMax.x - rayOrigin->x) / worldRay->x;
  if (txmin > txmax) swap(&txmin, &txmax);

  // At what time, t, will the world ray intersect the cube's planes parallel to the y-axis
  float32 tymin = (cube->boundingBoxMin.y - rayOrigin->y) / worldRay->y;
  float32 tymax = (cube->boundingBoxMax.y - rayOrigin->y) / worldRay->y;
  if (tymin > tymax) swap(&tymin, &tymax);

  // If the ray intersects both x-axis/y-axis aligned planes before intersecting one y-axis/x-axis aligned planes
  // the ray cannot be within the four planes simultaneously and the ray misses the cube
  if ((txmin > tymax) || (tymin > txmax)) return false;

  // What is the time interval the world ray could exist between the 2 x-axis aligned and 2 y-axis aligned planes
  float32 txymin = (txmin > tymin) ? txmin : tymin;
  float32 txymax = (txmax < tymax) ? txmax : tymax;

  // At what time, t, will the world ray intersect the cube's planes parallel to the z-axis
  float32 tzmin = (cube->boundingBoxMin.z - rayOrigin->z) / worldRay->z;
  float32 tzmax = (cube->boundingBoxMax.z - rayOrigin->z) / worldRay->z;
  if (tzmin > tzmax) swap(&tzmin, &tzmax);

  // If the ray passes through the interval in which it could exist between the y-axis and x-axis aligned planes
  // before intersecting either z-axis aligned planes, the ray cannot be within the six planes simultaneously
  // and the ray misses the cube
  if ((txymin > tzmax) || (tzmin > txymax)) return false;

  // What is the time interval the world ray intersects all six axis aligned planes?
  float32 tmin = (txymin > tzmin) ? txymin : tzmin;
  float32 tmax = (txymax < tzmax) ? txymax : tzmin;

  // If the time of the collisions is negative, an intersection happened "behind" the origin and we don't consider the intersection
  if(tmin < 0 && tmax < 0) return false;

  return true;
}
