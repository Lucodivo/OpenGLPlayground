#version 330 core

struct LightColor{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct Material {
  sampler2D diffuse;
  sampler2D normal;
  sampler2D height;
};

uniform sampler2D shadowMap;
uniform LightColor directionalLightColor;
uniform vec3 directionalLightDir;// normalized direction from origin to light
uniform Material material;

in VS_OUT {
  vec2 TextureCoord;
  vec4 PosLightSpace;
  vec3 TangentPos;
  vec3 TangentLightDir;
  vec3 TangentViewPos;
} fs_in;

out vec4 FragColor;

vec3 calcDirectionalLightColor();
float calcShadow(vec4 posLightSpace, float normalLightDirDot);
vec2 parallaxMapping(vec2 texCoords, vec3 viewDir);
vec2 steepParallaxMapping(vec2 texCoords, vec3 viewDir);
vec2 parallaxOcculusMapping(vec2 texCoords, vec3 viewDir);

vec3 diffColor;
vec3 specColor;
vec2 texCoords;

const float shadowBiasMax = 0.005;
const float shadowBiasMin = 0.0005;
const float heightScale = 0.1;

void main()
{
  vec3 finalColor = calcDirectionalLightColor();

  // apply gamma correction
  float gamma = 2.2;
  FragColor = vec4(pow(finalColor, vec3(1.0 / gamma)), 1.0);
}

vec3 calcDirectionalLightColor() {
  vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentPos);
  texCoords = parallaxOcculusMapping(fs_in.TextureCoord, viewDir);
  if (texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0) discard;// remove border artifacts caused by parallax mapping
  diffColor = texture(material.diffuse, texCoords).rgb;

  // ambient light
  vec3 ambient = directionalLightColor.ambient * diffColor;

  // diffuse light
  vec3 normal = texture(material.normal, texCoords).rgb;
  normal = normalize((normal * 2) - vec3(1.0, 1.0, 1.0));// normalize seems unnecessary
  float normalLightDirDot = dot(normal, fs_in.TangentLightDir);
  float diffStrength = max(normalLightDirDot, 0.0);
  vec3 diffuse = directionalLightColor.diffuse * diffStrength * diffColor;

  // specular light
  vec3 halfwayDir = normalize(fs_in.TangentLightDir + viewDir);
  float specStrength = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
  vec3 specular = directionalLightColor.specular * specStrength;

  // shadow
  float shadowInverse = 1.0 - calcShadow(fs_in.PosLightSpace, dot(normal, directionalLightDir));

  return (ambient + ((diffuse + specular) * shadowInverse));
}

vec2 parallaxOcculusMapping(vec2 texCoords, vec3 viewDir) {
  const float minLayers = 8.0;
  const float maxLayers = 32.0;
  float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));

  float layerDepth = 1.0 / numLayers;
  float currentLayerDepth = 0.0;
  vec2 P = viewDir.xy * heightScale;
  vec2 deltaTexCoords = P / numLayers;

  vec2 currentTexCoords = texCoords;
  float currentDepthMapValue = 1 - texture(material.height, currentTexCoords).r;

  while (currentLayerDepth < currentDepthMapValue) {
    currentTexCoords -= deltaTexCoords;
    currentDepthMapValue = 1 - texture(material.height, currentTexCoords).r;
    currentLayerDepth += layerDepth;
  }

  // get texture coordinates before collision (reverse operations)
  vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

  // get depth after and before collision for linear interpolation
  float afterDepth  = currentDepthMapValue - currentLayerDepth;
  float beforeDepth = (1 - texture(material.height, prevTexCoords).r) - currentLayerDepth + layerDepth;

  // interpolation of texture coordinates
  float weight = afterDepth / (afterDepth - beforeDepth);
  vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

  return finalTexCoords;
}

vec2 steepParallaxMapping(vec2 texCoords, vec3 viewDir) {
  const float minLayers = 8.0;
  const float maxLayers = 32.0;
  float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));

  float layerDepth = 1.0 / numLayers;
  float currentLayerDepth = 0.0;
  vec2 P = viewDir.xy * heightScale;
  vec2 deltaTexCoords = P / numLayers;

  vec2 currentTexCoords = texCoords;
  float currentDepthMapValue = 1 - texture(material.height, currentTexCoords).r;

  while (currentLayerDepth < currentDepthMapValue) {
    currentTexCoords -= deltaTexCoords;
    currentDepthMapValue = 1 - texture(material.height, currentTexCoords).r;
    currentLayerDepth += layerDepth;
  }

  return currentTexCoords;
}

vec2 parallaxMapping(vec2 texCoords, vec3 viewDir) {
  float height = 1.0f - texture(material.height, texCoords).r;
  return texCoords - (viewDir.xy * (height * heightScale));
}

// Using an albedo texture for parallax mapping instead of the height map
// Seems to give an interesting effect of a layer of "ice" resting on top of the surface
//vec2 parallaxMapping(vec2 texCoords, vec3 viewDir) {
//  float height =  1.0f - texture(material.diffuse, texCoords).r;
//  vec2 p = viewDir.xy / viewDir.z * (height * heightScale);
//  return texCoords - p;
//}

float calcShadow(vec4 posLightSpace, float normalLightDirDot)
{
  // perform perspective divide
  vec3 projCoords = posLightSpace.xyz / posLightSpace.w;
  // transform NDC from the range of [-1,1] to the range of [0,1]
  projCoords = projCoords * 0.5 + 0.5;

  // get depth of current fragment from light's perspective
  float currentDepth = projCoords.z;

  if (currentDepth > 1.0) return 0.0f;

  float shadow = 0.0f;
  vec2 texelSize = 1.0f / textureSize(shadowMap, 0);

  for (int x = -1; x <= 1; ++x)
  {
    for (int y = -1; y <= 1; ++y)
    {
      // get closest depth value from light's perspective (using [0,1] range posLight as coords)
      float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
      // check whether current frag pos is in shadow
      float bias = max(shadowBiasMax * (1.0 - normalLightDirDot), shadowBiasMin);
      shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;
    }
  }
  shadow /= 9.0f;

  return shadow;
}