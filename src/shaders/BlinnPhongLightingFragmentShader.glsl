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

struct Material {
	sampler2D diffTexture1;
	sampler2D specTexture1;
	float shininess; // TODO: Currently set manually, set in Mesh.h?
};

uniform vec3 viewPos;
uniform PositionalLight positionalLight;
uniform Material material;

in vec3 Normal;
in vec3 FragPos;
in vec2 TextureCoord;

out vec4 FragColor;

vec3 calcPositionalLightColor();
float calcAttenuation(LightAttenuation attenuation, float distanceFromLight);

vec3 diffColor;
vec3 specColor;

void main()
{
	diffColor = texture(material.diffTexture1, TextureCoord).rgb;
	specColor = texture(material.specTexture1, TextureCoord).rgb;

    FragColor = vec4(calcPositionalLightColor(), 1.0);
}

vec3 calcPositionalLightColor() {

	vec3 lightDir = normalize(positionalLight.position - FragPos);

	// ambient light
	vec3 ambient = positionalLight.color.ambient * diffColor;

	// diffuse light
	vec3 norm = normalize(Normal);
	float diffStrength = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = positionalLight.color.diffuse * diffStrength * diffColor;

	// specular light
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
    float specStrength = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
	vec3 specular = positionalLight.color.specular * specStrength * specColor;
	
	float distanceFromLight = length(positionalLight.position - FragPos);
	float attenuation = calcAttenuation(positionalLight.attenuation, distanceFromLight);

	return (ambient + diffuse + specular) * attenuation;
}

float calcAttenuation(LightAttenuation attenuation, float distanceFromLight) {
	return (1.0 / (attenuation.constant + (attenuation.linear * distanceFromLight) + (attenuation.quadratic * distanceFromLight * distanceFromLight)));
}