#version 330 core
out vec4 FragColor;

in vec4 vertexColor; // input variable from vertex shader

uniform vec4 programColor;

void main()
{
	FragColor = programColor;
    //FragColor = vertexColor;
}