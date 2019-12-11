#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out VS_OUT{
  vec3 Normal;
  vec3 Pos;
  vec2 TexCoords;
  vec4 PosLightSpace;
} vs_out;


layout(std140) uniform globalBlockVS{
                      // base alignment			aligned offset
  mat4 projection;    // 64 				        64
  mat4 view;          // 64				          0
};

uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{
  mat3 normalMatrix = transpose(inverse(mat3(model)));
  vs_out.Normal = aNormal;
  vs_out.Pos = vec3(model * vec4(aPos, 1.0));
  vs_out.TexCoords = aTexCoords;
  vs_out.PosLightSpace = lightSpaceMatrix * vec4(vs_out.Pos, 1.0);
  gl_Position = projection * view * model * vec4(aPos, 1.0);
}
