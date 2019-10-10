#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 2) in vec2 aTextureCoord;

out vec2 TextureCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
  gl_Position = projection * view * model * vec4(aPosition, 1.0);
  TextureCoord = aTextureCoord;
}