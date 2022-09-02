//
// Created by rolf on 14-8-22.
//

#ifndef RAYTRACING_SRC_MATERIALDATA_H_
#define RAYTRACING_SRC_MATERIALDATA_H_

#include <cassert>
#include "Vec3.h"
#include "Ray.h"
#include "HitRecord.h"
#include "Random.h"
#include <variant>


Real reflectance(Real cosine, Real refractionIndex){
  Real r0 = (1-refractionIndex) / (1+refractionIndex);
  r0 = r0*r0;
  return r0 + (1-r0)*pow(1-cosine,5);
}

struct DiffuseMaterial{
  explicit DiffuseMaterial(Vec3r color) : color{color},emittedColor{Vec3r(0,0,0)}{};
  explicit DiffuseMaterial(Vec3r color,Vec3r emittedColor) : color{color},emittedColor{emittedColor}{};

  Vec3r color;
  Vec3r emittedColor;
  bool scatter(const HitRecord &record, Ray &out, Vec3r &outColor,Real timeOffset,RandomDevice& device) const {
	auto scatterDirection = record.normal + device.randomInUnitSphere().normalized();

	if (scatterDirection.nearZero()) {
	  scatterDirection = record.normal;
	}
	out = Ray{.origin = record.point,.direction =  scatterDirection, .timeOffset = timeOffset};
	outColor = color;
	return true;
  }
  Vec3r emitted(const HitRecord& ) const{
	return emittedColor;
  }

};

struct MetalMaterial{
  MetalMaterial(Vec3r color, Real metalFuzziness) : color{color}, metalFuzziness{metalFuzziness}{};
  Vec3r color;
  Real metalFuzziness;
  bool scatter(const Ray &in, const HitRecord &record, Ray &out, Vec3r &outColor, RandomDevice& device) const {
	Vec3r reflected = reflect(in.direction.normalized(), record.normal);
	out = Ray{.origin = record.point, .direction = reflected + metalFuzziness * device.randomInUnitSphere(),
	    .timeOffset = in.timeOffset};
	outColor = color;
	return out.direction.dot(record.normal) > 0;
  }
  static Vec3r emitted() {
	return {0,0,0};
  }
};
struct DielectricMaterial{
  explicit DielectricMaterial(Real refractionIndex) : refractionIndex{refractionIndex}{};

  Real refractionIndex;

  bool scatter(const Ray &in, const HitRecord &record, Ray &out, Vec3r &outColor, RandomDevice& device) const {
	outColor = Vec3r(1.0, 1.0, 1.0);
	Real refractionRatio = record.frontFace ? (1.0 / refractionIndex) : refractionIndex;
	Vec3r unitDir = in.direction.normalized();
	Real cosTheta = fmin(-(unitDir).dot(record.normal), 1.0);
	Real sinTheta = sqrt(1.0 - cosTheta * cosTheta);
	bool cannotRefract = refractionRatio * sinTheta > Real(1.0) || (reflectance(cosTheta, refractionRatio) > device.randomReal());

	Vec3r direction = cannotRefract ? reflect(unitDir, record.normal) : refract(unitDir, record.normal, refractionRatio);
	out = Ray{.origin = record.point, .direction = direction,.timeOffset = in.timeOffset};
	return true;
  }

  static Vec3r emitted() {
	return {0,0,0};
  }
};

struct MaterialData {
 public:
  template<typename U>
  MaterialData(U data) : material(data){};

  bool scatter(const Ray &in, const HitRecord &record, Ray &out, Vec3r &outColor, RandomDevice& device) const {
	return std::visit(
		overload{
			[&](const DiffuseMaterial &mat) 		{ return mat.scatter(record, out, outColor,in.timeOffset,device); },
			[&](const MetalMaterial &mat) 		{ return mat.scatter(in, record, out, outColor,device); },
			[&](const DielectricMaterial &mat)	{ return mat.scatter(in, record, out, outColor,device); }
		}, material);
  }

  //TODO: merge interface with scattered
  [[nodiscard]] Vec3r emitted(const HitRecord& record) const{
	return std::visit(
		overload{
			[&](const DiffuseMaterial &mat) 		{ return mat.emitted(record); },
			[&](const MetalMaterial &) 		{ return MetalMaterial::emitted(); },
			[&](const DielectricMaterial &)	{ return DielectricMaterial::emitted(); }
		}, material);
  }
 private:
  std::variant<DiffuseMaterial, MetalMaterial, DielectricMaterial> material;

};

#endif //RAYTRACING_SRC_MATERIALDATA_H_
