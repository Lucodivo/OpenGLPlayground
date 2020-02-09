#version 330 core

out vec4 FragColor;

#define MAX_STEPS 120.0
#define MISS_DIST 60.0
#define HIT_DIST 0.01

float distPosToScene(vec3 pos);
vec2 distanceRayToScene(vec3 rayOrigin, vec3 rayDir);
float sdBox(vec3 rayPos, vec3 dimen);
float sdCross(vec3 rayPos, vec3 dimen);
float sdRect(vec2 rayPos, vec2 dimen);

float crush(vec3 rayPos, bool boxed);
float multistagePrison(vec3 rayPos, bool boxed);

uniform vec2 viewPortResolution;
uniform vec3 rayOrigin;
uniform float elapsedTime;
uniform mat4 viewRotationMat;

const float boxDimen = 20.0;
const float halfBoxDimen = boxDimen / 2.0;

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

  vec2 dist = distanceRayToScene(rayOrigin, rayDir);

  if(dist.x > 0.0) { // hit
    vec3 col = vec3(1.0 - (dist.y/MAX_STEPS));

    FragColor = vec4(col, 1.0);
  } else { // miss
    vec3 missColor = vec3(0.2, 0.2, 0.2);
    FragColor = vec4(missColor, 1.0);
  }
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

float distPosToScene(vec3 rayPos) {
  float boundingBox = sdBox(rayPos, vec3(boxDimen));
  if(boundingBox > HIT_DIST) return boundingBox; // Use the full box as a bounding volume
  float dist = sdBox(rayPos, vec3(halfBoxDimen));

  float scale = 1.0;
  for( int iteration = 0; iteration < 1; iteration++ )
  {
    // turn space into stacked box vectors that range from [0, boxDimen) for all axis
    // multiply ray by scale to create more boxes within [-boxDimen, boxDimen] world space
    // We use the scale before it gets multiplied as a convenience, we usually want n cross 1/3n the size of the whole box
    // ex:  iter 1: 1 cross, 1/3rd the width of the box
    //      iter 2: 3 crosses, 1/9th the width of the box
    vec3 ray = mod(rayPos * scale, boxDimen);
    // subtract half box dimension to create ranges from [-halfBoxDimen, halfBoxDimen)
    // this makes the center of each box have an origin coordinate of <0.0, 0.0, 0.0>
    // and will allow us to draw crosses in the center of each box, along the axes
    ray -= halfBoxDimen;
    // fold all quadrants into the positive quadrant for all three axes, back to [0,halfBoxDimen)
    ray = abs(ray);

    // multiply ray by 3.0 so value lies between [0,3 * halfBoxDimen) for all three axes
    ray = 3.0 * ray;
    // subtract ray from halfBoxDimen so values lie between [-boxDimen, halfBoxDimen) for all axes
    ray = halfBoxDimen - ray;
    //ray = abs(ray);

    // get distance to cross from ray
    float c = sdCross(ray, vec3(halfBoxDimen));
    // we must divide by same number that we have used to multiply the ray to undistort space
    scale *= 3.0;
    c /= scale;

    dist = min(dist, c);
  }

  return dist;
}

float multistagePrison(vec3 rayPos, bool boxed) {
  float dist;
  if(boxed) {
    dist = sdBox(rayPos, vec3(halfBoxDimen));
    if(dist > HIT_DIST) return dist; // Use the full box as a bounding volume
  }

  float scale = 1.0;
  if(sint > 0.5) {
    scale = 1.0;
  } else if(sint < -0.5) {
    scale = 9.0;
  } else {
    scale = 3.0;
  }
  for( int iteration = 0; iteration < 1; iteration++ )
  {
    vec3 ray = mod(rayPos * scale, boxDimen);
    ray -= halfBoxDimen;
    ray = abs(ray);

    ray = 3.0 * ray;

    float c = sdCross(ray, vec3(halfBoxDimen));
    scale *= 3.0;
    c /= scale;

    dist = c;
  }

  return dist;
}

float crushBox(vec3 rayPos, bool boxed) {
  float dist;
  if(boxed) {
    dist = sdBox(rayPos, vec3(halfBoxDimen));
    if(dist > HIT_DIST) return dist; // Use the full box as a bounding volume
  }

  float scale = 1.0;
  for( int iteration = 0; iteration < 1; iteration++ )
  {
    vec3 ray = mod(rayPos * scale, boxDimen);
    ray -= halfBoxDimen;
    ray = abs(ray);

    ray = 3.0 * ray;
    ray = (halfBoxDimen + sint * halfBoxDimen) - ray;
    ray = abs(ray);

    float c = sdCross(ray, vec3(halfBoxDimen));
    scale *= 3.0;
    c /= scale;

    dist = c;// = min(dist, c);
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