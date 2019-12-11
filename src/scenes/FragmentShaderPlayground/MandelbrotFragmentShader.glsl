#version 330 core

out vec4 FragColor;

#define MAX_ITERATIONS 200.0

uniform vec2 viewPortResolution;
uniform float elapsedTime;
uniform vec2 centerOffset;
uniform float zoom;

void main() {
  // Move (0,0) from bottom left to center
  vec2 uv = gl_FragCoord.xy-0.5*viewPortResolution.xy;

  // Scale y value to [-1.0, 1.0], scale x by same factor
  uv = uv / (viewPortResolution.y * zoom);

  uv += centerOffset / (viewPortResolution.y);

  int iterations = 0;
  vec2 c = uv;
  uv = vec2(0.0);
  while(iterations < MAX_ITERATIONS && length(uv) < 2.0) {
    uv = vec2((uv.x * uv.x) - (uv.y * uv.y), 2 * (uv.x * uv.y));
    uv += c;
    iterations += 1;
  }

  vec3 color = vec3(1.0);
  float iterFraction = (iterations / MAX_ITERATIONS);

  if(iterations > 150) {
    color -= iterFraction;
  } else if(iterations > 80) {
    color -= vec3(iterFraction, iterFraction, 0.0);
  } else {
    color -= vec3(iterFraction, 0.0, 0.0);
  }

  FragColor = vec4(color, 0.0);
}
