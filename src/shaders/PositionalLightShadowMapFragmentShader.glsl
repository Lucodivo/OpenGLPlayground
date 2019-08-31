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

struct Material
{
  sampler2D diffTexture1;
  sampler2D specTexture1;
};

struct Attenuation
{
  float constant;
  float linear;
  float quadratic;
};

uniform samplerCube shadowCubeMap;
uniform vec3 viewPos;
uniform PositionalLight positionalLight;
uniform Attenuation attenuation;
uniform Material material;
uniform float farPlane;

in vec3 Normal;
in vec3 FragPos;
in vec2 TextureCoord;

vec3 calcPositionalLight(PositionalLight positionalLight);
float calcShadow(float lightDirNormalDot);

vec3 diffColor;
vec3 specColor;

const float shadowBiasMax = 0.05;
const float shadowBiasMin = 0.01;

void main()
{
  diffColor = texture(material.diffTexture1, TextureCoord).rgb;
  specColor = texture(material.specTexture1, TextureCoord).rgb;

  vec3 finalColor = calcPositionalLight(positionalLight);

  // apply gamma correction
  float gamma = 2.2;
  FragColor = vec4(pow(finalColor, vec3(1.0 / gamma)), 1.0);
}

vec3 calcPositionalLight(PositionalLight positionalLight)
{
  vec3 normal = normalize(Normal);

  // ambient light
  vec3 ambient = positionalLight.color.ambient * diffColor;

  // diffuse light
  vec3 lightDir = normalize(positionalLight.position - FragPos);
  float lightDirNormalDot = dot(lightDir, normal); // TODO: Dot product used to be backwards
  float diffStrength = max(lightDirNormalDot, 0.0);
  vec3 diffuse = positionalLight.color.diffuse * diffStrength * diffColor;

  // specular light
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, normal);
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float specStrength = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
  vec3 specular = positionalLight.color.specular * specStrength * specColor;

  float shadowInverse = 1.0 - calcShadow(lightDirNormalDot);

  return (ambient + ((diffuse + specular) * shadowInverse));
}

float calcShadow(float lightDirNormalDot)
{
  
  // get vector between fragment position and light position
  vec3 fragToLight = FragPos - positionalLight.position;
  // use the light to fragment vector to sample from the depth map    
  float closestDepth = texture(shadowCubeMap, fragToLight).r;
  // it is currently in linear range between [0,1]. Re-transform back to original value
  closestDepth *= farPlane;
  // now get current linear depth as the length between the fragment and light position
  float currentDepth = length(fragToLight);
  // now test for shadows
  float bias = max(shadowBiasMax * (1.0 - lightDirNormalDot), shadowBiasMin);
  float shadow = (currentDepth - bias) > closestDepth ? 1.0 : 0.0;

  // TODO: soften shadows
  //for(int x = -1; x <= 1; ++x)
  //{
  //  for(int y = -1; y <= 1; ++y)
  //  {
  //    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
  //    float pcfDepth = texture(shadowCubeMap, projCoords.xy + vec2(x, y) * texelSize).r;
  //    // check whether current frag pos is in shadow
  //    float bias = max(shadowBiasMax * (1.0 - normalLightDirDot), shadowBiasMin);
  //    shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;
  //  }
  //}
  //shadow /= 9.0f;

  return shadow;
}