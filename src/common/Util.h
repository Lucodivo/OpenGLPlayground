#pragma once

#include <glm/gtc/type_ptr.hpp>
#include "../LearnOpenGLPlatform.h"

void swap(float32* a, float32* b);
glm::mat4& reverseZ(glm::mat4& mat);
float32 getTime();
bool consume(bool& val);

class Consumabool {
public:
  bool value;
  Consumabool(bool startValue) {
    value = startValue;
  }
  void set() { value = true; }
  bool consume() {
    bool returnValue = value;
    value = false;
    return returnValue;
  }
};