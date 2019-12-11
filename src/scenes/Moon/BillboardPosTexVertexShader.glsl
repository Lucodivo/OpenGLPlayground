#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 2) in vec2 aTextureCoord;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

mat4 billboard(mat4 modelMatrix);
vec3 extractScaling(mat3 rotationAndScaling);

void main()
{
  mat4 clipSpaceMatrix = projection * billboard(view *  model);
  gl_Position = clipSpaceMatrix * vec4(aPosition, 1.0);
  TexCoords = aTextureCoord;
}

mat4 billboard(mat4 modelMat) {
  mat4 billboardMat = mat4(0.0);
  vec3 scale = extractScaling(mat3(modelMat));
  billboardMat[3] = modelMat[3];
  billboardMat[0][0] = scale.x;
  billboardMat[1][1] = scale.y;
  billboardMat[2][2] = scale.z;
  return billboardMat;
}

vec3 extractScaling(mat3 rotationAndScaling) {
  float x = length(rotationAndScaling[0]);
  float y = length(rotationAndScaling[1]);
  float z = length(rotationAndScaling[2]);
  return vec3(x, y, z);
}