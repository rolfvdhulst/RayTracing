//
// Created by rolf on 13-8-22.
//

#ifndef RAYTRACING_SRC_RAY_H_
#define RAYTRACING_SRC_RAY_H_
#include "Vec3.h"

struct Ray{
  [[nodiscard]] Vec3r at(Real t) const{
	return origin+t*direction;
  }
  Vec3r origin;
  Vec3r direction;
  Real timeOffset;
};
#endif //RAYTRACING_SRC_RAY_H_
