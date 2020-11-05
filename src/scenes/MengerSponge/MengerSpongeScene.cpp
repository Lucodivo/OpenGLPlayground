//
// Created by Connor on 11/21/2019.
//

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "MengerSpongeScene.h"
#include "../../common/FileLocations.h"
#include "../../common/ObjectData.h"
#include "../../common/Util.h"

MengerSpongeScene::MengerSpongeScene(GLFWwindow* window, uint32 initScreenHeight, uint32 initScreenWidth)
        : GodModeScene(window, initScreenHeight, initScreenWidth),
          mengerSpongeShader(UVCoordVertexShaderFileLoc, MengerSpongeFragmentShaderFileLoc),
          pixel2DShader(pixel2DVertexShaderFileLoc, textureFragmentShaderFileLoc),
          cubeShader(CubePosNormTexVertexShaderFileLoc, CubeTextureFragmentShaderFileLoc) {
  if(showDebugWindows) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
}

void MengerSpongeScene::runScene()
{
  uint32 quadVAO, quadVBO, quadEBO;
  initializeFrameBufferQuadVertexAttBuffers(quadVAO, quadVBO, quadEBO);

  // TODO: delete
  uint32 cubeVAO, cubeVBO, cubeEBO;
  initializeCubePosNormTexVertexAttBuffers(cubeVAO, cubeVBO, cubeEBO);

  renderLoop(quadVAO, cubeVAO);

  glDeleteVertexArrays(1, &quadVAO);
  glDeleteBuffers(1, &quadVBO);
  glDeleteBuffers(1, &quadEBO);
}

void MengerSpongeScene::renderLoop(uint32 quadVAO, uint32 cubeVAO)
{
  resolution largestResolution = screenResolutions[ArrayCount(screenResolutions) - 1];
  Framebuffer dynamicResolutionFBO = initializeFrameBuffer(largestResolution.width, largestResolution.height);

  const float cubeScale = 10.0f;
  const glm::vec3 cubePos = glm::vec3(0.0, 0.0, 0.0);
  const glm::vec3 cubeColor = glm::vec3(0.5, 0.0, 0.0);
  const glm::vec3 cubeRotAxis = glm::vec3(1.0, 1.0, -1.0);

  // NOTE: This is helps maintain same projection for both the ray marching and rasterization
  // If how we shoot rays change, this must change. If this changes, how we shoot rays must change.
  const float rayMarchFovVertical = glm::radians(53.14f);
  const glm::mat4 projectionMat = glm::perspective(rayMarchFovVertical, (float32)currentResolution.width / (float32)currentResolution.height, 0.1f, 200.0f);

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

  const glm::vec3 directionalLightAmb = glm::vec3(0.15, 0.15, 0.15);
  const glm::vec3 directionalLightDiff = glm::vec3(0.6, 0.6, 0.6);
  const glm::vec3 directionalLightSpec = glm::vec3(0.8, 0.8, 0.8);
  const glm::vec3 directionalLightDir = glm::vec3(1.0, -1.0, -1.0);

  mengerSpongeShader.use();
  mengerSpongeShader.setUniform("viewPortResolution", glm::vec2(currentResolution.width, currentResolution.height));
  mengerSpongeShader.setUniform("directionalLight.color.ambient", directionalLightAmb);
  mengerSpongeShader.setUniform("directionalLight.color.diffuse", directionalLightDiff);
  mengerSpongeShader.setUniform("directionalLight.color.specular", directionalLightSpec);
  mengerSpongeShader.setUniform("directionalLight.direction", directionalLightDir);

  cubeShader.use();
  cubeShader.setUniform("projection", projectionMat);
  cubeShader.setUniform("material.diffTexture", 0);
  cubeShader.setUniform("material.specTexture", 1);
  cubeShader.setUniform("material.shininess", 16.0f);
  cubeShader.setUniform("directionalLight.color.ambient", directionalLightAmb);
  cubeShader.setUniform("directionalLight.color.diffuse", directionalLightDiff);
  cubeShader.setUniform("directionalLight.color.specular", directionalLightSpec);
  cubeShader.setUniform("directionalLight.direction", directionalLightDir);

//  pixel2DShader.use();
//  pixel2DShader.setUniform("windowDimens", glm::vec2(currentResolution.width, currentResolution.height));
//  pixel2DShader.setUniform("lowerLeftOffset", glm::vec2((currentResolution.width / 2) - 16.0, (currentResolution.height / 2) - 16.0));
//  pixel2DShader.setUniform("spriteDimens", glm::vec2(textureWidth, textureHeight));

  glBindVertexArray(quadVAO);

  camera.Position = glm::vec3(0.0f, 1.0f, 30.0f);

  lastFrame = (float32)glfwGetTime();
  float32 startTime = lastFrame;

  // background clear color
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  const float32 frameTime = 0.2f;
  int32 numSamples = 1;

  // NOTE: the viewport dictates the area of the bound
  glViewport(0, 0, currentResolution.width, currentResolution.height);

  // NOTE: render/game loop
  while (glfwWindowShouldClose(window) == GL_FALSE)
  {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (showDebugWindows){
      //ImGui::ShowDemoWindow(&showDebugWindows);

      ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.50f);
      // Simplified Settings (expose floating-pointer border sizes as boolean representing 0.0f or 1.0f)
      if (ImGui::SliderInt("Supersamples", &numSamples, 1, 8)) {}
      ImGui::PopItemWidth();
    }

    float32 t = (float32)glfwGetTime() - startTime;
    deltaTime = t - lastFrame;
    lastFrame = t;

    if(((uint32)(t / frameTime) % 2) == 0) {
      cubeShader.setUniform("material.diffTexture", 0);
      cubeShader.setUniform("material.specTexture", 1);
    } else {
      cubeShader.setUniform("material.diffTexture", 2);
      cubeShader.setUniform("material.specTexture", 3);
    }

    // Auto run forward
//    glm::vec3 deltaCameraPos = camera.Front;
//    deltaCameraPos *= 0.07;
//    camera.Position += deltaCameraPos;
    glm::mat4 cameraMat = camera.GetViewMatrix(deltaTime);

    // bind our frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, dynamicResolutionFBO.id);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mengerSpongeShader.use();
    glBindVertexArray(quadVAO);

    if(mengerSpongeShader.updateFragmentShaderIfOutdated()) {
      mengerSpongeShader.use();
      mengerSpongeShader.setUniform("viewPortResolution", glm::vec2(currentResolution.width, currentResolution.height));
      mengerSpongeShader.setUniform("directionalLight.color.ambient", directionalLightAmb);
      mengerSpongeShader.setUniform("directionalLight.color.diffuse", directionalLightDiff);
      mengerSpongeShader.setUniform("directionalLight.color.specular", directionalLightSpec);
      mengerSpongeShader.setUniform("directionalLight.direction", directionalLightDir);
    }

    mengerSpongeShader.setUniform("rayOrigin", camera.Position);
    mengerSpongeShader.setUniform("elapsedTime", t);
    mengerSpongeShader.setUniform("view", cameraMat);
    mengerSpongeShader.setUniform("projection", projectionMat);
    mengerSpongeShader.setUniform("cameraPos", camera.Position);
    mengerSpongeShader.setUniform("numSamples", numSamples);
    glDrawElements(GL_TRIANGLES, // drawing mode
                   6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
                   GL_UNSIGNED_INT, // type of the indices
                   0 /* offset in the EBO */);

//    static uint32 numSnapshots = 0;
//    if(numSnapshots < 1) {
//      snapshot(currentResolution.width, currentResolution.height, "C:\\Users\\Connor\\Desktop\\tmp\\snapshot.bmp", dynamicResolutionFBO);
//      ++numSnapshots;
//    }

    glm::mat4 cubeModel;
    cubeModel = glm::scale(cubeModel, glm::vec3(cubeScale));
    cubeModel = glm::translate(cubeModel, cubePos);
    cubeModel = glm::rotate(cubeModel, t * glm::radians(20.0f), cubeRotAxis);

    cubeShader.use();
    cubeShader.setUniform("model", cubeModel);
    cubeShader.setUniform("view", cameraMat);
    cubeShader.setUniform("cameraPos", camera.Position);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, // drawing mode
                   cubePosNormTexNumElements * 3, // number of elements to draw (3 vertices per triangle * 2 triangles per face * 6 faces)
                   GL_UNSIGNED_INT, // type of the indices
                   0); // offset in the EBO
    glBindVertexArray(0);

//    pixel2DShader.use();
//    glDrawElements(GL_TRIANGLES, // drawing mode
//                   6, // number of elements to draw (3 vertices per triangle * 2 triangles per quad)
//                   GL_UNSIGNED_INT, // type of the indices
//                   0); // offset in the EBO

    // bind our frame buffer
    //NOTE: dynamicResolutionFBO is already bound to GL_READ_FRAMEBUFFER
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, currentResolution.width, currentResolution.height, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    // Rendering ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window); // swaps double buffers (call after all render commands are completed)
    glfwPollEvents(); // checks for events (ex: keyboard/mouse input)
  }
}

void MengerSpongeScene::frameBufferSize(uint32 width, uint32 height)
{
  FirstPersonScene::frameBufferSize(width, height);
  // NOTE: We need our viewport extent to match specified resolution instead of matching out window extent
  glViewport(0, 0, currentResolution.width, currentResolution.height);
}

void MengerSpongeScene::key_E_pressed() {
  if(++currentResolutionIndex >= ArrayCount(screenResolutions)) {
    currentResolutionIndex = 0;
  }

  currentResolution = screenResolutions[currentResolutionIndex];

  glViewport(0, 0, currentResolution.width, currentResolution.height);

  mengerSpongeShader.use();
  mengerSpongeShader.setUniform("viewPortResolution", glm::vec2(currentResolution.width, currentResolution.height));

  pixel2DShader.use();
  pixel2DShader.setUniform("windowDimens", glm::vec2(currentResolution.width, currentResolution.height));
  pixel2DShader.setUniform("lowerLeftOffset", glm::vec2((currentResolution.width / 2) - 16.0, (currentResolution.height / 2) - 16.0));
}

void MengerSpongeScene::key_Q_pressed() {
  if(--currentResolutionIndex == -1) {
    currentResolutionIndex = ArrayCount(screenResolutions) - 1;
  }

  currentResolution = screenResolutions[currentResolutionIndex];

  glViewport(0, 0, currentResolution.width, currentResolution.height);

  mengerSpongeShader.use();
  mengerSpongeShader.setUniform("viewPortResolution", glm::vec2(currentResolution.width, currentResolution.height));

  pixel2DShader.use();
  pixel2DShader.setUniform("windowDimens", glm::vec2(currentResolution.width, currentResolution.height));
  pixel2DShader.setUniform("lowerLeftOffset", glm::vec2((currentResolution.width / 2) - 16.0, (currentResolution.height / 2) - 16.0));
}

void MengerSpongeScene::key_Tab_pressed() {
  showDebugWindows = !showDebugWindows;
  if(showDebugWindows) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  } else {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }
}

void MengerSpongeScene::mouseMovement(float32 xOffset, float32 yOffset)
{
  if(showDebugWindows) return;
  FirstPersonScene::mouseMovement(xOffset, yOffset);
}
