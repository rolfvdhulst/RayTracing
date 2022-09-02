//
// Created by rolf on 13-8-22.
//

#ifndef RAYTRACING_SRC_CAMERA_H_
#define RAYTRACING_SRC_CAMERA_H_
#include "Vec3.h"
#include "Ray.h"

class Camera{
 public:
  Camera(const Vec3r& lookFrom, const Vec3r& lookAt, const Vec3r& up, Real verticalFOVdegrees, Real aspectRatio,
		 Real aperature,
		 Real focusDist,
		 Real shutterCloseTime){
	Real theta = verticalFOVdegrees*(M_PI/180.0);
	Real h = tan(theta*0.5);
	Real viewportHeight = 2.0*h;
	Real viewportWidth = aspectRatio*viewportHeight;

	w = (lookFrom-lookAt).normalized();
	u = (up.cross(w)).normalized();
	v = w.cross(u);

	origin = lookFrom;
	horizontal = focusDist * viewportWidth * u;
	vertical = focusDist *  viewportHeight * v;
	lowerLeftCorner = origin - Real(0.5)*horizontal-Real(0.5)*vertical - focusDist * w;
	lensRadius = aperature * Real(0.5);
	shutterTime = shutterCloseTime;
  }
  [[nodiscard]] Ray getRay(Real s, Real t, RandomDevice& device) const{
	Vec3r rd = lensRadius * device.randomInUnitDisk();
	Vec3r offset = u * rd.x() + v * rd.y();

	return Ray{.origin = origin  + offset,
				  .direction = lowerLeftCorner + s * horizontal + t*vertical - origin - offset,
				  .timeOffset = device.randomReal(0.0,shutterTime)};
  }
 private:
  Vec3r origin;
  Vec3r lowerLeftCorner;
  Vec3r horizontal;
  Vec3r vertical;
  Vec3r u,v,w;
  Real lensRadius;
  Real shutterTime;
};
#endif //RAYTRACING_SRC_CAMERA_H_
