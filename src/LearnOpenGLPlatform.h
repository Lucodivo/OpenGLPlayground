#pragma once

#include <stdint.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float float32;
typedef double float64;

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

#define local_access static
#define file_access static
#define class_access static

#ifdef NOT_DEBUG
#define Assert(Expression)
#else
#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}
#endif

#define InvalidCodePath Assert(!"InvalidCodePath");