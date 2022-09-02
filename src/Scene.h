//
// Created by rolf on 13-8-22.
//

#ifndef RAYTRACING_SRC_SCENE_H_
#define RAYTRACING_SRC_SCENE_H_

#include "Sphere.h"
#include <vector>
#include <optional>
#include "HitRecord.h"
#include "MaterialData.h"
#include "Material.h"
#include "BoundingVolumeHierarchy.h"

class Scene{
 public:
  [[nodiscard]] const MaterialData& material(Material material) const{
	return materials[material.index()];
  }
  [[nodiscard]] Material addMaterial(MaterialData material){
	Material index(materials.size());
	materials.push_back(material);
	return index;
  }
  void addSphere(SphereData sphere);
  void addRectangle(AARectangleData rectangle);

  [[nodiscard]] std::optional<HitRecord> hit(const Ray& ray, Real tMin, Real tMax) const{
	return bvh.hit(ray,tMin,tMax);
  }
  void setBackgroundColor(Vec3r color){
	bgColor = color;
  }
  [[nodiscard]] const Vec3r& backgroundColor() const{
	return bgColor;
  }
  void initialize(Real shutterTime,RandomDevice& device);
 private:
  Vec3r bgColor;
  std::vector<SphereData> spheres;
  std::vector<AARectangleData> rectangles;

  BVH bvh;
  std::vector<MaterialData> materials;
};

void Scene::initialize(Real shutterTime,RandomDevice& device){
  std::vector<BVHObject> objects;
  for(const auto& sphere : spheres){
	objects.emplace_back(sphere);
  }
  for(const auto& rectangle : rectangles){
	objects.emplace_back(rectangle);
  }
  bvh = BVH(objects,shutterTime,device);
}
void Scene::addSphere(SphereData sphere) {
  spheres.push_back(sphere);
}
void Scene::addRectangle(AARectangleData rectangle){
  rectangles.push_back(rectangle);
}
#endif //RAYTRACING_SRC_SCENE_H_
