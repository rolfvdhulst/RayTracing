//
// Created by rolf on 17-8-22.
//

#ifndef RAYTRACING_SRC_AABB_H_
#define RAYTRACING_SRC_AABB_H_

#include "Vec3.h"
#include <cassert>
#include "Ray.h"
//Axis aligned bounding box
class AABB{
 public:
  AABB() = default;
  AABB(const AABB& first, const AABB& second) :
  	min(fmin(first.min.x(),second.min.x()),
	  	fmin(first.min.y(),second.min.y()),
	  	fmin(first.min.z(),second.min.z())),
	max(fmax(first.max.x(),second.max.x()),
		fmax(first.max.y(),second.max.y()),
		fmax(first.max.z(),second.max.z())){};

  AABB(const Vec3r& minimum, const Vec3r& maximum) : min{minimum}, max{maximum}{
	assert(minimum.x() <= maximum.x() && minimum.y() <= maximum.y() && minimum.z() <= maximum.z());
  };
  [[nodiscard]] bool hit(const Ray& ray, Real tMin, Real tMax) const{
	for (int a = 0; a < 3; a++) {
	  Real invD = Real(1.0) / ray.direction[a];
	  Real t0 = (min[a] - ray.origin[a]) * invD;
	  Real t1 = (max[a] - ray.origin[a]) * invD;
	  if (invD < 0.0f)
		std::swap(t0, t1);
	  tMin = t0 > tMin ? t0 : tMin;
	  tMax = t1 < tMax ? t1 : tMax;
	  if (tMax <= tMin)
		return false;
	}
	return true;
  }
  [[nodiscard]] Vec3r minimum() const { return min;}
  [[nodiscard]] Vec3r maximum() const { return max;}
 private:
  Vec3r min;
  Vec3r max;
};
#endif //RAYTRACING_SRC_AABB_H_
