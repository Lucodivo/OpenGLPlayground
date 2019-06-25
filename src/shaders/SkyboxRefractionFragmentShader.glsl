#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform float refractiveIndex;
uniform samplerCube skybox;

void main()
{             
	float refractionRatio = 1.0 / refractiveIndex;
    vec3 I = normalize(Position - cameraPos);
    vec3 R = refract(I, normalize(Normal), refractionRatio);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}