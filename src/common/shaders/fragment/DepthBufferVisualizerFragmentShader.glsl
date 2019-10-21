#version 330 core
out vec4 FragColor;

vec4 defaultDepth();
vec4 linearDepth();

void main()
{
  FragColor = linearDepth();
}

// the default NON-LINEAR depth, after the projection matrix was applied
vec4 defaultDepth() {
  return vec4(vec3(gl_FragCoord.z), 1.0);
}

// LINEAR depth
vec4 linearDepth() {
  float near = 0.1;
  float far  = 100.0;
  float z = gl_FragCoord.z * 2.0 - 1.0;// transform to NDC
  float depth = (2.0 * near * far) / (far + near - z * (far - near)) / far;
  return vec4(vec3(depth), 1.0);
}