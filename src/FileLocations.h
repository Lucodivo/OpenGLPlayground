#pragma once

// shaders
const char* const posTexNormalVertexShaderFileLoc = "src/shaders/PosTexNormalVertexShader.glsl";
const char* const cubeFragmentShaderFileLoc = "src/shaders/CubeFragmentShader.glsl";
const char* const posVertexShaderFileLoc = "src/shaders/PosVertexShader.glsl";
const char* const lightFragmentShaderFileLoc = "src/shaders/LightSourceFragmentShader.glsl";
const char* const modelFragmentShaderFileLoc = "src/shaders/ModelFragmentShader.glsl";
const char* const depthFragmentShaderFileLoc = "src/shaders/DepthBufferVisualizerFragmentShader.glsl";
const char* const singleColorFragmentShaderFileLoc = "src/shaders/SingleColorFragmentShader.glsl";
const char* const frameBufferVertexShaderFileLoc = "src/shaders/FrameBufferVertexShader.glsl";
const char* const basicTextureFragmentShaderFileLoc = "src/shaders/BasicTextureFragmentShader.glsl";
const char* const negativeTextureFragmentShaderFileLoc = "src/shaders/NegativeTextureFragmentShader.glsl";
const char* const kernel3x3TextureFragmentShaderFileLoc = "src/shaders/Kernel3x3TextureFragmentShader.glsl";
const char* const kernel5x5TextureFragmentShaderFileLoc = "src/shaders/Kernel5x5TextureFragmentShader.glsl";
const char* const cropCenterSquareTexFragmentShader = "src/shaders/CropCenterSquareTexFragmentShader.glsl";
const char* const discardAlphaFragmentShaderFileLoc = "src/shaders/DiscardAlphaFragmentShader.glsl";
const char* const skyboxVertexShaderFileLoc = "src/shaders/SkyboxVertexShader.glsl";
const char* const skyboxFragmentShaderFileLoc = "src/shaders/SkyboxFragmentShader.glsl";
const char* const posNormalVertexShaderFileLoc = "src/shaders/PosNormalVertexShader.glsl";
const char* const posNormal10InstanceVertexShaderFileLoc = "src/shaders/PosNormal10InstanceVertexShader.glsl";
const char* const skyboxReflectionFragmentShaderFileLoc = "src/shaders/SkyboxReflectionFragmentShader.glsl";
const char* const skyboxRefractionFragmentShaderFileLoc = "src/shaders/SkyboxRefractionFragmentShader.glsl";
const char* const explodeGeometryShaderFileLoc = "src/shaders/ExplodeGeometryShader.glsl";
const char* const triangleNormalVisualizerGeometryShaderFileLoc = "src/shaders/TriangleNormalVisualizerGeometryShader.glsl";
const char* const normalVisualizerVertexShaderFileLoc = "src/shaders/NormalVisualizerVertexShader.glsl";
const char* const normalVisualizer10InstanceVertexShaderFileLoc = "src/shaders/NormalVisualizer10InstanceVertexShader.glsl";

// 2D textures
const char* const diffuseTextureLoc = "src/data/diffuse_map_alpha_channel.png";
const char* const specularTextureLoc = "src/data/specular_map.png";
const char* const emissionTextureLoc = "src/data/emission_map.png";
const char* const outlineTextureLoc = "src/data/cube_outline.png";
const char* const outlineWhiteFillTextureLoc = "src/data/cube_outline_white_fill.png";
const char* const nanoSuitModelAbsoluteLoc = "C:/developer/repos/LearnOpenGL/LearnOpenGL/src/data/nanosuit/nanosuit.obj";

// Cube Map textures
const std::vector<const char*> skyboxWaterFaceLocations{
  "src/data/skybox1/right.jpg",
  "src/data/skybox1/left.jpg",
  "src/data/skybox1/top.jpg",
  "src/data/skybox1/bottom.jpg",
  "src/data/skybox1/front.jpg",
  "src/data/skybox1/back.jpg"
};

// Cube Map textures
const std::vector<const char*> skyboxGrassFaceLocations{
  "src/data/skybox2/right.jpg",
  "src/data/skybox2/left.jpg",
  "src/data/skybox2/top.jpg",
  "src/data/skybox2/bottom.jpg",
  "src/data/skybox2/front.jpg",
  "src/data/skybox2/back.jpg"
};