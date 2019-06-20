#version 330 core

uniform sampler2D diffTexture;
uniform float texWidth;
uniform float texHeight;

in vec3 Normal;
in vec3 FragPos;
in vec2 TextureCoord;

out vec4 FragColor;

void main()
{
	float texXStart = ((texWidth - texHeight) / 2.0) / texWidth;
	float heightOverWidth = texHeight / texWidth;

	vec2 croppedTexCoord = vec2(texXStart + (TextureCoord.x * heightOverWidth), TextureCoord.y);

	vec4 diffColor = texture(diffTexture, croppedTexCoord);

	FragColor = diffColor;
}