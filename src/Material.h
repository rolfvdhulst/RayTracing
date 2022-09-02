//
// Created by rolf on 13-8-22.
//

#ifndef RAYTRACING_SRC_MATERIAL_H_
#define RAYTRACING_SRC_MATERIAL_H_



class Material{
 public:
  explicit Material(std::size_t index) : idx{index}{};
  Material() = default;
  [[nodiscard]] std::size_t index() const{
	return idx;
  }
 private:
  std::size_t idx;
};





#endif //RAYTRACING_SRC_MATERIAL_H_
