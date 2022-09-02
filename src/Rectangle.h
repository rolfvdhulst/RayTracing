//
// Created by rolf on 20-8-22.
//

#ifndef RAYTRACING_SRC_RECTANGLE_H_
#define RAYTRACING_SRC_RECTANGLE_H_
#include "Vec3.h"
#include "Ray.h"
#include "HitRecord.h"
#include <optional>
#include "AABB.h"

enum RectangleType : int {
  yz = 0,
  zx = 1,
  xy = 2
};
class AARectangleData{
 public:
  AARectangleData(Material material, RectangleType type,
				  Real uMin, Real uMax, Real vMin, Real vMax, Real w)
				  : u1{uMin},u2{uMax},v1{vMin},v2{vMax},w{w},
				  mat{material}, type{type}{}
				   //U is the first of the rectnagle type, v, the second.
				  //w indicates the coordinate of the third dimension which is fixed.
  [[nodiscard]] AABB boundingBox() const{
	constexpr Real epsilon = 0.0001;
	switch(type){
	  case yz:{
		return {Vec3r(w-epsilon,u1,v1),Vec3r(w+epsilon,u2,v2)};
	  }
	  case zx:{
		return {Vec3r(v1,w-epsilon,u1),Vec3r(v2,w+epsilon,u2)};
	  }
	  case xy:{
		return {Vec3r(u1,v1,w-epsilon),Vec3r(u2,v2,w+epsilon)};
	  }
	}
  }
  [[nodiscard]] std::optional<HitRecord> hit(const Ray& ray, Real tMin, Real tMax) const{
	int  wIdx = (int ) type;
	int  uIdx = ((int) type+1)%3;
	int  vIdx = ((int ) type+2)%3;

	double tPos = (w-ray.origin[wIdx]) / ray.direction[wIdx];
	if( tPos < tMin || tPos > tMax){
	  return std::nullopt;
	}

	Vec3r at = ray.at(tPos);

	if(at[uIdx] < u1 || at[uIdx] > u2 || at[vIdx] < v1 || at[vIdx] > v2){
	  return std::nullopt;
	}

	//Double sided plane
	Vec3r normal(0,0,0);
	normal[wIdx] = 1.0;
	Real dot = normal.dot(ray.direction);
	normal[wIdx] = dot < 0.0 ? 1.0 : -1.0;
	//uv = {(at[u_idx] - u1) / (u2 - u1), (at[v_idx] - v1) / (v2 - v1)};
	return HitRecord{
		.point = at,
		.normal = normal,
		.t = tPos,
		.material = mat,
		.frontFace = dot < 0.0 //TODO: what is frontface here?
	};
  }
 private:
  Real u1,u2;
  Real v1,v2;
  Real w;
  Material mat;
  RectangleType type;
};

#endif //RAYTRACING_SRC_RECTANGLE_H_
