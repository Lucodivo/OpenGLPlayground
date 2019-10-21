#version 330 core
layout (location = 0) in vec3 aPosition;

layout (std140) uniform globalBlockVS {
                    // base alignment	  aligned offset
  mat4 projection;  // 64				        0
  mat4 view;        // 64				        64
};

uniform mat4 model;

void main()
{
  gl_Position = projection * view * model * vec4(aPosition, 1.0);
}