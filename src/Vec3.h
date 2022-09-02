//
// Created by rolf on 13-8-22.
//

#ifndef RAYTRACING__VEC3_H_
#define RAYTRACING__VEC3_H_

#include <concepts>
#include <cmath>
#include "Definitions.h"

template<std::floating_point T>
class Vec3;

template<std::floating_point T>
Vec3<T> operator-(const Vec3<T> &u, const Vec3<T> &v);

template<std::floating_point T>
Vec3<T> operator*(T val, const Vec3<T> &vec);

template<std::floating_point T>
Vec3<T> operator*(const Vec3<T> &vec, T val);

template<std::floating_point T>
Vec3<T> operator+(const Vec3<T> &u, const Vec3<T> &v);

template<std::floating_point T>
Vec3<T> operator+(const Vec3<T> &u, T val);

template<std::floating_point T>
Vec3<T> operator*(const Vec3<T> &u, const Vec3<T> &v);

template<std::floating_point T>
Vec3<T> operator/(const Vec3<T> &vec, T val);


template<std::floating_point T>
class Vec3 {
 public:
  Vec3() = default;
  Vec3(T a, T b, T c) : element{a, b, c} {};

  T x() const { return element[0]; }
  T y() const { return element[1]; }
  T z() const { return element[2]; }

  T &x() { return element[0]; }
  T &y() { return element[1]; }
  T &z() { return element[2]; }

  T operator[](int i) const { return element[i]; } //TODO: int index, size_t index performance effects?
  T &operator[](int i) { return element[i]; }

  Vec3 operator-() const { return Vec3(-element[0], -element[1], -element[2]); }
  Vec3 &operator+=(const Vec3 &other) {
	element[0] += other.element[0];
	element[1] += other.element[1];
	element[2] += other.element[2];
	return *this;
  }
  Vec3 &operator*=(const Vec3 &other) {
	element[0] *= other.element[0];
	element[1] *= other.element[1];
	element[2] *= other.element[2];
	return *this;
  }

  Vec3 &operator/=(const T val) {
	element[0] /= val;
	element[1] /= val;
	element[2] /= val;
	return *this;
  }

  T squaredNorm() const {
	return element[0] * element[0] + element[1] * element[1] + element[2] * element[2];
  };
  T norm() const {
	return std::sqrt(squaredNorm());
  }

  T dot(const Vec3 &other) const {
	return element[0] * other.element[0] + element[1] * other.element[1] + element[2] * other.element[2];
  }
  Vec3 cross(const Vec3& other) const {
	return Vec3(element[1] * other.element[2] - element[2] * other.element[1],
				element[2] * other.element[0] - element[0] * other.element[2],
				element[0] * other.element[1] - element[1] * other.element[0]);
  }

  Vec3 normalized() const{
	T length = norm();
	Vec3 vec = *this;
	return (vec /= length);
  }

  friend Vec3<T> operator-<>(const Vec3<T> &u, const Vec3<T> &v);
  friend Vec3<T> operator*<>(T val, const Vec3<T> &vec);

  friend Vec3<T> operator*<>(const Vec3<T> &vec, T val);

  friend Vec3<T> operator+<>(const Vec3<T> &u, const Vec3<T> &v);
  friend Vec3<T> operator+<>(const Vec3<T> &u,T val);

  friend Vec3<T> operator*<>(const Vec3<T> &u, const Vec3<T> &v);

  friend Vec3<T> operator/<>(const Vec3<T> &u, T val);

  [[nodiscard]] bool nearZero() const{
	constexpr auto s = 1e-8;
	return(fabs(element[0]) < s && fabs(element[1]) < s && fabs(element[2]) < s );
  }
 private:
  T element[3];
};
template<std::floating_point T>
Vec3<T> operator-(const Vec3<T> &u, const Vec3<T> &v) {
  return Vec3<T>(u.element[0] - v.element[0], u.element[1] - v.element[1], u.element[2] - v.element[2]);
}
template<std::floating_point T>
Vec3<T> operator*(T val, const Vec3<T> &vec) {
  return Vec3(val * vec.element[0], val * vec.element[1], val * vec.element[2]);
}
template<std::floating_point T>
Vec3<T> operator*(const Vec3<T> &vec, T val) {
  return Vec3(vec.element[0] * val, vec.element[1] * val, vec.element[2] * val);
}

template<std::floating_point T>
Vec3<T> operator+(const Vec3<T> &u, const Vec3<T> &v) {
  return Vec3<T>(u.element[0] + v.element[0], u.element[1] + v.element[1], u.element[2] + v.element[2]);
}
template<std::floating_point T>
Vec3<T> operator+(const Vec3<T> &u, T val){
  return Vec3<T>(u.element[0] + val, u.element[1] + val, u.element[2] + val);
}

template<std::floating_point T>
Vec3<T> operator*(const Vec3<T> &u, const Vec3<T> &v) {
  return Vec3<T>(u.element[0] * v.element[0], u.element[1] * v.element[1], u.element[2] * v.element[2]);
}

template<std::floating_point T>
Vec3<T> operator/(const Vec3<T> &vec, T val) {
  return Vec3(vec.element[0] / val, vec.element[1] / val, vec.element[2] / val);
}

template<std::floating_point T>
Vec3<T> reflect(const Vec3<T>& v, const Vec3<T>& normal){
  return v - 2*v.dot(normal)*normal;
}
template<std::floating_point T>
Vec3<T> refract(const Vec3<T>& uv, const Vec3<T>& n, T etai_over_etat) {
  T cos_theta = fmin((-uv).dot(n), 1.0);
  Vec3<T> r_out_perp =  etai_over_etat * (uv + cos_theta*n);
  T val = std::sqrt(fabs(T(1.0) - r_out_perp.squaredNorm()));
  Vec3<T> r_out_parallel = (-val) * n;
  return r_out_perp + r_out_parallel;
}
using Vec3d = Vec3<double>;
using Vec3f = Vec3<float>;
using Vec3r = Vec3<Real>;

#endif //RAYTRACING__VEC3_H_
