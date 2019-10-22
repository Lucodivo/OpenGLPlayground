#version 330 core
out vec4 FragColor;

struct LightColor
{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PositionalLight
{
  vec3 position;
  LightColor color;
};

struct LightAttenuation
{
  float constant;
  float linear;
  float quadratic;
};

struct Material
{
  sampler2D diffTexture1;
  sampler2D specTexture1;
};

uniform samplerCube shadowCubeMap;
uniform vec3 viewPos;
uniform PositionalLight positionalLight;
uniform LightAttenuation attenuation;
uniform Material material;
uniform float lightFarPlane;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

vec3 calcPositionalLight(PositionalLight positionalLight);
float calcShadow(float lightDirNormalDot, vec3 fragToLightDist);
float calcAttenuation(float distance);

vec3 diffColor;
vec3 specColor;

const float shadowBiasMax = 0.05;
const float shadowBiasMin = 0.01;

void main()
{
  diffColor = texture(material.diffTexture1, TexCoords).rgb;
  specColor = texture(material.specTexture1, TexCoords).rgb;

  vec3 finalColor = calcPositionalLight(positionalLight);

  // apply gamma correction
  float gamma = 2.2;
  FragColor = vec4(pow(finalColor, vec3(1.0 / gamma)), 1.0);
}

vec3 calcPositionalLight(PositionalLight positionalLight)
{
  vec3 normal = normalize(Normal);
  vec3 lightDir = normalize(positionalLight.position - FragPos);
  float lightDirNormalDot = dot(lightDir, normal);

  // ambient light
  vec3 ambient = positionalLight.color.ambient * diffColor;

  // get vector between from light to frag position
  vec3 lightToFrag = FragPos - positionalLight.position;

  float shadowInverse = 1.0 - calcShadow(lightDirNormalDot, lightToFrag);
  float attenuation = calcAttenuation(length(lightToFrag));

  // diffuse light
  float diffStrength = max(lightDirNormalDot, 0.0);
  vec3 diffuse = positionalLight.color.diffuse * diffStrength * diffColor;

  // specular light
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float specStrength = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
  vec3 specular = positionalLight.color.specular * specStrength * specColor;

  return (ambient + ((diffuse + specular) * shadowInverse * attenuation));
}

vec3 sampleOffsetDirections[20] = vec3[]
(
vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
);

float calcShadow(float lightDirNormalDot, vec3 lightToFrag)
{
  // now get current linear depth as the length between the fragment and light position
  float currentDepth = length(lightToFrag);
  // now test for shadows
  float bias = max(shadowBiasMax * (1.0 - lightDirNormalDot), shadowBiasMin);

  // soften shadow edges with PCF (percentage-closer filtering)
  float shadow = 0.0;
  float samples = 20.0;
  float diskRadius = 0.05;
  for (int i = 0; i < samples; ++i)
  {
    // use the light to fragment vector to sample from the depth map
    float closestDepth = texture(shadowCubeMap, lightToFrag + sampleOffsetDirections[i] * diskRadius).r;
    // it is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= lightFarPlane;
    if (currentDepth - bias > closestDepth)
    {
      shadow += 1.0;
    }
  }
  shadow /= samples;

  return shadow;
}

float calcAttenuation(float distance)
{
  return (1.0 / (attenuation.constant + (attenuation.linear * distance) + (attenuation.quadratic * distance * distance)));
}