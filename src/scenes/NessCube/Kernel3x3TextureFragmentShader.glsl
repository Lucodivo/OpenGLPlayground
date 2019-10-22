#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D tex;
uniform float kernel[9];
uniform float textureWidth;
uniform float textureHeight;

void main()
{
  float sOffset = 1.0 / textureWidth;
  float tOffset = 1.0 / textureHeight;

  float negativeS = TexCoords.s - sOffset;
  float negativeT = TexCoords.t - tOffset;

  float positiveS = TexCoords.s + sOffset;
  float positiveT = TexCoords.t + tOffset;

  if (negativeS < 0.0) {
    negativeS = TexCoords.s;
  }

  if (negativeT < 0.0) {
    negativeT = TexCoords.t;
  }

  if (positiveS > 1.0) {
    positiveS = TexCoords.s;
  }

  if (positiveT > 1.0) {
    positiveT = TexCoords.t;
  }

  vec3 col = vec3(0.0);

  // top row
  col += vec3(texture(tex, vec2(negativeS, positiveT))) * kernel[0];
  col += vec3(texture(tex, vec2(TexCoords.s, positiveT))) * kernel[1];
  col += vec3(texture(tex, vec2(positiveS, positiveT))) * kernel[2];

  // middle row
  col += vec3(texture(tex, vec2(negativeS, TexCoords.t))) * kernel[3];
  col += vec3(texture(tex, vec2(TexCoords.s, TexCoords.t))) * kernel[4];
  col += vec3(texture(tex, vec2(positiveS, TexCoords.t))) * kernel[5];

  // bottom row
  col += vec3(texture(tex, vec2(negativeS, negativeT))) * kernel[6];
  col += vec3(texture(tex, vec2(TexCoords.s, negativeT))) * kernel[7];
  col += vec3(texture(tex, vec2(positiveS, negativeT))) * kernel[8];

  FragColor = vec4(col, 1.0);
}  