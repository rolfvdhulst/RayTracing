//
// Created by rolf on 13-8-22.
//

#ifndef RAYTRACING_SRC_HITRECORD_H_
#define RAYTRACING_SRC_HITRECORD_H_

#include "Vec3.h"
#include "Material.h"
#include <optional>
struct HitRecord{
  Vec3r point;
  Vec3r normal;
  Real t;
  Material material;
  bool frontFace;
};

std::optional<HitRecord> closestHitRecord(std::optional<HitRecord> first, std::optional<HitRecord> second){
  if(first.has_value() && second.has_value()){
	if(first->t < second->t){
	  return first;
	}
	return second;
  }
  if(first.has_value()){return first;}
  return second; //If second has no value, it is nullopt, so this is correct.
}

#endif //RAYTRACING_SRC_HITRECORD_H_
