#version 330
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT{
  vec3 Normal;
  vec3 Pos;
  vec2 TextureCoord;
  vec4 PosLightSpace;
} vs_in[];

out VS_OUT {
  vec3 Pos;
  vec2 TextureCoord;
  vec4 PosLightSpace;
  mat3 TBN;
} gs_out;

uniform mat4 model;

void main() {
  mat3 normalMatrix = transpose(inverse(mat3(model)));
  vec3 normal = normalize(normalMatrix * vs_in[0].Normal);

  vec3 posDiff1 = vs_in[1].Pos - vs_in[0].Pos;
  vec3 posDiff2 = vs_in[2].Pos - vs_in[0].Pos;
  vec2 texDiff1 = vs_in[1].TextureCoord - vs_in[0].TextureCoord;
  vec2 texDiff2 = vs_in[2].TextureCoord - vs_in[0].TextureCoord;

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

  gs_out.TBN = mat3(tangent, bitangent, normal);
  gl_Position = gl_in[0].gl_Position;
  gs_out.PosLightSpace = vs_in[0].PosLightSpace;
  gs_out.Pos = vs_in[0].Pos;
  gs_out.TextureCoord = vs_in[0].TextureCoord;
  EmitVertex();
  gl_Position = gl_in[1].gl_Position;
  gs_out.PosLightSpace = vs_in[1].PosLightSpace;
  gs_out.Pos = vs_in[1].Pos;
  gs_out.TextureCoord = vs_in[1].TextureCoord;
  EmitVertex();
  gl_Position = gl_in[2].gl_Position;
  gs_out.PosLightSpace = vs_in[2].PosLightSpace;
  gs_out.Pos = vs_in[2].Pos;
  gs_out.TextureCoord = vs_in[2].TextureCoord;
  EmitVertex();
  EndPrimitive();
}