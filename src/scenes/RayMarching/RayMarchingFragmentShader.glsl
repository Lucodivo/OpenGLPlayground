#version 330 core

out vec4 FragColor;

#define MAX_STEPS 100
#define MISS_DIST 20.0
#define HIT_DIST 0.01

const vec4 sphere = vec4(0.0, 1.0, 6.0, 1.0);
const float floorDistance = 0.0;

float distPosToScene(vec3 pos);
float distanceRayToScene(vec3 rayOrigin, vec3 rayDir);

uniform vec2 viewPortResolution;

uniform vec3 rayOrigin;

void main()
{
  vec2 uv = (gl_FragCoord.xy-0.5*viewPortResolution.xy)/viewPortResolution.y;

  vec3 rayDir = normalize(vec3(uv.x, uv.y, 1.0)); // NOTE: Expected to be normalized!

  float d = distanceRayToScene(rayOrigin, rayDir);
  d = d / 7.0;
  vec3 col = vec3(d);

  FragColor = vec4(col, 1.0);
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
  float distToSphere = distance(sphere.xyz, pos.xyz) - sphere.w;
  float distToPlane = pos.y;
  return min(distToSphere, distToPlane);
}