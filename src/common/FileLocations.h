#pragma once

#include <vector>

// ==== shaders ====
// Common Shaders
#define COMMON_BASE_VERTEX "src/common/shaders/vertex/"
const char* const posNormalVertexShaderFileLoc = COMMON_BASE_VERTEX"PosNormalVertexShader.glsl";
const char* const posNormTexVertexShaderFileLoc = COMMON_BASE_VERTEX"PosNormTexVertexShader.glsl";
const char* const posVertexShaderFileLoc = COMMON_BASE_VERTEX"PosVertexShader.glsl";
const char* const frameBufferVertexShaderFileLoc = COMMON_BASE_VERTEX"FrameBufferVertexShader.glsl";
const char* const skyboxVertexShaderFileLoc = COMMON_BASE_VERTEX"SkyboxVertexShader.glsl";
const char* const textVertexShaderFileLoc = COMMON_BASE_VERTEX"TextVertexShader.glsl";
const char* const posTexModelVertexShaderFileLoc = COMMON_BASE_VERTEX"PosTexModelVertexShader.glsl";
const char* const posNormModelVertexShaderFileLoc = COMMON_BASE_VERTEX"PosNormModelVertexShader.glsl";
const char* const posTexVertexShaderFileLoc = COMMON_BASE_VERTEX"PosTexVertexShader.glsl";

#define COMMON_BASE_FRAGMENT "src/common/shaders/fragment/"
const char* const singleColorFragmentShaderFileLoc = COMMON_BASE_FRAGMENT"SingleColorFragmentShader.glsl";
const char* const skyboxFragmentShaderFileLoc = COMMON_BASE_FRAGMENT"SkyboxFragmentShader.glsl";
const char* const skyboxReflectionFragmentShaderFileLoc = COMMON_BASE_FRAGMENT"SkyboxReflectionFragmentShader.glsl";
const char* const emptyFragmentShaderFileLoc = COMMON_BASE_FRAGMENT"EmptyFragmentShader.glsl";
const char* const textFragmentShaderFileLoc = COMMON_BASE_FRAGMENT"TextFragmentShader.glsl";
const char* const depthVisualizerFragmentShaderFileLoc = COMMON_BASE_FRAGMENT"DepthBufferVisualizerFragmentShader.glsl";
const char* const negativeTextureFragmentShaderFileLoc = COMMON_BASE_FRAGMENT"NegativeTextureFragmentShader.glsl";

// Asteroid Belt Shaders
#define ASTEROID_BELT_BASE "src/scenes/AsteroidBelt/"
const char* const AsteroidVertexShaderFileLoc = ASTEROID_BELT_BASE"AsteroidVertexShader.glsl";
const char* const textureModelFragmentShaderFileLoc = ASTEROID_BELT_BASE"TextureModelFragmentShader.glsl";

// Floor Shaders
#define FLOOR_BASE "src/scenes/Floor/"
const char* const lightSpaceVertexShaderFileLoc = FLOOR_BASE"LightSpaceVertexShader.glsl";
const char* const directionalLightShadowMapFragmentShaderFileLoc = FLOOR_BASE"DirectionalLightShadowMapFragmentShader.glsl";
const char* const simpleDepthVertexShaderFileLoc = FLOOR_BASE"SimpleDepthVertexShader.glsl";
const char* const tbnGeometryShaderFileLoc = FLOOR_BASE"TBNGeometryShader.glsl";

// Infinite Cube Shaders
#define INFINITE_CUBE_BASE "src/scenes/InfiniteCube/"
const char* const cropCenterSquareTexFragmentShader = INFINITE_CUBE_BASE"CropCenterSquareTexFragmentShader.glsl";
const char* const discardAlphaFragmentShaderFileLoc = INFINITE_CUBE_BASE"DiscardAlphaFragmentShader.glsl";
const char* const basicTextureFragmentShaderFileLoc = INFINITE_CUBE_BASE"BasicTextureFragmentShader.glsl";

// Ness Cubes Shaders
#define NESS_BASE "src/scenes/NessCube/"
const char* const cubeFragmentShaderFileLoc = NESS_BASE"CubeFragmentShader.glsl";
const char* const dirPosSpotLightModelFragmentShaderFileLoc = NESS_BASE"DirPosSpotLightModelFragmentShader.glsl";
const char* const kernel3x3TextureFragmentShaderFileLoc = NESS_BASE"Kernel3x3TextureFragmentShader.glsl";
const char* const kernel5x5TextureFragmentShaderFileLoc = NESS_BASE"Kernel5x5TextureFragmentShader.glsl";

// Reflect Refract Shaders
#define REFLECT_REFRACT_BASE "src/scenes/ReflectRefract/"
const char* const explodeGeometryShaderFileLoc = REFLECT_REFRACT_BASE"ExplodeGeometryShader.glsl";
const char* const posNormal10InstanceVertexShaderFileLoc = REFLECT_REFRACT_BASE"PosNormal10InstanceVertexShader.glsl";
const char* const skyboxRefractionFragmentShaderFileLoc = REFLECT_REFRACT_BASE"SkyboxRefractionFragmentShader.glsl";
const char* const normalVisualizerVertexShaderFileLoc = REFLECT_REFRACT_BASE"NormalVisualizerVertexShader.glsl";
const char* const triangleNormalVisualizerGeometryShaderFileLoc = REFLECT_REFRACT_BASE"TriangleNormalVisualizerGeometryShader.glsl";
const char* const normalVisualizer10InstanceVertexShaderFileLoc = REFLECT_REFRACT_BASE"NormalVisualizer10InstanceVertexShader.glsl";

// Room Shaders
#define ROOM_BASE "src/scenes/Room/"
const char* const positionalLightShadowMapFragmentShaderFileLoc = ROOM_BASE"PositionalLightShadowMapFragmentShader.glsl";
const char* const modelMatVertexShaderFileLoc = ROOM_BASE"ModelMatVertexShader.glsl";
const char* const linearDepthMapFragmentShaderFileLoc = ROOM_BASE"LinearDepthMapFragmentShader.glsl";
const char* const cubeMapGeometryShaderFileLoc = ROOM_BASE"CubeMapGeometryShader.glsl";

// 2D textures
const char* const diffuseTextureLoc = "src/data/diffuse_map_alpha_channel.png";
const char* const specularTextureLoc = "src/data/specular_map.png";
const char* const emissionTextureLoc = "src/data/emission_map.png";
const char* const outlineTextureLoc = "src/data/cube_outline.png";
const char* const outlineWhiteFillTextureLoc = "src/data/cube_outline_white_fill.png";
const char* const hardwoodTextureLoc = "src/data/hardwood.jpg";
const char* const cementTextureLoc = "src/data/cement.png";
const char* const brickAlbedoTextureLoc = "src/data/PBR/brick_albedo.jpg";
const char* const brickNormalTextureLoc = "src/data/PBR/brick_normal.jpg";
const char* const agedPlanksAlbedoTextureLoc = "src/data/PBR/agedplanks_albedo.png";
const char* const agedPlanksNormalTextureLoc = "src/data/PBR/agedplanks_normal.png";
const char* const greasyAlbedoTextureLoc = "src/data/PBR/greasy_albedo.png";
const char* const greasyNormalTextureLoc = "src/data/PBR/greasy_normal.png";
const char* const cementAlbedoTextureLoc = "src/data/PBR/cement_albedo.png";
const char* const cementNormalTextureLoc = "src/data/PBR/cement_normal.png";
const char* const cementHeightTextureLoc = "src/data/PBR/cement_height.png";
const char* const whiteSpruceAlbedoTextureLoc = "src/data/PBR/whitespruce_albedo.png";
const char* const whiteSpruceNormalTextureLoc = "src/data/PBR/whitespruce_normal.png";
const char* const whiteSpruceHeightTextureLoc = "src/data/PBR/whitespruce_height.png";
const char* const copperRockAlbedoTextureLoc = "src/data/PBR/copperrock_albedo.png";
const char* const copperRockNormalTextureLoc = "src/data/PBR/copperrock_normal.png";
const char* const copperRockHeightTextureLoc = "src/data/PBR/copperrock_height.png";
const char* const dungeonStoneAlbedoTextureLoc = "src/data/PBR/dungeonstone_albedo.png";
const char* const dungeonStoneNormalTextureLoc = "src/data/PBR/dungeonstone_normal.png";
const char* const dungeonStoneHeightTextureLoc = "src/data/PBR/dungeonstone_height.png";
const char* const waterWornStoneAlbedoTextureLoc = "src/data/PBR/waterwornstone_albedo.png";
const char* const waterWornStoneNormalTextureLoc = "src/data/PBR/waterwornstone_normal.png";
const char* const waterWornStoneHeightTextureLoc = "src/data/PBR/waterwornstone_height.png";
const char* const nanoSuitModelAbsoluteLoc = "src/data/models/nanosuit/nanosuit.obj";
const char* const asteroidModelAbsoluteLoc = "src/data/models/rock/rock.obj";
const char* const planetModelAbsoluteLoc = "src/data/models/planet/planet.obj";
const char* const bb8ModelAbsoluteLoc = "src/data/models/BB8.obj";
const char* const theObjectModelAbsoluteLoc = "src/data/models/TheObject.obj";

// Skybox Cube Map textures
#define skybox(folder, extension) std::vector<const char*>{  \
"src/data/skyboxes/"#folder"/right."#extension,              \
"src/data/skyboxes/"#folder"/left."#extension,               \
"src/data/skyboxes/"#folder"/top."#extension,                \
"src/data/skyboxes/"#folder"/bottom."#extension,             \
"src/data/skyboxes/"#folder"/front."#extension,              \
"src/data/skyboxes/"#folder"/back."#extension                \
}                                                            \

const std::vector<const char*> skyboxWaterFaceLocations = skybox(water, jpg);
const std::vector<const char*> skyboxGrassFaceLocations = skybox(grass_photograph, jpg);
const std::vector<const char*> skyboxSpaceBlueFaceLocations = skybox(space_blue, png);
const std::vector<const char*> skyboxSpaceLightBlueFaceLocations = skybox(space_light_blue, png);
const std::vector<const char*> skyboxSpaceRed1FaceLocations = skybox(space_red_1, png);
const std::vector<const char*> skyboxSpaceRed2FaceLocations = skybox(space_red_2, png);
const std::vector<const char*> skyboxSpaceRed3FaceLocations = skybox(space_red_3, png);
const std::vector<const char*> skyboxInterstellarFaceLocations = skybox(interstellar, png);
const std::vector<const char*> skyboxGloomyFaceLocations = skybox(gloomy, png);