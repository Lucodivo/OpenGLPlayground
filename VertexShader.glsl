#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexture;

out vec3 vertexColor; // output variable to fragment shader
out vec2 textureCoord; // output variable to fragment shader

uniform float sineVal; // uniform variable from program

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  gl_Position = projection * view * model * vec4(aPosition, 1.0);
  textureCoord = aTexture;
  //vertexColor = aColor;
  //vertexColor = vec3(abs(aPosition.x), abs(aPosition.y), abs(aPosition.z));
}