#version 330 core
out vec4 FragColor;

in VS_OUT {
  vec3 Normal;
  vec3 Position;
} fs_in;

uniform vec3 cameraPos;
uniform float refractiveIndex;
uniform samplerCube skybox;

void main()
{
  float refractionRatio = 1.0 / refractiveIndex;
  vec3 I = normalize(fs_in.Position - cameraPos);
  vec3 R = refract(I, fs_in.Normal, refractionRatio);
  FragColor = vec4(texture(skybox, R).rgb, 1.0);
}