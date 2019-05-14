#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTextureCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
out vec2 TextureCoord; 

void main()
{
  gl_Position = projection * view * model * vec4(aPosition, 1.0);
  FragPos = vec3(model * vec4(aPosition, 1.0));
  mat3 normalMat = mat3(transpose(inverse(model)));
  Normal = normalMat * aNormal;
  TextureCoord = aTextureCoord;
}