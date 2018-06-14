#version 330 core
out vec4 FragColor;

uniform vec3 viewPos;
uniform bool animSwitch;
uniform float emissionStrength;

struct LightColor{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PositionalLight{
	vec3 position;
	LightColor color;
};
uniform PositionalLight positionalLight;

struct DirectionalLight{
	vec3 direction;
	LightColor color;
};
uniform DirectionalLight directionalLight;

struct Material {
	sampler2D diffTexture;
	sampler2D specTexture;
	sampler2D emissionTexture;
	float shininess;
};
uniform Material material;

in vec3 Normal;
in vec3 FragPos;
in vec2 TextureCoord;

vec3 calcPositionalLightColor(vec3 diffColor, vec3 specColor);
vec3 calcDirectionalLightColor(vec3 diffColor, vec3 specColor);
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

	// emission light
	vec3 emissionResult = emissionColor * emissionStrength;

	vec3 result = rotationalResult + directionalResult + emissionResult;
    FragColor = vec4(result, 1.0);
}

vec3 calcPositionalLightColor(vec3 diffColor, vec3 specColor) {
	vec3 lightDir = normalize(positionalLight.position - FragPos);
	return calcLightColor(lightDir, positionalLight.color, diffColor, specColor);
}

vec3 calcDirectionalLightColor(vec3 diffColor, vec3 specColor) {
	vec3 lightDir = normalize(-directionalLight.direction);
	return calcLightColor(lightDir, directionalLight.color, diffColor, specColor);
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
	vec3 specular = lightColor.specular * (spec * specColor);

	vec3 result = ambient + diffuse + specular;

	return result; 
}