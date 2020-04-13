#version 330 core

out vec4 FragColor;

uniform vec2 viewPortResolution;
uniform vec3 rayOrigin;
uniform float elapsedTime;
uniform mat4 viewRotationMat;

const vec3 sphereColor = vec3(0.0, 0.5, 0.0);
const vec3 planeColor = vec3(0.0, 0.0, 0.5);
const vec3 missColor = vec3(0.5, 0.0, 0.0);

const vec2 center = vec2(0.0, 0.0);
const float maxDist = 80.0;
const float missDist = maxDist + 1.0;

float raySphereIntersectionAlgebraic(vec3 rayOrigin, vec3 rayDir, float radius);
float raySphereIntersectionGeometric(vec3 rayOrigin, vec3 rayDir);
float rayXYAlignedPlaneIntersection(vec3 rayOrigin, vec3 rayDir, float z);
float rayXZAlignedPlaneIntersection(vec3 rayOrigin, vec3 rayDir, float y);
float rayYZAlignedPlaneIntersection(vec3 rayOrigin, vec3 rayDir, float x);

float sint;

void main()
{
  // Move (0,0) from bottom left to center
  vec2 pixelCoord = gl_FragCoord.xy-0.5*viewPortResolution.xy;
  // Scale y value to [-1.0, 1.0], scale x by same factor
  pixelCoord = pixelCoord / viewPortResolution.y;

  sint = sin(elapsedTime);

  vec3 rayDir = vec3(pixelCoord.x, pixelCoord.y, 1.0);
  rayDir = vec3(vec4(rayDir, 0.0) * viewRotationMat);
  rayDir = normalize(rayDir);

  vec3 sphereLocation = vec3(sint, 0.0, 0.0);
  float sphereHit = raySphereIntersectionAlgebraic(rayOrigin - sphereLocation, rayDir, 4.0);
  float xyPlaneHit = rayXYAlignedPlaneIntersection(rayOrigin, rayDir, -6.0);
  if(sphereHit < missDist && sphereHit < xyPlaneHit) {
    FragColor = vec4(sphereColor * ((maxDist - sphereHit) / maxDist), 1.0);
  } else if(xyPlaneHit < missDist) {
    FragColor = vec4(planeColor * ((maxDist - xyPlaneHit) / maxDist), 1.0);
  } else {
    FragColor = vec4(missColor, 1.0);
  }
}

float rayXYAlignedPlaneIntersection(vec3 rayOrigin, vec3 rayDir, float z) {
  float t = (z - rayOrigin.z) / rayDir.z;
  return t > 0 ? t : missDist;
}

float rayXZAlignedPlaneIntersection(vec3 rayOrigin, vec3 rayDir, float y) {
  return 0.0;
}

float rayYZAlignedPlaneIntersection(vec3 rayOrigin, vec3 rayDir, float x) {
  return 0.0;
}

float raySphereIntersectionAlgebraic(vec3 rayOrigin, vec3 rayDir, float radius) {
  float a = dot(rayDir, rayDir);
  float b = 2.0 * dot(rayDir, rayOrigin);
  float c = dot(rayOrigin, rayOrigin) - (radius * radius);
  float discriminant = (b * b) - (4 * a * c);
  if(discriminant < 0) return missDist;

  float t0 = (-b - sqrt(discriminant)) / (2 * a);
  float t1 = (-b + sqrt(discriminant)) / (2 * a);

  float closest = t0 > 0.0 ? t0 : t1;
  return closest > 0.0 ? closest : missDist;
}


float raySphereIntersectionGeometric(vec3 rayOrigin, vec3 rayDir, float radius) {
  vec3 rayToCenter = -rayOrigin;
  return 0.0;
}
