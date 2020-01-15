#version 330 core

out vec4 FragColor;

uniform vec2 viewPortResolution;
uniform float elapsedTime;

void main() {
  // reconfigure the cordinates to have (0,0) in the center with equal sized units for width/height
  // NOTE: The final division by y is an arbitrary choice. x can be used instead.
  vec2 uv = (gl_FragCoord.xy-0.5*viewPortResolution.xy)/viewPortResolution.y;

  vec3 col = vec3(0.0f);

  uv = abs(uv);

  float c = dot(uv, vec2(1.0, 1.0));

  col += step(c, 0.2);

  FragColor = vec4(col, 1.0);
}
