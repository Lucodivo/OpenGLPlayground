#version 330 core
out vec4 FragColor;

in vec3 vertexColor; // input variable from vertex shader
in vec2 textureCoord;

uniform float sineVal; // uniform variable from program

uniform sampler2D aTexture;
uniform sampler2D bTexture;

void main()
{
	FragColor = mix(texture(aTexture, textureCoord), texture(bTexture, textureCoord), sineVal);
	//FragColor = vec4(vertexColor * vec3(sineVal, sineVal, sineVal), 1.0f);
    //FragColor = vec4(sineVal, sineVal, sineVal, 1.0f);
}