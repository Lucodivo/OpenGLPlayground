#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 vertexColor; // output variable to fragment shader

void main()
{
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
  vertexColor = vec4(0.3, 0.0, 0.0, 1.0);
  //vertexColor = vec4(abs(aPos.x), abs(aPos.y), abs(aPos.z), 1.0);
}