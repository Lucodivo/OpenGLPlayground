#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT {
  vec3 Normal;
  vec3 Position;
} vs_out;

uniform mat4 models[10];
uniform mat4 view;
uniform mat4 projection;

void main()
{
  mat3 normalMat = mat3(transpose(inverse(models[gl_InstanceID])));
  vs_out.Normal = normalize(normalMat * aNormal);
  vs_out.Position = vec3(models[gl_InstanceID] * vec4(aPos, 1.0));
  gl_Position = projection * view * models[gl_InstanceID] * vec4(aPos, 1.0);
}  