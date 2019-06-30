#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT {
 vec3 Normal;
 vec3 Position;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    mat3 normalMat = mat3(transpose(inverse(model)));
    vs_out.Normal = normalize(vec3(projection * view * vec4(normalMat * aNormal, 0.0)));
    vs_out.Position = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}  