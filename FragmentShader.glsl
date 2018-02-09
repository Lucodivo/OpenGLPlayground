#version 330 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float sineVal;
uniform sampler2D aTexture;
uniform sampler2D bTexture;

in vec3 Normal;
in vec3 FragPos;
in vec2 TextureCoord;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec4 textureColor = mix(texture(aTexture, TextureCoord), texture(bTexture, TextureCoord), sineVal);
	vec3 result = min(ambient + diffuse, 1.0) * textureColor.xyz;
    FragColor = vec4(result, 1.0);
}