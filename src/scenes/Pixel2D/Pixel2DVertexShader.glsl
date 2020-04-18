#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform vec2 windowDimens;
uniform vec2 lowerLeftOffset;
uniform vec2 spriteDimens;

out vec2 TexCoords;

void main()
{
  float pixelWidth = (1.0 / windowDimens.x);
  float pixelHeight = (1.0 / windowDimens.y);
  float xOffset = pixelWidth * lowerLeftOffset.x;
  float yOffset = pixelHeight * lowerLeftOffset.y;

  gl_Position = vec4(pixelWidth * (xOffset + aPos.x * spriteDimens.x), pixelHeight * (yOffset + aPos.y * spriteDimens.y), 0.0, 1.0);
  TexCoords = aTexCoords;
}