#version 330 core
out vec4 FragColor;

in vec3 vertexColor; // input variable from vertex shader

uniform vec3 sineVals; // uniform variable from program

void main()
{
	FragColor = vec4(vertexColor * sineVals, 1.0f);
    //FragColor = vec4(sineVals, 1.0f);
}