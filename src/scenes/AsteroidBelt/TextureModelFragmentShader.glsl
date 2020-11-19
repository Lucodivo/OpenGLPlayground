#version 330 core

struct Material
{
  sampler2D diffTexture1;
  sampler2D specTexture1;
};

uniform Material material;

in vec2 TexCoords;

out vec4 FragColor;

void main()
{
  vec3 diffColor = texture(material.diffTexture1, TexCoords).rgb;

  FragColor = vec4(diffColor, 1.0);
}