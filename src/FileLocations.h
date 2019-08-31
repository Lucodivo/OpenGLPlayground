#pragma once

#include <vector>

// shaders
const char* const posNormTexVertexShaderFileLoc = "src/shaders/PosNormTexVertexShader.glsl";
const char* const cubeFragmentShaderFileLoc = "src/shaders/CubeFragmentShader.glsl";
const char* const posVertexShaderFileLoc = "src/shaders/PosVertexShader.glsl";
const char* const textureModelFragmentShaderFileLoc = "src/shaders/TextureModelFragmentShader.glsl";
const char* const dirPosSpotLightModelFragmentShaderFileLoc = "src/shaders/DirPosSpotLightModelFragmentShader.glsl";
const char* const directionalLightShadowMapFragmentShaderFIleLoc = "src/shaders/DirectionalLightShadowMapFragmentShader.glsl";
const char* const posTexModelVertexShaderFileLoc = "src/shaders/PosTexModelVertexShader.glsl";
const char* const posNormModelVertexShaderFileLoc = "src/shaders/PosNormModelVertexShader.glsl";
const char* const AsteroidVertexShaderFileLoc = "src/shaders/AsteroidVertexShader.glsl";
const char* const posTexVertexShaderFileLoc = "src/shaders/PosTexVertexShader.glsl";
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
const char* const textVertexShaderFileLoc = "src/shaders/TextVertexShader.glsl";
const char* const textFragmentShaderFileLoc = "src/shaders/TextFragmentShader.glsl";
const char* const simpleDepthVertexShaderFileLoc = "src/shaders/SimpleDepthVertexShader.glsl";
const char* const emptyFragmentShaderFileLoc = "src/shaders/EmptyFragmentShader.glsl";
const char* const lightSpaceVertexShaderFileLoc = "src/shaders/LightSpaceVertexShader.glsl";
const char* const modelMatVertexShaderFileLoc = "src/shaders/ModelMatVertexShader.glsl";
const char* const cubeMapGeometryShaderFileLoc = "src/shaders/CubeMapGeometryShader.glsl";
const char* const linearDepthMapFragmentShaderFileLoc = "src/shaders/LinearDepthMapFragmentShader.glsl";
const char* const positionalLightShadowMapFragmentShaderFileLoc = "src/shaders/PositionalLightShadowMapFragmentShader.glsl";

// 2D textures
const char* const diffuseTextureLoc = "src/data/diffuse_map_alpha_channel.png";
const char* const specularTextureLoc = "src/data/specular_map.png";
const char* const emissionTextureLoc = "src/data/emission_map.png";
const char* const outlineTextureLoc = "src/data/cube_outline.png";
const char* const outlineWhiteFillTextureLoc = "src/data/cube_outline_white_fill.png";
const char* const hardwoodTextureLoc = "src/data/hardwood.jpg";
const char* const cementTextureLoc = "src/data/cement.png";
const char* const nanoSuitModelAbsoluteLoc = "C:/developer/repos/OpenGLScenes/LearnOpenGL/src/data/models/nanosuit/nanosuit.obj";
const char* const asteroidModelAbsoluteLoc = "C:/developer/repos/OpenGLScenes/LearnOpenGL/src/data/models/rock/rock.obj";
const char* const planetModelAbsoluteLoc = "C:/developer/repos/OpenGLScenes/LearnOpenGL/src/data/models/planet/planet.obj";
const char* const bb8ModelAbsoluteLoc = "C:/developer/repos/OpenGLScenes/LearnOpenGL/src/data/models/BB8.obj";
const char* const theObjectModelAbsoluteLoc = "C:/developer/repos/OpenGLScenes/LearnOpenGL/src/data/models/TheObject.obj";


#define skybox(folder, extension) std::vector<const char*>{  \
"src/data/skyboxes/"#folder"/right."#extension,              \
"src/data/skyboxes/"#folder"/left."#extension,               \
"src/data/skyboxes/"#folder"/top."#extension,                \
"src/data/skyboxes/"#folder"/bottom."#extension,             \
"src/data/skyboxes/"#folder"/front."#extension,              \
"src/data/skyboxes/"#folder"/back."#extension                \
}                                                            \

// Cube Map textures
const std::vector<const char*> skyboxWaterFaceLocations = skybox(water, jpg);
const std::vector<const char*> skyboxGrassFaceLocations = skybox(grass_photograph, jpg);
const std::vector<const char*> skyboxSpaceBlueFaceLocations = skybox(space_blue, png);
const std::vector<const char*> skyboxSpaceLightBlueFaceLocations = skybox(space_light_blue, png);
const std::vector<const char*> skyboxSpaceRed1FaceLocations = skybox(space_red_1, png);
const std::vector<const char*> skyboxSpaceRed2FaceLocations = skybox(space_red_2, png);
const std::vector<const char*> skyboxSpaceRed3FaceLocations = skybox(space_red_3, png);
const std::vector<const char*> skyboxInterstellarFaceLocations = skybox(interstellar, png);
const std::vector<const char*> skyboxGloomyFaceLocations = skybox(gloomy, png);