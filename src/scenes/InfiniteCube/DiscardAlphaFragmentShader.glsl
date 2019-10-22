#version 330 core

uniform sampler2D diffTexture;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

void main()
{
  vec4 diffColor = texture(diffTexture, TexCoords);
  if (diffColor.a < 0.1){
    discard;
  }
  FragColor = diffColor;
}