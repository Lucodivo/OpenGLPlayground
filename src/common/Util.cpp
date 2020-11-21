#include "Util.h"

#include <time.h>

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

float32 getTime() {
  clock_t time = clock();
  return (float32)time / CLOCKS_PER_SEC;
}

//template <typename T>
//class Consumable {
//  T value;
//  const T emptyValue;
//  Consumable<T>(T startValue, T emptyValue) {
//    value = startValue;
//    this->emptyValue = emptyValue
//  }
//  T setValue(T value) { this->value = value; }
//  bool isEmpty() { value == emptyValue; }
//  T consume() {
//    T returnValue = value;
//    value = emptyValue;
//    return returnValue;
//  }
//};