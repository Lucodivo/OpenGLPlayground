
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

struct Material {
	sampler2D diffTexture1;
};

uniform Material material;

void main()
{    
    FragColor = texture(material.diffTexture1, TexCoords);
}

