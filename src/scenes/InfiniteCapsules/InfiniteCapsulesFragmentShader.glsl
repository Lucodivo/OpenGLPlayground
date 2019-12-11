#version 330 core

out vec4 FragColor;

#define MAX_STEPS 150
#define MISS_DIST 100.0
#define HIT_DIST 0.01

float distPosToScene(vec3 pos);
float distanceRayToScene(vec3 rayOrigin, vec3 rayDir);
float distToInfiniteSpheres(vec3 pos);
float distToInfiniteCapsules(vec3 rayPos);
float distToXZAlignedPlane(vec3 rayPos, float planeValue);
float distToSphere(vec3 rayPos, vec3 spherePos, float radius);
float distToCapsule(vec3 rayPos, vec3 posA, vec3 posB, float radius);
float distToCylinder(vec3 rayPos, vec3 posA, vec3 posB, float radius);
float distToTorus(vec3 rayPos, vec3 torusPos, float radiusOuter, float radiusInner);
float distToBox(vec3 rayPos, float width, float height, float depth);
float getLight(vec3 surfacePos);
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

  vec2 uv = (gl_FragCoord.xy-0.5*viewPortResolution.xy)/viewPortResolution.y;

  vec3 rayDir = vec3(uv.x, uv.y, 1.0); // NOTE: Expected to be normalized!
  rayDir = vec3(vec4(rayDir, 0.0) * viewRotationMat);
  rayDir = normalize(rayDir);

  float dist = distanceRayToScene(rayOrigin, rayDir);
  //vec3 worldColor = vec3((sinElapsedTime + 1.0) / 2.0, cos(elapsedTime/7), (cosElapsedTime + 1.0) / 2.0);
  //vec3 worldColor = vec3(1.0, 1.0, 0.4);

  if(dist > 0.0) { // hit
    vec3 surfacePos = rayOrigin + (rayDir * dist);
    float diffuse = getLight(surfacePos);
    vec3 col = vec3(diffuse);

    //dist = dist / 7.0;
    //vec3 col = vec3(dist);

    FragColor = vec4(col * (lightColor * 0.75), 1.0);
  } else { // miss
    vec3 missColor = vec3(0.2, 0.2, 0.2);
    FragColor = vec4(missColor, 1.0);
  }
}

vec3 getNormal(vec3 surfacePos) {
  float dist = distPosToScene(surfacePos);
  vec2 epsilon = vec2(0.1, 0);
  vec3 normal = vec3(dist) - vec3(
    distPosToScene(surfacePos - epsilon.xyy),
    distPosToScene(surfacePos - epsilon.yxy),
    distPosToScene(surfacePos - epsilon.yyx)
  );
  return normalize(normal);
}

float getLight(vec3 surfacePos) {

  vec3 lightDir = normalize(lightPos - surfacePos);
  vec3 normal = getNormal(surfacePos);

  float diff = clamp(dot(normal, lightDir), 0.0, 1.0);

  // calculate for shadows
  //float dist = distanceRayToScene(surfacePos + (normal * HIT_DIST * 2.0), lightDir);
  //if(dist < length(lightPos - surfacePos)) diff *= 0.1;

  if(lightPos == vec3(0.0, 0.0, 0.0)) return diff * 0.1;

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

float distPosToScene(vec3 rayPos) {
  //return distToBox(rayPos, 3.0, 3.0, 3.0);
  //return distToCylinder(rayPos, vec3(-1.5, 1.5, -6.0), vec3(1.5, 1.5, -6.0), 1.0);
  return distToInfiniteCapsules(rayPos);
}

float distToBox(vec3 rayPos, float width, float height, float depth) {
  vec3 rayToCorner = abs(rayPos) - vec3(width, height, depth);
  return length(max(rayToCorner, 0.0));
}

float distToTorus(vec3 rayPos, vec3 torusPos, float radiusOuter, float radiusInner) {
  float distXZRayToCenter = length(length(rayPos.xz - torusPos.xz));
  float a = distXZRayToCenter - radiusInner;
  float b = rayPos.y - torusPos.y;
  float c = length(vec2(a, b));
  return c - radiusOuter;
}

const vec3 offset = vec3(3.0, 3.0, 3.0);
float distToInfiniteCapsules(vec3 rayPos) {
  vec3 posA = vec3(vec4(-1.5, 0.0, 0.0, 0.0) * viewRotationMat);
  vec3 posB = vec3(vec4(1.5, 0.0, 0.0, 0.0) * viewRotationMat);
  posA = posA + offset;
  posB = posB + offset;
  float infiniteCapsuleDistance = distToCapsule(mod(rayPos, 2 * offset), posA, posB, 1.0);
  float lightSphereDistance = distance(rayPos.xyz, lightPos) - 0.5;
  return min(infiniteCapsuleDistance, lightSphereDistance);
}

float distToCapsule(vec3 rayPos, vec3 posA, vec3 posB, float radius) {
  vec3 aToB = posB - posA;
  vec3 aToRayPos = rayPos - posA;
  float abCosTheta = dot(aToB, aToRayPos);
  float magnitudeAToB = length(aToB);
  float projectionAToRayOnAToB = abCosTheta / magnitudeAToB;
  vec3 closestPoint = posA + (clamp(projectionAToRayOnAToB / magnitudeAToB, 0.0, 1.0) * aToB);
  return length(rayPos - closestPoint) - radius;
}

float distToCylinder(vec3 rayPos, vec3 posA, vec3 posB, float radius) {
  vec3 aToB = posB - posA;
  vec3 aToRayPos = rayPos - posA;
  float abCosTheta = dot(aToB, aToRayPos);
  float magnitudeAToB = length(aToB);
  float projectionAToRayOnAToB = abCosTheta / magnitudeAToB; // projection of ray onto vector from A to B
  projectionAToRayOnAToB = projectionAToRayOnAToB / magnitudeAToB; // projection scaled (A = 0, B = 1)
  vec3 closestPointToInfiniteCylinder = posA + (projectionAToRayOnAToB * aToB);
  float distanceToInfiniteCylinder = length(rayPos - closestPointToInfiniteCylinder) - radius;
  float yDist = (abs(projectionAToRayOnAToB - 0.5) - 0.5) * magnitudeAToB;
  float exteriorDistance = length(max(vec2(distanceToInfiniteCylinder, yDist), 0.0));
  float interiorDistance = min(max(distanceToInfiniteCylinder, yDist), 0.0);
  return exteriorDistance + interiorDistance;
}

float distToSphere(vec3 rayPos, vec3 spherePos, float radius) {
  return distance(spherePos, rayPos) - radius;
}

float distToXZAlignedPlane(vec3 rayPos, float planeValue) {
  return rayPos.y - planeValue;
}

const vec3 infiniteSpherePosition = vec3(1.5, 1.5, 1.5);
const float infiniteSphereRadius = 0.5;
const float repeatWidth = 3.0;
float distToInfiniteSpheres(vec3 rayPos) {
  float infiniteSphereDistance = distance(mod(rayPos, repeatWidth), infiniteSpherePosition) - infiniteSphereRadius;
  float lightSphereDistance = distance(rayPos.xyz, lightPos) - 0.5;
  return min(infiniteSphereDistance, lightSphereDistance);
}