#pragma once

// shaders
const char* const cubeVertexShaderFileLoc = "src/shaders/CubeVertexShader.glsl";
const char* const cubeFragmentShaderFileLoc = "src/shaders/CubeFragmentShader.glsl";
const char* const lightVertexShaderFileLoc = "src/shaders/LightSourceVertexShader.glsl";
const char* const lightFragmentShaderFileLoc = "src/shaders/LightSourceFragmentShader.glsl";
const char* const modelVertexShaderFileLoc = "src/shaders/ModelVertexShader.glsl";
const char* const modelFragmentShaderFileLoc = "src/shaders/ModelFragmentShader.glsl";
const char* const depthFragmentShaderFileLoc = "src/shaders/DepthBufferVisualizerFragmentShader.glsl";
const char* const stencilFragmentShaderFileLoc = "src/shaders/SingleColorFragmentShader.glsl";
const char* const frameBufferVertexShaderFileLoc = "src/shaders/FrameBufferVertexShader.glsl";
const char* const frameBufferFragmentShaderFileLoc = "src/shaders/FrameBufferFragmentShader.glsl";
const char* const negativeFrameBufferFragmentShaderFileLoc = "src/shaders/NegativeFrameBufferFragmentShader.glsl";
const char* const kernelFrameBufferFragmentShaderFileLoc = "src/shaders/KernelFrameBufferFragmentShader.glsl";
const char* const infiniteCubeFragmentShaderFileLoc = "src/shaders/InfiniteCubeFragmentShader.glsl";
const char* const discardAlphaFragmentShaderFileLoc = "src/shaders/DiscardAlphaFragmentShader.glsl";

// textures
const char* const diffuseTextureLoc = "src/data/diffuse_map_alpha_channel.png";
const char* const specularTextureLoc = "src/data/specular_map.png";
const char* const emissionTextureLoc = "src/data/emission_map.png";
const char* const outlineTextureLoc = "src/data/cube_outline.png";
const char* const outlineWhiteFillTextureLoc = "src/data/cube_outline_white_fill.png";
const char* const nanoSuitModelLoc = "C:/developer/repos/LearnOpenGL/LearnOpenGL/src/data/nanosuit/nanosuit.obj";