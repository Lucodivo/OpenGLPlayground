#version 330 core
out vec4 FragColor;
  
// uniform vec3 objectColor;
uniform vec3 viewPos;
uniform float sineVal;
uniform sampler2D aTexture;

struct Light{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

in vec3 Normal;
in vec3 FragPos;
in vec2 TextureCoord;

void main()
{
	// ambient light
	vec3 ambient = light.ambient * material.ambient;

	// diffuse light
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// specular light
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	int shininess = 32;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	vec4 textureColor;
	if(sineVal > 0) {
		textureColor = texture(aTexture, TextureCoord);
	} else {
		textureColor = texture(aTexture, vec2(-TextureCoord.x, TextureCoord.y));
	}
	vec3 result = (ambient + diffuse + specular) * textureColor.rgb;
    FragColor = vec4(result, 1.0);
}