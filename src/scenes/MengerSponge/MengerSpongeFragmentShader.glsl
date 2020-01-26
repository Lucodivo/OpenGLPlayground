#version 330 core

out vec4 FragColor;

#define MAX_STEPS 150
#define MISS_DIST 100.0
#define HIT_DIST 0.01

float distPosToScene(vec3 pos);
float distanceRayToScene(vec3 rayOrigin, vec3 rayDir);
float distToBox(vec3 rayPos, vec3 dimen);
float distToCross(vec3 rayPos, vec3 dimen);
float getLight(vec3 surfacePos);
float distToRect(vec2 rayPos, vec2 dimen);
vec3 getNormal(vec3 surfacePos);

uniform vec2 viewPortResolution;
uniform vec3 rayOrigin;
uniform float elapsedTime;
uniform mat4 viewRotationMat;
uniform vec3 lightPos;
uniform vec3 lightColor;

float sinElapsedTime;
float cosElapsedTime;

void main()
{
  sinElapsedTime = sin(elapsedTime / 4.0);
  cosElapsedTime = cos(elapsedTime / 4.0);

  // Move (0,0) from bottom left to center
  vec2 pixelCoord = gl_FragCoord.xy-0.5*viewPortResolution.xy;
  // Scale y value to [-1.0, 1.0], scale x by same factor
  pixelCoord = pixelCoord / viewPortResolution.y;

  vec3 rayDir = vec3(pixelCoord.x, pixelCoord.y, 1.0);
  rayDir = vec3(vec4(rayDir, 0.0) * viewRotationMat);
  rayDir = normalize(rayDir);

  float dist = distanceRayToScene(rayOrigin, rayDir);

  if(dist > 0.0) { // hit
    vec3 surfacePos = rayOrigin + (rayDir * dist);
    float diffuse = getLight(surfacePos);
    vec3 col = vec3(diffuse * lightColor);

    FragColor = vec4(col, 1.0);
  } else { // miss
    vec3 missColor = vec3(0.2, 0.2, 0.2);
    FragColor = vec4(missColor, 1.0);
  }
}

vec3 getNormal(vec3 surfacePos) {
  vec2 epsilon = vec2(0.001, 0);
  float dist = distPosToScene(surfacePos);
  float xDist = distPosToScene(surfacePos + epsilon.xyy);
  float yDist = distPosToScene(surfacePos + epsilon.yxy);
  float zDist = distPosToScene(surfacePos + epsilon.yyx);
  vec3 normal = (vec3(xDist, yDist, zDist) - dist) / epsilon.x;
  return normalize(normal);
}


float getLight(vec3 surfacePos) {

  vec3 lightDir = normalize(lightPos - surfacePos);
  vec3 normal = getNormal(surfacePos);

  float diff = clamp(dot(normal, lightDir), 0.3, 1.0);

  // calculate for shadows
//  float dist = distanceRayToScene(surfacePos + (normal * HIT_DIST * 2.0), lightDir);
//  if(dist < length(lightPos - surfacePos)) diff *= 0.1;

  return diff;
}

// NOTE: ray dir is assumed to be normalized
float distanceRayToScene(vec3 rayOrigin, vec3 rayDir) {

  float dist = 0.0;

  for(int i = 0; i < MAX_STEPS; i++) {
    vec3 pos = rayOrigin + (dist * rayDir);
    float posToScene = distPosToScene(pos);
    dist += posToScene;
    if(abs(posToScene) < HIT_DIST) return dist; // absolute value for posToScene incase the ray makes its way inside an object
    if(posToScene > MISS_DIST) break;
  }

  return -1.0f;
}

//const float boxSize = 3.0f;
//const float crossSize = boxSize / 3.0;
float distPosToScene(vec3 rayPos) {
  float boxSize = 3.0f + sinElapsedTime;
  float crossSize = boxSize / 3.0;
  float distBox = distToBox(rayPos, vec3(boxSize));
  float dist = distBox;

  float s = boxSize;
  for(int m = 0; m < 3; ++m) {
    vec3 a = mod(rayPos * s, 2.0 * boxSize) - boxSize;
    s *= 3.0;
    vec3 r = boxSize - 3.0*abs(a);

    float c = distToCross(r, vec3(boxSize))/s;
    dist = max(dist, -c);
  }

  return dist;
}

float distToBox(vec3 rayPos, vec3 dimen) {
  vec3 rayToCorner = abs(rayPos) - dimen;
  float maxDelta = min(max(rayToCorner.x, max(rayToCorner.y, rayToCorner.z)), 0.0);
  return length(max(rayToCorner, 0.0)) + maxDelta;
}

float distToRect(vec2 rayPos, vec2 dimen) {
  vec2 rayToCorner = abs(rayPos) - dimen;
  float maxDelta = min(max(rayToCorner.x, rayToCorner.y), 0.0);
  return length(max(rayToCorner, 0.0)) + maxDelta;
}

float distToCross(vec3 rayPos, vec3 dimen) {
  float da = distToRect(rayPos.xy, dimen.xy);
  float db = distToRect(rayPos.xz, dimen.xz);
  float dc = distToRect(rayPos.yz, dimen.yz);
  return min(da,min(db,dc));
}