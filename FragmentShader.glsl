#version 330 core
out vec4 FragColor;

in vec3 vertexColor; // input variable from vertex shader
in vec2 textureCoord;

uniform float sineVal1; // uniform variable from program
uniform float sineVal2; // uniform variable from program
uniform float sineVal3; // uniform variable from program

uniform sampler2D aTexture;
uniform sampler2D bTexture;

void main()
{
	FragColor = mix(texture(aTexture, textureCoord), texture(bTexture, textureCoord), 0.5);
	//FragColor = vec4(vertexColor * vec3(sineVal1, sineVal2, sineVal3), 1.0f);
    //FragColor = vec4(sineVals, 1.0f);
}