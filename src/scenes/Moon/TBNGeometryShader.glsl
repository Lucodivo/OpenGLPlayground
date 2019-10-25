#version 330
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT{
  vec3 Normal;
  vec3 Pos;
  vec2 TexCoords;
  vec4 PosLightSpace;
} vs_in[];

out VS_OUT {
  vec2 TexCoords;
  vec4 PosLightSpace;
  vec3 TangentPos;
  vec3 TangentLightDir;
  vec3 TangentViewPos;
} gs_out;

uniform mat4 model;
uniform vec3 directionalLightDir; // normalized direction from origin to light
uniform vec3 viewPos;

void main() {
  mat3 normalMatrix = transpose(inverse(mat3(model)));
  vec3 normal = normalize(normalMatrix * vs_in[0].Normal);

  vec3 posDiff1 = vs_in[1].Pos - vs_in[0].Pos;
  vec3 posDiff2 = vs_in[2].Pos - vs_in[0].Pos;
  vec2 texDiff1 = vs_in[1].TexCoords - vs_in[0].TexCoords;
  vec2 texDiff2 = vs_in[2].TexCoords - vs_in[0].TexCoords;

  float f = 1.0f / (texDiff1.x * texDiff2.y - texDiff2.x * texDiff1.y);

  vec3 tangent;
  tangent.x = f * (texDiff2.y * posDiff1.x - texDiff1.y * posDiff2.x);
  tangent.y = f * (texDiff2.y * posDiff1.y - texDiff1.y * posDiff2.y);
  tangent.z = f * (texDiff2.y * posDiff1.z - texDiff1.y * posDiff2.z);
  tangent = normalize(normalMatrix * tangent);

  // re-orthogonalize tangent with respect to normal
  tangent = normalize(tangent - dot(tangent, normal) * normal);
  // then retrieve perpendicular vector B with the cross product of T and N
  vec3 bitangent = cross(normal, tangent);

  mat3 inverseTBN = transpose(mat3(tangent, bitangent, normal));
  gs_out.TangentLightDir = inverseTBN * directionalLightDir;
  gs_out.TangentViewPos = inverseTBN * viewPos;
  gl_Position = gl_in[0].gl_Position;
  gs_out.PosLightSpace = vs_in[0].PosLightSpace;
  gs_out.TangentPos = inverseTBN * vs_in[0].Pos;
  gs_out.TexCoords = vs_in[0].TexCoords;
  EmitVertex();
  gl_Position = gl_in[1].gl_Position;
  gs_out.PosLightSpace = vs_in[1].PosLightSpace;
  gs_out.TangentPos = inverseTBN * vs_in[1].Pos;
  gs_out.TexCoords = vs_in[1].TexCoords;
  EmitVertex();
  gl_Position = gl_in[2].gl_Position;
  gs_out.PosLightSpace = vs_in[2].PosLightSpace;
  gs_out.TangentPos = inverseTBN * vs_in[2].Pos;
  gs_out.TexCoords = vs_in[2].TexCoords;
  EmitVertex();
  EndPrimitive();
}