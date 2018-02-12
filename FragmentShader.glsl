#version 330 core
out vec4 FragColor;
  
// uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float sineVal;
uniform sampler2D aTexture;

in vec3 Normal;
in vec3 FragPos;
in vec2 TextureCoord;

void main()
{
	// ambient light
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse light
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular light
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	int shinyness = 32;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shinyness);
	vec3 specular = specularStrength * spec * lightColor;

	vec4 textureColor;
	if(sineVal > 0) {
		textureColor = texture(aTexture, TextureCoord);
	} else {
		textureColor = texture(aTexture, vec2(-TextureCoord.x, TextureCoord.y));
	}
	vec3 result = (ambient + diffuse + specular) * textureColor.rgb;
    FragColor = vec4(result, 1.0);
}