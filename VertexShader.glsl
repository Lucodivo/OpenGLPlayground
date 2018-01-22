#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexture;

out vec3 vertexColor; // output variable to fragment shader
out vec2 textureCoord; // output variable to fragment shader

uniform float sineVal1; // uniform variable from program
uniform float sineVal2; // uniform variable from program
uniform float sineVal3; // uniform variable from program

void main()
{
  gl_Position = vec4(aPosition - vec3(sineVal1 - 0.5, sineVal2 - 0.5, 0), 1.0);
  textureCoord = aTexture;
  //vertexColor = aColor;
  //vertexColor = vec3(abs(aPosition.x), abs(aPosition.y), abs(aPosition.z));
}