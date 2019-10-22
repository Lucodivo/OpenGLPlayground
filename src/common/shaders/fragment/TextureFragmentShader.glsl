#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D tex;

void main()
{
  vec4 texVals = texture(tex, TexCoords);
  if(texVals.a < 0.05) discard;
  FragColor = texVals;
}