#version 330 core

out vec4 FragColor;

#define MAX_STEPS 120
#define MISS_DIST 60.0
#define HIT_DIST 0.01

vec3 hitColor(float numSteps);
float distPosToScene(vec3 pos);
vec2 distanceRayToScene(vec3 rayOrigin, vec3 rayDir);
float sdBox(vec3 rayPos, vec3 dimen);
float sdCross(vec3 rayPos, vec3 dimen);
float sdRect(vec2 rayPos, vec2 dimen);
float sdMengerSponge(vec3 rayPos, int numIterations);

float sdTwentySevenCrossesBound(vec3 rayPos);
float sdTwentySevenCrossesUnbound(vec3 rayPos);
float sdOneCrossOvercomplicatedBound(vec3 rayPos);
float sdOneCrossOvercomplicatedUnbound(vec3 rayPos);
float sdMengerSpongeFirstIteration(vec3 rayPos);
float sdMengerSpongeSecondIteration(vec3 rayPos);
float sdMengerSpongeFirstIterationOvercomplicated(vec3 rayPos);
float sdInigoQuilezMengerSponge(vec3 rayPos);

float crush(vec3 rayPos, bool boxed);
float multistagePrison(vec3 rayPos, bool boxed);
float sdMengerJank(vec3 rayPos, int numIterations);
float sdMengerPrison(vec3 rayPos);

uniform vec2 viewPortResolution;
uniform vec3 rayOrigin;
uniform float elapsedTime;
uniform mat4 viewRotationMat;

const float boxDimen = 20.0;
const float halfBoxDimen = boxDimen / 2.0;
const int numSamples = 5;


float sint;

// TODO: Multiple samples per pixel?
void main()
{
  // Move (0,0) from bottom left to center
  // Coordinate system goes from [-viewPortResolution / 2, viewPortResolution / 2]
  vec2 pixelCoord = gl_FragCoord.xy-0.5*viewPortResolution.xy;
  float pixelWidth = 1.0 / viewPortResolution.y;
  // Scale y value to [-0.5, 0.5], scale x by same factor
  pixelCoord = pixelCoord * pixelWidth;

  float samplePixelOffset = pixelWidth / 3.0; // 1.0 / 4.0; <- for a tripy things
  vec3 rayDirSamples[numSamples] = vec3[](
    normalize(vec3(pixelCoord.x, pixelCoord.y, 1.0)),
    normalize(vec3(pixelCoord.x - samplePixelOffset, pixelCoord.y - samplePixelOffset, 1.0)),
    normalize(vec3(pixelCoord.x - samplePixelOffset, pixelCoord.y + samplePixelOffset, 1.0)),
    normalize(vec3(pixelCoord.x + samplePixelOffset, pixelCoord.y - samplePixelOffset, 1.0)),
    normalize(vec3(pixelCoord.x + samplePixelOffset, pixelCoord.y + samplePixelOffset, 1.0))
  );

  vec2 dist = vec2(0.0, 0.0);
  for(int sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex) {
    rayDirSamples[sampleIndex] = vec3(vec4(rayDirSamples[sampleIndex], 0.0) * viewRotationMat);
    dist += distanceRayToScene(rayOrigin, rayDirSamples[sampleIndex]);
  }
  dist /= numSamples;


  if(dist.x > 0.0) { // hit
    vec3 col = hitColor(dist.y);
    FragColor = vec4(col, 1.0);
  } else { // miss
    vec3 missColor = hitColor(MAX_STEPS);
    FragColor = vec4(missColor, 1.0);
  }
}

vec3 hitColor(float numSteps) {
  float brightness = 1.0 - (numSteps/float(MAX_STEPS));
//  brightness = clamp(brightness, 0.4, 0.9);
//  brightness = brightness - mod(brightness, 0.1);

  vec3 color = vec3(brightness, 0.0, 0.0);
  return color;
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
  return sdMengerPrison(rayPos);
}

float sdMengerPrison(vec3 rayPos) {
  float sintime = sin((rayPos.y * 6.28) / 8 + (elapsedTime)) * 1.5 ;
  rayPos.x += sintime;

  vec3 prisonRay = mod(rayPos, boxDimen * 2.0);
  prisonRay -= boxDimen;

  float mengerPrisonDist = sdCross(prisonRay, vec3(halfBoxDimen));
  if(mengerPrisonDist > HIT_DIST) return mengerPrisonDist;

  float scale = 1.0;
  for(int i = 0; i < 5; ++i) {
    float boxedWorldDimen = boxDimen / scale;
    vec3 ray = mod(rayPos + boxedWorldDimen / 2.0, boxedWorldDimen);
    ray -= boxedWorldDimen / 2.0;
    ray *= scale;
    float crossesDist = sdCross(ray * 3.0, vec3(halfBoxDimen));
    scale *= 3.0;
    crossesDist /= scale;
    mengerPrisonDist = max(mengerPrisonDist, -crossesDist);
  }

  return mengerPrisonDist;
}

float sdMengerJank(vec3 rayPos, int numIterations) {
  vec3 prisonRay = rayPos + (boxDimen * 1.5);
  prisonRay = mod(prisonRay, boxDimen * 3.0);
  float mengerPrisonDist = sdCross(prisonRay, vec3(halfBoxDimen));

  float scale = 1.0;
  for(int i = 0; i < numIterations; ++i) {
    float boxedWorldDimen = boxDimen / scale;
    float translation = boxedWorldDimen / 2.0;
    vec3 ray = mod(rayPos + translation, boxedWorldDimen);
    ray -= (boxedWorldDimen) / 2.0;
    ray *= scale;
    float crossesDist = sdCross(ray * 3.0, vec3(halfBoxDimen)) / 3.0;
    crossesDist /= scale;
    mengerPrisonDist = max(mengerPrisonDist, -crossesDist);
    scale *= 3.0;
  }

  return mengerPrisonDist;
}

float sdMengerSponge(vec3 rayPos, int numIterations) {
  float mengerSpongeDist = sdBox(rayPos, vec3(halfBoxDimen));
  if(mengerSpongeDist > HIT_DIST) return mengerSpongeDist; // use dist of sponge box as bounding box

  float scale = 1.0;
  for(int i = 0; i < numIterations; ++i) {
    float boxedWorldDimen = boxDimen / scale;
    vec3 ray = mod(rayPos + boxedWorldDimen / 2.0, boxedWorldDimen);
    ray -= boxedWorldDimen / 2.0;
    ray *= scale;
    float crossesDist = sdCross(ray * 3.0, vec3(halfBoxDimen));
    scale *= 3.0;
    crossesDist /= scale;
    mengerSpongeDist = max(mengerSpongeDist, -crossesDist);
  }

  return mengerSpongeDist;
}

float sdMengerSpongeSecondIteration(vec3 rayPos) {
  float distFirstIter = sdMengerSpongeFirstIteration(rayPos);
  float distCrossesUnbound = sdTwentySevenCrossesUnbound(rayPos);
  return max(distFirstIter, -distCrossesUnbound);
}

float sdTwentySevenCrossesBound(vec3 rayPos) {
  float boundingBox = sdBox(rayPos, vec3(halfBoxDimen));
  float crossDist = sdTwentySevenCrossesUnbound(rayPos);
  return max(boundingBox, crossDist);
}

float sdTwentySevenCrossesUnbound(vec3 rayPos) {
  float boxedWorldDimen = boxDimen / 3.0;
  float translation = boxedWorldDimen / 2.0;
  vec3 ray = mod(rayPos + translation, boxedWorldDimen);
  ray -= (boxDimen / 3.0) / 2.0;
  ray *= 3.0;
  float crossesDist = sdCross(ray * 3.0, vec3(halfBoxDimen)) / 3.0;
  crossesDist /= 3.0;
  return crossesDist;
}

float sdOneCrossOvercomplicatedBound(vec3 rayPos) {
  float boundingBox = sdBox(rayPos, vec3(halfBoxDimen));
  float crossDist = sdOneCrossOvercomplicatedUnbound(rayPos);
  return max(boundingBox, crossDist);
}

float sdOneCrossOvercomplicatedUnbound(vec3 rayPos) {
  float boxedWorldDimen = boxDimen / 1.0;
  float translation = boxedWorldDimen / 2.0;
  vec3 ray = mod(rayPos + translation, boxedWorldDimen);
  ray -= (boxDimen / 1.0) / 2.0;
  ray *= 1.0;
  float crossDist = sdCross(ray * 3.0, vec3(halfBoxDimen)) / 3.0;
  crossDist /= 1.0;
  return crossDist;
}

float sdMengerSpongeFirstIterationOvercomplicated(vec3 rayPos) {
  float crossDist = sdOneCrossOvercomplicatedUnbound(rayPos);
  float spongeBox = sdBox(rayPos, vec3(halfBoxDimen));
  return max(spongeBox, -crossDist);
}

float sdMengerSpongeFirstIteration(vec3 rayPos) {
  float crossDist = sdCross(rayPos * 3.0, vec3(halfBoxDimen)) / 3.0;
  float spongeBox = sdBox(rayPos, vec3(halfBoxDimen));
  return max(spongeBox, -crossDist);
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

    dist = c;
  }

  return dist;
}

float sdInigoQuilezMengerSponge(vec3 rayPos) {
  float dist = sdBox(rayPos, vec3(halfBoxDimen));
  if(dist > HIT_DIST) return dist; // use the full box as a bounding volume

  float scale = 1.0;
  for( int iteration = 0; iteration < 3; iteration++ )
  {
    vec3 ray = mod(rayPos * scale, boxDimen);
    ray -= halfBoxDimen;
    ray = abs(ray);
    ray = 3.0 * ray;
    ray = halfBoxDimen - ray;
    ray = abs(ray);
    float crossDist = sdCross(ray, vec3(halfBoxDimen));
    scale *= 3.0;
    crossDist /= scale;
    dist = max(dist, crossDist);
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
  // maxDelta is the maximum negative value if the point exists inside of the box, otherwise 0.0
  float maxDelta = min(max(rayToCorner.x, rayToCorner.y), 0.0);
  return length(max(rayToCorner, 0.0)) + maxDelta;
}

float sdCross(vec3 rayPos, vec3 dimen) {
  float da = sdRect(rayPos.xy, dimen.xy);
  float db = sdRect(rayPos.xz, dimen.xz);
  float dc = sdRect(rayPos.yz, dimen.yz);
  return min(da,min(db,dc));
}

//float sdCross(vec3 rayPos, vec3 dimen) {
//  vec3 ray = abs(rayPos); // fold ray into positive quadrant
//  vec3 cornerToRay = ray - dimen;
//
//  float smallestComp = min(min(cornerToRay.x, cornerToRay.y), cornerToRay.z);
//  float largestComp = max(max(cornerToRay.x, cornerToRay.y), cornerToRay.z);
//  float middleComp = cornerToRay.x - largestComp + cornerToRay.y - smallestComp + cornerToRay.z;
//
//  vec2 closestOutsidePoint = max(vec2(smallestComp, middleComp), 0.0);
//  vec2 closestInsidePoint = min(vec2(middleComp, largestComp), 0.0);
//
//  // if at least two components are larger than the dimen
//  // we are outside of the cross
//  return (middleComp > 0.0) ? length(closestOutsidePoint) : -length(closestInsidePoint);
//}