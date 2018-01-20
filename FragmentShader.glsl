#version 330 core
out vec4 FragColor;

in vec3 vertexColor; // input variable from vertex shader

uniform float sineVal1; // uniform variable from program
uniform float sineVal2; // uniform variable from program
uniform float sineVal3; // uniform variable from program

void main()
{
	FragColor = vec4(vertexColor * vec3(sineVal1, sineVal2, sineVal3), 1.0f);
    //FragColor = vec4(sineVals, 1.0f);
}