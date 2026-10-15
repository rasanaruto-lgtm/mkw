#include "float.h"

#include <rk_types.h>

int __fpclassifyf(float x) {
  union {
    float f;
    u32 w;
  } u;
  u.f = x;

  switch (u.w & 0x7F800000) {
  case 0x7F800000:
    return (u.w & 0x007FFFFF) ? 1 : 2;
  case 0:
    return (u.w & 0x007FFFFF) ? 5 : 3;
  }
  return 4;
}

int __signbitd(double x) {
  union {
    double d;
    u32 w[2];
  } u;
  u.d = x;
  return (int)(u.w[0] & 0x80000000);
}

int __fpclassifyd(double x) {
  union {
    double d;
    u32 w[2];
  } u;
  u.d = x;

  switch (u.w[0] & 0x7FF00000) {
  case 0x7FF00000:
    if ((u.w[0] & 0x000FFFFF) || u.w[1]) {
      return 1;
    }
    return 2;
  case 0:
    if ((u.w[0] & 0x000FFFFF) || u.w[1]) {
      return 5;
    }
    return 3;
  }
  return 4;
}
