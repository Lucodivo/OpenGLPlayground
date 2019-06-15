#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform vec2 screenTexMax;
uniform float kernel[9];

const float offset = 1.0 / 300.0;  

void main()
{   
    float negativeS = TexCoords.s - offset;
    float negativeT = TexCoords.t - offset;
    float positiveS = TexCoords.s + offset;
    float positiveT = TexCoords.t + offset;

    if(negativeS < 0.0) {
        negativeS = TexCoords.s;
    }
    if(negativeT < 0.0) {
        negativeT = TexCoords.t;
    }
    if(positiveS > 1.0) {
        positiveS = TexCoords.s;
    }
    if(positiveT > 1.0) {
        positiveT = TexCoords.t;
    }

    vec3 col = vec3(0.0);
    col += vec3(texture(screenTexture, vec2(negativeS, positiveT))) * kernel[0];
    col += vec3(texture(screenTexture, vec2(TexCoords.s, positiveT))) * kernel[1];
    col += vec3(texture(screenTexture, vec2(positiveS, positiveT))) * kernel[2];
    col += vec3(texture(screenTexture, vec2(negativeS, TexCoords.t))) * kernel[3];
    col += vec3(texture(screenTexture, vec2(TexCoords.s, TexCoords.t))) * kernel[4];
    col += vec3(texture(screenTexture, vec2(positiveS, TexCoords.t))) * kernel[5];
    col += vec3(texture(screenTexture, vec2(negativeS, negativeT))) * kernel[6];
    col += vec3(texture(screenTexture, vec2(TexCoords.s, negativeT))) * kernel[7];
    col += vec3(texture(screenTexture, vec2(positiveS, negativeT))) * kernel[8];
    
    FragColor = vec4(col, 1.0);
}  