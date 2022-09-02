//
// Created by rolf on 13-8-22.
//

#ifndef RAYTRACING__SPHERE_H_
#define RAYTRACING__SPHERE_H_
#include "Vec3.h"
#include "Ray.h"
#include "HitRecord.h"
#include <optional>
#include "AABB.h"

class Sphere{
 public:
  [[nodiscard]] std::size_t index() const;
 private:
  std::size_t idx;
};
std::size_t Sphere::index() const {
  return idx;
}
class SphereData {
 public:
  SphereData(Vec3r origin, Real rad, Material material, Vec3r velocity = Vec3r(0,0,0)) : origin{origin}, radius{rad}, velocity{velocity}, mat{material}{};

  [[nodiscard]] std::optional<HitRecord> hit(const Ray& ray, Real tMin, Real tMax) const;

  [[nodiscard]] Vec3r center(Real timeOffset) const;

  [[nodiscard]] AABB boundingBox(Real maxTimeOffset) const;

  [[nodiscard]] Material material() const{ return mat;}
 private:
  Vec3r origin;
  Real radius;
  Vec3r velocity;

  Material mat;
};


std::optional<HitRecord> SphereData::hit(const Ray &ray, Real tMin, Real tMax) const{
  Vec3r sphereCenter = center(ray.timeOffset);
  Vec3r originToCenter = ray.origin-sphereCenter;
  Real a = ray.direction.squaredNorm();
  Real half_b = originToCenter.dot(ray.direction);
  Real c = originToCenter.squaredNorm() - radius*radius;
  Real discriminant = half_b*half_b-a*c;
  if(discriminant < 0){
	return std::nullopt;
  }
  Real sqrtDiscriminant = std::sqrt(discriminant);
  auto root = (- half_b - sqrtDiscriminant) / a;
  if( root < tMin || tMax < root){
	root = (-half_b + sqrtDiscriminant) / a;
	if(root < tMin || tMax < root){
	  return std::nullopt;
	}
  }
  Vec3r point = ray.at(root);
  Vec3r outwardNormal = (point-sphereCenter) / radius;
  bool frontFace = ray.direction.dot(outwardNormal) < 0;
  if(!frontFace){
	outwardNormal = -outwardNormal;
  }
  return HitRecord{.point = point,.normal = outwardNormal , .t = root,.material = mat,.frontFace = frontFace,
				   };
}
Vec3r SphereData::center(Real timeOffset) const{
  return origin + velocity*timeOffset;
}
AABB SphereData::boundingBox(Real maxTimeOffset) const {
  AABB box0(origin - Vec3r(radius,radius,radius),
			origin + Vec3r(radius,radius,radius));
  Vec3r finalCenter =center(maxTimeOffset);
  AABB box1(finalCenter - Vec3r(radius,radius,radius),
			finalCenter + Vec3r(radius,radius,radius));
  return {box0,box1};
}

#endif //RAYTRACING__SPHERE_H_
