#version 330 core

struct LightColor{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct LightAttenuation {
	float constant;
	float linear;
	float quadratic;
};

struct PositionalLight{
	vec3 position;
	LightColor color;
};

struct Material {
	sampler2D diffTexture1;
	sampler2D specTexture1;
	float shininess; // TODO: Currently set manually, set in Mesh.h?
};

uniform sampler2D shadowMap;
uniform vec3 viewPos;
uniform PositionalLight positionalLight;
uniform LightAttenuation attenuation;
uniform Material material;

in VS_OUT{
  vec3 Normal;
  vec3 FragPos;
  vec2 TextureCoord;
  vec4 FragPosLightSpace;
} fs_in;

out vec4 FragColor;

vec3 calcPositionalLightColor(PositionalLight positionalLight);
float calcAttenuation(float distanceFromLight);
float calcShadow(vec4 fragPosLightSpace, float normalLightDirDot);

vec3 diffColor;
vec3 specColor;

void main()
{
	diffColor = texture(material.diffTexture1, fs_in.TextureCoord).rgb;
	specColor = texture(material.specTexture1, fs_in.TextureCoord).rgb;

  vec3 finalColor = calcPositionalLightColor(positionalLight);

	// apply gamma correction
	float gamma = 2.2;
	FragColor = vec4(pow(finalColor, vec3(1.0 / gamma)), 1.0);
}

vec3 calcPositionalLightColor(PositionalLight positionalLight) {


	vec3 lightDir = normalize(positionalLight.position - fs_in.FragPos);

	// ambient light
	vec3 ambient = positionalLight.color.ambient * diffColor;

	// diffuse light
	vec3 norm = normalize(fs_in.Normal);
  float normalLightDirDot = dot(norm, lightDir);
	float diffStrength = max(normalLightDirDot, 0.0);
	vec3 diffuse = positionalLight.color.diffuse * diffStrength * diffColor;

	// specular light
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float specStrength = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
	vec3 specular = positionalLight.color.specular * specStrength * specColor;
	
	float distanceFromLight = length(positionalLight.position - fs_in.FragPos);
  float att = 1.0;//calcAttenuation(distanceFromLight); // TODO: Bring back

  float shadowInverse = 1.0 - calcShadow(fs_in.FragPosLightSpace, normalLightDirDot);

	return (ambient + ((diffuse + specular) * shadowInverse)) * att;
}

float calcAttenuation(float distanceFromLight) {
	return (1.0 / (attenuation.constant + (attenuation.linear * distanceFromLight) + (attenuation.quadratic * distanceFromLight * distanceFromLight)));
}

float calcShadow(vec4 fragPosLightSpace, float normalLightDirDot)
{
  // perform perspective divide
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
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
      // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
      float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
      // check whether current frag pos is in shadow
      float bias = max(0.05 * (1.0 - normalLightDirDot), 0.005);
      shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;
    }
  }
  shadow /= 9.0f;

  return shadow;
}