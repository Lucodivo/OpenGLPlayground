#version 330 core

struct Material
{
  sampler2D diffTexture1;
  sampler2D specTexture1;
  float shininess;// TODO: Currently set manually, set in Mesh.h?
};

uniform Material material;

in vec2 TextureCoord;

out vec4 FragColor;

void main()
{
  vec3 diffColor = texture(material.diffTexture1, TextureCoord).rgb;

  FragColor = vec4(diffColor, 1.0);
}