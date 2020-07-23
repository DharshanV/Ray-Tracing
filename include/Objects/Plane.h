#ifndef PLANE_H
#define PLANE_H

#include "Object.h"
#include "Vector3.h"

#define MAX_DISTANCE 1000.0f
typedef Vector3<float> vec3f;

class Plane : public Object {
 public:
  Plane(const vec3f& center, const vec3f& N, float width, float height,
        const Material& mat)
      : p0(center), N(N), width(width), height(height), material(mat) {}
  Plane(const vec3f& center, const vec3f& N, float width, float height)
      : p0(center), N(N), width(width), height(height) {}

  bool rayIntersect(const vec3f& origin, const vec3f& dir, float& t) const {
    float denom = dir.dot(N);
    if (fabs(denom) > .0001f) {
      t = ((p0 - origin).dot(N)) / denom;
      // get our point on the plane
      vec3f hitPoint = origin + dir * t;
      vec3f p0ToHit = p0 - hitPoint;
      // check if that point is out of bounds
      if (fabs(p0ToHit[0]) > width || fabs(p0ToHit[2]) > height) return false;
      return t >= 0;
    }
    return false;
  }

  const Material* getMaterial() const { return &material; }
  vec3f getNormal(const vec3f& hit) const { return N; }

 private:
  vec3f p0, N;
  Material material;
  float width, height;
};
#endif