#version 330 core

uniform sampler2D diffTexture;
uniform float texWidth;
uniform float texHeight;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

void main()
{
  vec2 croppedTexCoord;
  if (texWidth > texHeight) {
    float texXStart = ((texWidth - texHeight) / 2.0) / texWidth;
    float heightOverWidth = texHeight / texWidth;

    croppedTexCoord = vec2(texXStart + (TexCoords.x * heightOverWidth), TexCoords.y);
  } else {
    float texYStart = ((texHeight - texWidth) / 2.0) / texHeight;
    float widthOverHeight = texWidth / texHeight;

    croppedTexCoord = vec2(TexCoords.x, texYStart + (TexCoords.y * widthOverHeight));
  }

  vec4 diffColor = texture(diffTexture, croppedTexCoord);

  FragColor = diffColor;
}