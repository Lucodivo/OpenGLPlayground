#include "Util.h"

void swap(float32* a, float32* b)
{
  float32 tmp = *a;
  *a = *b;
  *b = tmp;
}

glm::mat4& reverseZ(glm::mat4& mat)
{
  mat[0][2] = -mat[0][2];
  mat[1][2] = -mat[1][2];
  mat[2][2] = -mat[2][2];
  mat[3][2] = -mat[3][2];
  return mat;
}