//
// Created by Connor on 11/21/2019.
//

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include "MengerSpongeScene.h"
#include "../../common/FileLocations.h"
#include "../../common/ObjectData.h"
#include "../../common/Util.h"

MengerSpongeScene::MengerSpongeScene(GLFWwindow* window): FirstPersonScene(), window(window)
{
  camera.Position = glm::vec3(0.0f, 1.0f, 30.0f);
}

const char* MengerSpongeScene::title()
{
  return "Menger Prison SDF + Raster Cube";
}

void MengerSpongeScene::init(uint32 windowWidth, uint32 windowHeight)
{
  FirstPersonScene::init(windowWidth, windowHeight);

  enableCursor(window, showDebugWindows);

  mengerSpongeShader = new Shader(UVCoordVertexShaderFileLoc, MengerSpongeFragmentShaderFileLoc);
  pixel2DShader = new Shader(pixel2DVertexShaderFileLoc, textureFragmentShaderFileLoc);
  cubeShader = new Shader(CubePosNormTexVertexShaderFileLoc, CubeTextureFragmentShaderFileLoc);

  quadVertexAtt = initializeFrameBufferQuadVertexAttBuffers();
  cubeVertexAtt = initializeCubePosNormTexVertexAttBuffers();

  resolution largestResolution = screenResolutions[ArrayCount(screenResolutions) - 1];
  dynamicResolutionFBO = initializeFrameBuffer(largestResolution.width, largestResolution.height);

  // NOTE: This is helps maintain same projection for both the ray marching and rasterization
  // If how we shoot rays change, this must change. If this changes, how we shoot rays must change.
  const float rayMarchFovVertical = glm::radians(53.14f);
  projectionMat = glm::perspective(rayMarchFovVertical, (float32)currentResolution.width / (float32)currentResolution.height, 0.1f, 200.0f);

  load2DTexture(scarabWingsTextureLoc, textureDiff1Id, true, true, &textureWidth, &textureHeight);
  load2DTexture(scarabWingsSpecTextureLoc, textureSpec1Id, true, false);
  load2DTexture(scarabTextureLoc, textureDiff2Id, true, true);
  load2DTexture(scarabSpecTextureLoc, textureSpec2Id, true, false);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureDiff1Id);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textureSpec1Id);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, textureDiff2Id);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, textureSpec2Id);

  mengerSpongeShader->use();
  mengerSpongeShader->setUniform("viewPortResolution", glm::vec2(currentResolution.width, currentResolution.height));
  mengerSpongeShader->setUniform("directionalLight.color.ambient", directionalLightAmb);
  mengerSpongeShader->setUniform("directionalLight.color.diffuse", directionalLightDiff);
  mengerSpongeShader->setUniform("directionalLight.color.specular", directionalLightSpec);
  mengerSpongeShader->setUniform("directionalLight.direction", directionalLightDir);
  mengerSpongeShader->setUniform("projection", projectionMat);

  cubeShader->use();
  cubeShader->setUniform("projection", projectionMat);
  cubeShader->setUniform("material.diffTexture", 0);
  cubeShader->setUniform("material.specTexture", 1);
  cubeShader->setUniform("material.shininess", 16.0f);
  cubeShader->setUniform("directionalLight.color.ambient", directionalLightAmb);
  cubeShader->setUniform("directionalLight.color.diffuse", directionalLightDiff);
  cubeShader->setUniform("directionalLight.color.specular", directionalLightSpec);
  cubeShader->setUniform("directionalLight.direction", directionalLightDir);

//  pixel2DShader->use();
//  pixel2DShader->setUniform("windowDimens", glm::vec2(currentResolution.width, currentResolution.height));
//  pixel2DShader->setUniform("lowerLeftOffset", glm::vec2((currentResolution.width / 2) - 16.0, (currentResolution.height / 2) - 16.0));
//  pixel2DShader->setUniform("spriteDimens", glm::vec2(textureWidth, textureHeight));

  glBindVertexArray(quadVertexAtt.arrayObject);

  lastFrame = (float32)glfwGetTime();
  startTime = lastFrame;

  // background clear color
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // NOTE: the viewport dictates the area of the bound
  glViewport(0, 0, currentResolution.width, currentResolution.height);
}

void MengerSpongeScene::deinit()
{
  FirstPersonScene::deinit();

  mengerSpongeShader->deleteShaderResources();
  pixel2DShader->deleteShaderResources();
  cubeShader->deleteShaderResources();
  delete mengerSpongeShader;
  delete pixel2DShader;
  delete cubeShader;

  VertexAtt deleteVertexAttributes[] = { quadVertexAtt, cubeVertexAtt };
  deleteVertexAtts(ArrayCount(deleteVertexAttributes), deleteVertexAttributes);

  deleteFrameBuffer(dynamicResolutionFBO);

  uint32 deleteTextures[] = { textureDiff1Id, textureSpec1Id, textureDiff2Id, textureSpec2Id };
  glDeleteTextures(ArrayCount(deleteTextures), deleteTextures);
}

void MengerSpongeScene::drawFrame()
{
  FirstPersonScene::drawFrame();

  float32 t = (float32)glfwGetTime() - startTime;
  deltaTime = t - lastFrame;
  lastFrame = t;

  if(((uint32)(t / frameTime) % 2) == 0) {
    cubeShader->setUniform("material.diffTexture", 0);
    cubeShader->setUniform("material.specTexture", 1);
  } else {
    cubeShader->setUniform("material.diffTexture", 2);
    cubeShader->setUniform("material.specTexture", 3);
  }

  // Auto run forward
//    glm::vec3 deltaCameraPos = camera.Front;
//    deltaCameraPos *= 0.07;
//    camera.Position += deltaCameraPos;
  glm::mat4 cameraMat = camera.UpdateViewMatrix(deltaTime, cameraMovementSpeed * 4.0f, false);

  // bind our frame buffer
  glBindFramebuffer(GL_FRAMEBUFFER, dynamicResolutionFBO.id);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  bool checkShaderFiles = t - lastShaderFileUpdateCheck > shaderFileCheckIntervalInSeconds;
  if(checkShaderFiles && mengerSpongeShader->updateShadersWhenOutdated(FragmentShaderFlag)) {
    mengerSpongeShader->use();
    mengerSpongeShader->setUniform("viewPortResolution", glm::vec2(currentResolution.width, currentResolution.height));
    mengerSpongeShader->setUniform("directionalLight.color.ambient", directionalLightAmb);
    mengerSpongeShader->setUniform("directionalLight.color.diffuse", directionalLightDiff);
    mengerSpongeShader->setUniform("directionalLight.color.specular", directionalLightSpec);
    mengerSpongeShader->setUniform("directionalLight.direction", directionalLightDir);
    mengerSpongeShader->setUniform("projection", projectionMat);
    lastShaderFileUpdateCheck = t;
  }

  mengerSpongeShader->use();
  mengerSpongeShader->setUniform("rayOrigin", camera.Position);
  mengerSpongeShader->setUniform("elapsedTime", t);
  mengerSpongeShader->setUniform("view", cameraMat);
  mengerSpongeShader->setUniform("cameraPos", camera.Position);
  mengerSpongeShader->setUniform("numSamples", numSamples);
  glBindVertexArray(quadVertexAtt.arrayObject);
  glDrawElements(GL_TRIANGLES, // drawing mode
                 6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
                 GL_UNSIGNED_INT, // type of the indices
                 0 /* offset in the EBO */);

  glm::mat4 cubeModel = glm::scale(glm::mat4(), glm::vec3(cubeScale)); // scale is uniform
  cubeModel = glm::translate(cubeModel, cubePos);
  cubeModel = glm::rotate(cubeModel, t * glm::radians(20.0f), cubeRotAxis);

  glBindVertexArray(cubeVertexAtt.arrayObject);

  // TODO: supply the camera position in cube model space to avoid requiring computation of inverse matrix
  uint32 cubeAttrIndices[ArrayCount(cubeAttributeIndices)];
  cubeAttributeIndicesBackToFront(camera.Position, cubeModel, cubeAttrIndices);

  // bind element buffer object to give indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeVertexAtt.indexObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeAttrIndices), cubeAttrIndices, GL_DYNAMIC_DRAW);

  cubeShader->use();
  cubeShader->setUniform("model", cubeModel);
  cubeShader->setUniform("view", cameraMat);
  cubeShader->setUniform("cameraPos", camera.Position);

  glDrawElements(GL_TRIANGLES, // drawing mode
                 cubePosNormTexNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                 GL_UNSIGNED_INT, // type of the indices
                 0); // offset in the EBO
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//    pixel2DShader->use();
//    glDrawElements(GL_TRIANGLES, // drawing mode
//                   6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
//                   GL_UNSIGNED_INT, // type of the indices
//                   0); // offset in the EBO

  // bind our frame buffer
  //NOTE: dynamicResolutionFBO is already bound to GL_READ_FRAMEBUFFER
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glBlitFramebuffer(0, 0, currentResolution.width, currentResolution.height, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

void MengerSpongeScene::drawGui()
{
  FirstPersonScene::drawGui();

  if (showDebugWindows){
    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.50f);
    // Simplified Settings (expose floating-pointer border sizes as boolean representing 0.0f or 1.0f)
    if (ImGui::SliderInt("Supersamples", &numSamples, 1, 8)) {}
    ImGui::PopItemWidth();
  }
}

void MengerSpongeScene::inputStatesUpdated() {
  FirstPersonScene::inputStatesUpdated();

  if(hotPress(KeyboardInput_E))
  {
    if(++currentResolutionIndex >= ArrayCount(screenResolutions)) {
      currentResolutionIndex = 0;
    }

    currentResolution = screenResolutions[currentResolutionIndex];

    glViewport(0, 0, currentResolution.width, currentResolution.height);

    mengerSpongeShader->use();
    mengerSpongeShader->setUniform("viewPortResolution", glm::vec2(currentResolution.width, currentResolution.height));

    pixel2DShader->use();
    pixel2DShader->setUniform("windowDimens", glm::vec2(currentResolution.width, currentResolution.height));
    pixel2DShader->setUniform("lowerLeftOffset", glm::vec2((currentResolution.width / 2) - 16.0, (currentResolution.height / 2) - 16.0));
  }

  if(hotPress(KeyboardInput_Q))
  {
    if(--currentResolutionIndex == -1) {
      currentResolutionIndex = ArrayCount(screenResolutions) - 1;
    }

    currentResolution = screenResolutions[currentResolutionIndex];

    glViewport(0, 0, currentResolution.width, currentResolution.height);

    mengerSpongeShader->use();
    mengerSpongeShader->setUniform("viewPortResolution", glm::vec2(currentResolution.width, currentResolution.height));

    pixel2DShader->use();
    pixel2DShader->setUniform("windowDimens", glm::vec2(currentResolution.width, currentResolution.height));
    pixel2DShader->setUniform("lowerLeftOffset", glm::vec2((currentResolution.width / 2) - 16.0, (currentResolution.height / 2) - 16.0));
  }

  if(hotPress(KeyboardInput_R))
  {
    showDebugWindows = !showDebugWindows;
    enableDefaultMouseCameraMovement(!showDebugWindows);
    enableCursor(window, showDebugWindows);
  }

  // TODO: Overriding viewport changes in Scenes class. Should we simply not call glViewport in parent classes?
  if(isActive(WindowInput_SizeChange)) {
    // NOTE: We need our viewport extent to match specified resolution instead of matching out window extent
    glViewport(0, 0, currentResolution.width, currentResolution.height);
  }
}

