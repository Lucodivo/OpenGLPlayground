#version 330 core

struct LightColor{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct DirectionalLight{
  vec3 direction;
  LightColor color;
};

struct Material {
  sampler2D diffTexture;
  sampler2D specTexture;
  float shininess;
};

uniform vec3 cameraPos;
uniform DirectionalLight directionalLight;
uniform Material material;
uniform bool animSwitch;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

vec4 calcDirectionalLightColor();
vec4 calcLightColor(vec3 lightDir, LightColor lightColor);
vec4 gammaCorrectionToSRGB(vec4 color);

vec4 diffColor;
vec4 specColor;

const float gammaToLinear = 2.2;
const float gammaToSRBG = 1.0 / gammaToLinear;

void main()
{
  diffColor = texture(material.diffTexture, TexCoords);
  specColor = texture(material.specTexture, TexCoords);

  vec4 directionalResult = calcDirectionalLightColor();

  vec4 color = directionalResult;
  FragColor = gammaCorrectionToSRGB(color);
}

vec4 calcDirectionalLightColor() {
  vec3 lightDir = normalize(-directionalLight.direction);
  return calcLightColor(lightDir, directionalLight.color);
}

vec4 calcLightColor(vec3 lightDir, LightColor lightColor) {
  // ambient light
  vec4 ambient = vec4(lightColor.ambient, 1.0) * diffColor;

  // diffuse light
  vec3 norm = normalize(Normal);
  float diffStrength = max(dot(norm, lightDir), 0.0);
  vec4 diffuse = vec4(lightColor.diffuse, 1.0) * diffStrength * diffColor;

  // specular light
  vec3 viewDir = normalize(cameraPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec4 specular = vec4(lightColor.specular, 1.0) * spec * specColor;

  return ambient + diffuse + specular;
}

vec4 gammaCorrectionToSRGB(vec4 color) {
  const float gammaPiecewiseEpsilon = 0.0031308;
  const float gammaMultiplierBelowEspilon = 12.92;
  const float gammaMultiplierOtherwise = 1.055;
  const float gammaPowOtherwise = 1 / 2.4;
  const float gammaOffsetOtherwise = -0.055;
  vec4 sRGB;

  // This formula was pulled from Real-Time Rendering 4th Edition pg 162
  sRGB.x = color.x < gammaPiecewiseEpsilon ? (gammaMultiplierBelowEspilon * color.x) :
  ((gammaMultiplierOtherwise * pow(color.x, gammaPowOtherwise)) + gammaOffsetOtherwise);
  sRGB.y = color.y < gammaPiecewiseEpsilon ? (gammaMultiplierBelowEspilon * color.y) :
  ((gammaMultiplierOtherwise * pow(color.y, gammaPowOtherwise)) + gammaOffsetOtherwise);
  sRGB.z = color.z < gammaPiecewiseEpsilon ? (gammaMultiplierBelowEspilon * color.z) :
  ((gammaMultiplierOtherwise * pow(color.z, gammaPowOtherwise)) + gammaOffsetOtherwise);
  sRGB.w = color.w; // no gamma correction for alpha values

  return sRGB;
}