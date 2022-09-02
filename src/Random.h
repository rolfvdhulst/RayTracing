//
// Created by rolf on 13-8-22.
//

#ifndef RAYTRACING_SRC_RANDOM_H_
#define RAYTRACING_SRC_RANDOM_H_
#include "Vec3.h"

//int randomInt(){
//  return rand();
//}
//template<std::floating_point T>
//T randomReal(){
//  return rand() / (RAND_MAX+1.0);
//}
//template<std::floating_point T>
//T randomReal(T a, T b){
//  return a + (b-a) * randomReal<T>();
//}
//
//template<std::floating_point T>
//Vec3<T> randomInUnitSphere() {
//  while(true){
//	Vec3<T> vec(randomReal(-1.0,1.0),randomReal(-1.0,1.0), randomReal(-1.0,1.0));
//	if(vec.squaredNorm() > 1) continue;
//	return vec;
//  }
//}
//
//template<std::floating_point T>
//Vec3<T> randomInUnitDisk() {
//  while(true){
//	Vec3<T> vec(randomReal(-1.0,1.0),randomReal(-1.0,1.0),0);
//	if(vec.squaredNorm() > 1) continue;
//	return vec;
//  }
//}
//template<std::floating_point T>
//Vec3<T> randomInHemisphere(const Vec3<T>& normal) {
//  Vec3<T> inUnitSphere = randomInUnitSphere<T>();
//  if(inUnitSphere.dot(normal) > 0.0){
//	return inUnitSphere;
//  }else{
//	return -inUnitSphere;
//  }
//}


class RandomDevice64{
 public:
  explicit RandomDevice64(uint64_t seed){
	state = (((__uint128_t) seed) << 64 ) + seed; //bad seeding, but should suffice/whatever
  }
  //By Lehmer
  uint64_t randomUInt(){
	state *= 0xda942042e4dd58b5;
	return state >> 64;
  }
  double randomReal(){
	return (double)randomUInt()/ double(UINT64_MAX);
  }
  double randomReal(double a, double b){
	return a + (b-a) * randomReal();
  }
  Vec3d randomVec(){
	return {randomReal(),randomReal(),randomReal()};
  }
  Vec3d randomInUnitSphere(){
	while(true){
	  Vec3d vec(randomReal(-1.0,1.0),randomReal(-1.0,1.0), randomReal(-1.0,1.0));
	  if(vec.squaredNorm() > 1) continue;
	  return vec;
	}
  }
  Vec3d randomInUnitDisk() {
	while (true) {
	  Vec3d vec(randomReal(-1.0, 1.0), randomReal(-1.0, 1.0), 0);
	  if (vec.squaredNorm() > 1) continue;
	  return vec;
	}
  }
 private:
  __uint128_t state;
};

using RandomDevice = RandomDevice64;

#endif //RAYTRACING_SRC_RANDOM_H_
