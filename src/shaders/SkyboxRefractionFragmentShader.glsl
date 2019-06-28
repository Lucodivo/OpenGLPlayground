#version 330 core
out vec4 FragColor;

in GS_OUT {
  vec3 normal;
  vec3 position;
} fs_in;

uniform vec3 cameraPos;
uniform float refractiveIndex;
uniform samplerCube skybox;

void main()
{             
	float refractionRatio = 1.0 / refractiveIndex;
    vec3 I = normalize(fs_in.position - cameraPos);
    vec3 R = refract(I, fs_in.normal, refractionRatio);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}