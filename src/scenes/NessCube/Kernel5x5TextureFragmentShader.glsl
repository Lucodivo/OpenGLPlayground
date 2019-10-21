#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float kernel[25];
uniform float textureWidth;
uniform float textureHeight;

void main()
{
  float sOffset = 1.0 / textureWidth;
  float tOffset = 1.0 / textureHeight;

  float sMinusOne = TexCoords.s - sOffset;
  float tMinusOne = TexCoords.t - tOffset;

  float sMinusTwo = TexCoords.s - (2 * sOffset);
  float tMinusTwo = TexCoords.t - (2 * tOffset);

  float sPlusOne = TexCoords.s + sOffset;
  float tPlusOne = TexCoords.t + tOffset;

  float sPlusTwo = TexCoords.s + (2.0f * sOffset);
  float tPlusTwo = TexCoords.t + (2.0f * tOffset);

  if (sMinusOne < 0.0) {
    sMinusOne = TexCoords.s;
  }

  if (tMinusOne < 0.0) {
    tMinusOne = TexCoords.t;
  }

  if (sMinusTwo < 0.0) {
    sMinusTwo = TexCoords.s;
  }

  if (tMinusTwo < 0.0) {
    tMinusTwo = TexCoords.t;
  }

  if (sPlusOne > 1.0) {
    sPlusOne = TexCoords.s;
  }

  if (tPlusOne > 1.0) {
    tPlusOne = TexCoords.t;
  }

  if (sPlusTwo > 1.0) {
    sPlusTwo = TexCoords.s;
  }

  if (tPlusTwo > 1.0) {
    tPlusTwo = TexCoords.t;
  }

  vec3 col = vec3(0.0);

  // top row
  col += vec3(texture(screenTexture, vec2(sMinusTwo, tPlusTwo))) * kernel[0];
  col += vec3(texture(screenTexture, vec2(sMinusOne, tPlusTwo))) * kernel[1];
  col += vec3(texture(screenTexture, vec2(TexCoords.s, tPlusTwo))) * kernel[2];
  col += vec3(texture(screenTexture, vec2(sPlusOne, tPlusTwo))) * kernel[3];
  col += vec3(texture(screenTexture, vec2(sPlusTwo, tPlusTwo))) * kernel[4];

  // second row
  col += vec3(texture(screenTexture, vec2(sMinusTwo, tPlusOne))) * kernel[5];
  col += vec3(texture(screenTexture, vec2(sMinusOne, tPlusOne))) * kernel[6];
  col += vec3(texture(screenTexture, vec2(TexCoords.s, tPlusOne))) * kernel[7];
  col += vec3(texture(screenTexture, vec2(sPlusOne, tPlusOne))) * kernel[8];
  col += vec3(texture(screenTexture, vec2(sPlusTwo, tPlusOne))) * kernel[9];

  // middle row
  col += vec3(texture(screenTexture, vec2(sMinusTwo, TexCoords.t))) * kernel[10];
  col += vec3(texture(screenTexture, vec2(sMinusOne, TexCoords.t))) * kernel[11];
  col += vec3(texture(screenTexture, vec2(TexCoords.s, TexCoords.t))) * kernel[12];
  col += vec3(texture(screenTexture, vec2(sPlusOne, TexCoords.t))) * kernel[13];
  col += vec3(texture(screenTexture, vec2(sPlusTwo, TexCoords.t))) * kernel[14];

  // fourth row
  col += vec3(texture(screenTexture, vec2(sMinusTwo, tMinusOne))) * kernel[15];
  col += vec3(texture(screenTexture, vec2(sMinusOne, tMinusOne))) * kernel[16];
  col += vec3(texture(screenTexture, vec2(TexCoords.s, tMinusOne))) * kernel[17];
  col += vec3(texture(screenTexture, vec2(sPlusOne, tMinusOne))) * kernel[18];
  col += vec3(texture(screenTexture, vec2(sPlusTwo, tMinusOne))) * kernel[19];

  // bottom row
  col += vec3(texture(screenTexture, vec2(sMinusTwo, tMinusTwo))) * kernel[20];
  col += vec3(texture(screenTexture, vec2(sMinusOne, tMinusTwo))) * kernel[21];
  col += vec3(texture(screenTexture, vec2(TexCoords.s, tMinusTwo))) * kernel[22];
  col += vec3(texture(screenTexture, vec2(sPlusOne, tMinusTwo))) * kernel[23];
  col += vec3(texture(screenTexture, vec2(sPlusTwo, tMinusTwo))) * kernel[24];

  FragColor = vec4(col, 1.0);
}  