#version 330 core
out vec4 FragColor;

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
};
struct Material {
	sampler2D diffTexture;
	sampler2D specTexture;
	sampler2D emissionTexture;
	float shininess;
};

uniform vec3 viewPos;
uniform bool animSwitch;
uniform float emissionStrength;
uniform PositionalLight positionalLight;
uniform DirectionalLight directionalLight;
uniform SpotLight spotLight;
uniform Material material;

in vec3 Normal;
in vec3 FragPos;
in vec2 TextureCoord;

vec3 calcPositionalLightColor(vec3 diffColor, vec3 specColor);
vec3 calcDirectionalLightColor(vec3 diffColor, vec3 specColor);
vec3 calcSpotLightColor(vec3 diffColor, vec3 specColor);
vec3 calcLightColor(vec3 lightDir, LightColor lightColor, vec3 diffColor, vec3 specColor);

void main()
{
	vec2 animTextCoord;
	if(animSwitch) {
		animTextCoord = vec2(-TextureCoord.x, TextureCoord.y);
	} else {
		animTextCoord = TextureCoord;
	}

	vec3 diffColor = texture(material.diffTexture, animTextCoord).rgb;
	vec3 specColor = texture(material.specTexture, animTextCoord).rgb;
	vec3 emissionColor = texture(material.emissionTexture, animTextCoord).rgb;

	vec3 rotationalResult = calcPositionalLightColor(diffColor, specColor);

	vec3 directionalResult = calcDirectionalLightColor(diffColor, specColor);

	vec3 spotResult = calcSpotLightColor(diffColor, specColor);

	// emission light
	vec3 emissionResult = emissionColor * emissionStrength;

	vec3 result = rotationalResult + directionalResult + spotResult + emissionResult;
    FragColor = vec4(result, 1.0);
}

vec3 calcPositionalLightColor(vec3 diffColor, vec3 specColor) {
	vec3 lightDir = normalize(positionalLight.position - FragPos);

	float distance = length(positionalLight.position - FragPos);
	float attenuation = 1.0 / (positionalLight.attenuation.constant + 
		positionalLight.attenuation.linear * distance + 
		positionalLight.attenuation.quadratic * (distance * distance));

	return (calcLightColor(lightDir, positionalLight.color, diffColor, specColor) * attenuation);
}

vec3 calcDirectionalLightColor(vec3 diffColor, vec3 specColor) {
	vec3 lightDir = normalize(-directionalLight.direction);
	return calcLightColor(lightDir, directionalLight.color, diffColor, specColor);
}

vec3 calcSpotLightColor(vec3 diffColor, vec3 specColor) {
	vec3 lightDir = normalize(spotLight.position - FragPos);
	float theta = dot(lightDir, normalize(-spotLight.direction));
	float epsilon = spotLight.cutOff - spotLight.outerCutOff;
	float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);

	return (calcLightColor(lightDir, spotLight.color, diffColor, specColor) * intensity);
}

vec3 calcLightColor(vec3 lightDir, LightColor lightColor, vec3 diffColor, vec3 specColor) { 
	// ambient light
	vec3 ambient = lightColor.ambient * diffColor;

	// diffuse light
	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = lightColor.diffuse * diff * diffColor;

	// specular light
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = lightColor.specular * spec * specColor;

	return ambient + diffuse + specular;
}