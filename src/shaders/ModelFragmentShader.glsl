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
	LightAttenuation attenuation;
};
struct DirectionalLight{
	vec3 direction;
	LightColor color;
};
struct SpotLight{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;
	LightColor color;
	LightAttenuation attenuation;
};

struct Material {
	sampler2D diffTexture1;
	sampler2D specTexture1;
	float shininess; // TODO: Currently set manually, set in Mesh.h?
};

layout (std140) uniform globalBlockFS {
										// base alignment	// aligned offset
	DirectionalLight directionalLight;	// 16				// 0				// vec3 direction
										// 16				// 16				// vec3 LightColor.ambient
										// 16				// 32				// vec3 LightColor.diffuse
										// 16				// 48				// vec3 LightColor.specular
										
};

uniform vec3 viewPos;
uniform float emissionStength;
uniform PositionalLight positionalLight;
uniform SpotLight spotLight;
uniform Material material;

in vec3 Normal;
in vec3 FragPos;
in vec2 TextureCoord;

out vec4 FragColor;

vec3 calcPositionalLightColor();
vec3 calcDirectionalLightColor();
vec3 calcSpotLightColor();
vec3 calcLightColor(vec3 lightDir, LightColor lightColor);
float calcAttenuation(LightAttenuation attenuation, float distance);

vec3 diffColor;
vec3 specColor;

void main()
{
	diffColor = texture(material.diffTexture1, TextureCoord).rgb;
	specColor = texture(material.specTexture1, TextureCoord).rgb;

	vec3 rotationalResult = calcPositionalLightColor();
	vec3 directionalResult = calcDirectionalLightColor();
	vec3 spotResult = calcSpotLightColor();

	vec3 result = rotationalResult + directionalResult + spotResult;
    FragColor = vec4(result, 1.0);
}

vec3 calcPositionalLightColor() {
	vec3 lightDir = normalize(positionalLight.position - FragPos);

	float distance = length(positionalLight.position - FragPos);
	float attenuation = calcAttenuation(positionalLight.attenuation, distance);

	return (calcLightColor(lightDir, positionalLight.color) * attenuation);
}

vec3 calcDirectionalLightColor() {
	vec3 lightDir = normalize(-directionalLight.direction);
	return calcLightColor(lightDir, directionalLight.color);
}

vec3 calcSpotLightColor() {
	vec3 lightDir = normalize(spotLight.position - FragPos);
	float theta = dot(lightDir, normalize(-spotLight.direction));
	float epsilon = spotLight.cutOff - spotLight.outerCutOff;
	float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);
	float distance = length(spotLight.position - FragPos);
	float attenuation = calcAttenuation(spotLight.attenuation, distance);

	return (calcLightColor(lightDir, spotLight.color) * intensity * attenuation);
}

vec3 calcLightColor(vec3 lightDir, LightColor lightColor) { 
	// ambient light
	vec3 ambient = lightColor.ambient * diffColor;

	// diffuse light
	vec3 norm = normalize(Normal);
	float diffStrength = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = lightColor.diffuse * diffStrength * diffColor;

	// specular light
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = lightColor.specular * spec * specColor;

	return ambient + diffuse + specular;
}

float calcAttenuation(LightAttenuation attenuation, float distance) {
	return (1.0 / (attenuation.constant + (attenuation.linear * distance) + (attenuation.quadratic * distance * distance)));
}