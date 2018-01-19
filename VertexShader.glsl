#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor; // output variable to fragment shader

uniform vec4 sineVals; // uniform variable from program

void main()
{
  gl_Position = vec4(aPos, 1.0);
  vertexColor = aColor;
  //vertexColor = vec3(abs(aPos.x), abs(aPos.y), abs(aPos.z));
}