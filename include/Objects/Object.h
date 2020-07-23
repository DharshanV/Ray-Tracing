#ifndef OBJECT_H
#define OBJECT_H

#include "Material.h"
#include "Vector3.h"
typedef Vector3<float> vec3f;

class Object {
 public:
  // Check if given ray intersects a given object
  // and if it does return true and modifie the t value
  virtual bool rayIntersect(const vec3f& origin, const vec3f& dir,
                            float& t) const = 0;
  virtual const Material* getMaterial() const = 0;
  virtual vec3f getNormal(const vec3f& hit) const = 0;
};
#endif