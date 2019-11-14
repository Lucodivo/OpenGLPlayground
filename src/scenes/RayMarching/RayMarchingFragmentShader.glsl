#version 330 core

out vec4 FragColor;

#define MAX_STEPS 100
#define MISS_DIST 20.0
#define HIT_DIST 0.01

const vec4 sphere = vec4(1.5, 1.5, 1.5, 0.5);
const float floorDistance = 0.0;

float distPosToScene(vec3 pos);
float distanceRayToScene(vec3 rayOrigin, vec3 rayDir);
float getLight(vec3 surfacePos);
vec3 getNormal(vec3 surfacePos);

uniform vec2 viewPortResolution;
uniform vec3 rayOrigin;
uniform float elapsedTime;

void main()
{
  vec2 uv = (gl_FragCoord.xy-0.5*viewPortResolution.xy)/viewPortResolution.y;

  vec3 rayDir = normalize(vec3(uv.x, uv.y, 1.0)); // NOTE: Expected to be normalized!

  float dist = distanceRayToScene(rayOrigin, rayDir);

  vec3 surfacePos = rayOrigin + (rayDir * dist);
  float diffuse = getLight(surfacePos);
  vec3 col = vec3(diffuse);

  //dist = dist / 7.0;
  //vec3 col = vec3(dist);

  FragColor = vec4(col, 1.0);
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
  vec3 lightPos = vec3(0, 40.0, 0.0);
  lightPos.xz += vec2(sin(elapsedTime), cos(elapsedTime)) * 60;
  vec3 lightDir = normalize(lightPos - surfacePos);
  vec3 normal = getNormal(surfacePos);

  float diff = clamp(dot(normal, lightDir), 0.0, 1.0);

  // calculate for shadows
  //float dist = distanceRayToScene(surfacePos + (normal * HIT_DIST * 2.0), lightDir);
  //if(dist < length(lightPos - surfacePos)) diff *= 0.1;

  return diff;
}

// NOTE: ray dir is assumed to be normalized
float distanceRayToScene(vec3 rayOrigin, vec3 rayDir) {

  float dist = 0.0;

  for(int i = 0; i < MAX_STEPS; i++) {

    vec3 pos = rayOrigin + (dist * rayDir);
    float posToScene = distPosToScene(pos);
    dist += posToScene;
    if(posToScene < HIT_DIST || posToScene > MISS_DIST) break;
  }

  return dist;
}

float distPosToScene(vec3 pos) {
  //float distToSphere = distance(sphere.xyz, pos.xyz) - sphere.w;
  //float distToPlane = pos.y;
  //return min(distToSphere, distToPlane);
  return distance(mod(pos.xyz, 3.0), sphere.xyz) - sphere.w;
}