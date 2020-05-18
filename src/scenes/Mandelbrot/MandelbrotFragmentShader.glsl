#version 330 core

out vec4 FragColor;

#define MAX_ITERATIONS 200.0

uniform vec2 viewPortResolution;
uniform vec2 centerOffset;
uniform float zoom;
uniform vec3 colorFavor;

vec2 complexMul(vec2 a, vec2 b);
vec2 complexSquare(vec2 a, vec2 b);
vec2 mandelbrotIter(vec2 coord, vec2 constant);

void main() {
  // Move (0,0) from bottom left to center
  vec2 pixelCoord = gl_FragCoord.xy-0.5*viewPortResolution.xy;

  // Scale y value to [-1.0, 1.0], scale x by same factor
  pixelCoord = pixelCoord / (viewPortResolution.y * zoom);

  pixelCoord += centerOffset / (viewPortResolution.y);

  float iterations = 0.0;
  vec2 constant = pixelCoord;
  pixelCoord = vec2(0.0, 0.0);
  while(iterations < MAX_ITERATIONS && length(pixelCoord) < 2.0) {
    pixelCoord = mandelbrotIter(pixelCoord, constant);
    iterations += 1.0;
  }

  vec3 color = vec3(1.0, 1.0, 1.0);
  float iterFraction = (iterations / MAX_ITERATIONS);
  vec3 colorSub = (1.0 / colorFavor) * iterFraction;
  color -= colorSub;

  FragColor = vec4(color, 0.0);
}

vec2 complexMul(vec2 a, vec2 b) {
  return vec2((a.x * b.x) - (a.y * b.y), (a.x * b.y) + (a.y * b.x));
}

vec2 complexSquare(vec2 a) {
  return vec2((a.x * a.x) - (a.y * a.y), 2.0 * (a.x * a.y));
}

vec2 mandelbrotIter(vec2 coord, vec2 constant) {
  return complexSquare(coord) + constant;
}