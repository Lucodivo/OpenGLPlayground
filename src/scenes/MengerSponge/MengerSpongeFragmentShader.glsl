#version 330 core

out vec4 FragColor;

#define MAX_STEPS 120
#define MISS_DIST 60.0
#define HIT_DIST 0.01

float distPosToScene(vec3 pos);
vec2 distanceRayToScene(vec3 rayOrigin, vec3 rayDir);
float sdBox(vec3 rayPos, vec3 dimen);
float sdCross(vec3 rayPos, vec3 dimen);
float getLight(vec3 surfacePos);
float sdRect(vec2 rayPos, vec2 dimen);
vec3 getNormal(vec3 surfacePos);

uniform vec2 viewPortResolution;
uniform vec3 rayOrigin;
uniform float elapsedTime;
uniform mat4 viewRotationMat;
uniform vec3 lightPos;
uniform vec3 lightColor;

void main()
{
  // Move (0,0) from bottom left to center
  vec2 pixelCoord = gl_FragCoord.xy-0.5*viewPortResolution.xy;
  // Scale y value to [-1.0, 1.0], scale x by same factor
  pixelCoord = pixelCoord / viewPortResolution.y;

  vec3 rayDir = vec3(pixelCoord.x, pixelCoord.y, 1.0);
  rayDir = vec3(vec4(rayDir, 0.0) * viewRotationMat);
  rayDir = normalize(rayDir);

  vec2 dist = distanceRayToScene(rayOrigin, rayDir);

  if(dist.x > 0.0) { // hit
//    vec3 surfacePos = rayOrigin + (rayDir * dist.x);
//    float diffuse = getLight(surfacePos);
//    vec3 col = vec3(diffuse * lightColor);
    vec3 col = vec3(1.0 - (dist.y/MAX_STEPS));

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

// returns vec2(dist, iterations)
// NOTE: ray dir is assumed to be normalized
vec2 distanceRayToScene(vec3 rayOrigin, vec3 rayDir) {

  float dist = 0.0;

  for(int i = 0; i < MAX_STEPS; i++) {
    vec3 pos = rayOrigin + (dist * rayDir);
    float posToScene = distPosToScene(pos);
    dist += posToScene;
    if(abs(posToScene) < HIT_DIST) return vec2(dist, i); // absolute value for posToScene incase the ray makes its way inside an object
    if(posToScene > MISS_DIST) break;
  }

  return vec2(-1.0f, MAX_STEPS);
}

const float halfBoxDimen = 10.0;
const float mengerDivision = 3.0;
float distPosToScene(vec3 rayPos) {
  float dist = sdBox(rayPos, vec3(halfBoxDimen));
  if(dist > HIT_DIST) return dist; // Use the full box as a bounding volume

  float scale = 1.0;
  for( int iteration = 0; iteration < 4; iteration++ )
  {
    // multiplying by scale determines how many crosses in a single row/col will intersect the cube
    // modulo of 2 * halfBoxDimen creates "stacked" cube worlds the size of the full box
    // subtracting halfBoxDimen puts the box at the origin
    // although modulo returns positive numbers, subtracting the halfBoxDimen requires us to get the absolute value
    vec3 a = abs(mod(rayPos * scale, 2.0 * halfBoxDimen) - halfBoxDimen);
    scale *= mengerDivision;
    vec3 r = abs(halfBoxDimen - (mengerDivision * a));
    // division by scale undistorts space
    float c = sdCross(r, vec3(halfBoxDimen)) / scale;

    dist = max(dist, c);
  }

  return dist;
}

float sdBox(vec3 rayPos, vec3 dimen) {
  vec3 rayToCorner = abs(rayPos) - dimen;
  float maxDelta = min(max(rayToCorner.x, max(rayToCorner.y, rayToCorner.z)), 0.0);
  return length(max(rayToCorner, 0.0)) + maxDelta;
}

float sdRect(vec2 rayPos, vec2 dimen) {
  vec2 rayToCorner = abs(rayPos) - dimen;
  float maxDelta = min(max(rayToCorner.x, rayToCorner.y), 0.0);
  return length(max(rayToCorner, 0.0)) + maxDelta;
}

float sdCross(vec3 rayPos, vec3 dimen) {
  float da = sdRect(rayPos.xy, dimen.xy);
  float db = sdRect(rayPos.xz, dimen.xz);
  float dc = sdRect(rayPos.yz, dimen.yz);
  return min(da,min(db,dc));
}