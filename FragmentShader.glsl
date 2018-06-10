#version 330 core
out vec4 FragColor;

uniform vec3 viewPos;
uniform float sineVal;

struct Light{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;

struct Material {
	sampler2D diffTexture;
	sampler2D specTexture;
	float shininess;
};
uniform Material material;

in vec3 Normal;
in vec3 FragPos;
in vec2 TextureCoord;

void main()
{
	vec3 diffColor;
	vec3 specColor;
	if(sineVal > 0) {
		diffColor = texture(material.diffTexture, TextureCoord).rgb;
		specColor = texture(material.specTexture, TextureCoord).rgb;
	} else {
		diffColor = texture(material.diffTexture, vec2(-TextureCoord.x, TextureCoord.y)).rgb;
		specColor = texture(material.specTexture, vec2(-TextureCoord.x, TextureCoord.y)).rgb;
	}

	// ambient light
	vec3 ambient = light.ambient * diffColor;

	// diffuse light
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * diffColor;

	// specular light
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * specColor);

	vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}