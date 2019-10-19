#version 330 core

struct LightColor{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material {
  sampler2D diffuse;
  sampler2D normal;
};

uniform sampler2D shadowMap;
uniform LightColor directionalLightColor;
uniform vec3 directionalLightDir; // normalized direction from origin to light
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

vec3 diffColor;
vec3 specColor;

const float shadowBiasMax = 0.005;
const float shadowBiasMin = 0.0005;

void main()
{
	diffColor = texture(material.diffuse, fs_in.TextureCoord).rgb;

  vec3 finalColor = calcDirectionalLightColor();

	// apply gamma correction
	float gamma = 2.2;
	FragColor = vec4(pow(finalColor, vec3(1.0 / gamma)), 1.0);
}

vec3 calcDirectionalLightColor() {

	// ambient light
	vec3 ambient = directionalLightColor.ambient * diffColor;

	// diffuse light
  vec3 normal = texture(material.normal, fs_in.TextureCoord).rgb;
  normal = normalize((normal * 2) - vec3(1.0, 1.0, 1.0)); // normalize seems unnecessary
  //normal = normalize(fs_in.TBN * normal);
  float normalLightDirDot = dot(normal, fs_in.TangentLightDir);
	float diffStrength = max(normalLightDirDot, 0.0);
	vec3 diffuse = directionalLightColor.diffuse * diffStrength * diffColor;

  // specular light
  vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentPos);
  vec3 halfwayDir = normalize(fs_in.TangentLightDir + viewDir);
  float specStrength = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
  vec3 specular = directionalLightColor.specular * specStrength;

  // shadow
  float shadowInverse = 1.0 - calcShadow(fs_in.PosLightSpace, dot(normal, directionalLightDir));

	return (ambient + ((diffuse + specular) * shadowInverse));
}

float calcShadow(vec4 posLightSpace, float normalLightDirDot)
{
  // perform perspective divide
  vec3 projCoords = posLightSpace.xyz / posLightSpace.w;
  // transform NDC from the range of [-1,1] to the range of [0,1]
  projCoords = projCoords * 0.5 + 0.5;

  // get depth of current fragment from light's perspective
  float currentDepth = projCoords.z;

  if(currentDepth > 1.0) return 0.0f;

  float shadow = 0.0f;
  vec2 texelSize = 1.0f / textureSize(shadowMap, 0);

  for(int x = -1; x <= 1; ++x)
  {
    for(int y = -1; y <= 1; ++y)
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