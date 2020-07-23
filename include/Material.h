#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vector3.h"
typedef Vector3<float> vec3f;

class Material {
 public:
  Material() : albedoColor(1, 0, 0), diffuseColor(0), specularExpo(2) {}
  Material(const Material& m)
      : albedoColor(m.albedoColor),
        diffuseColor(m.diffuseColor),
        specularExpo(m.specularExpo) {}
  Material(const vec3f& a, const vec3f& color, float specular)
      : albedoColor(a), diffuseColor(color), specularExpo(specular) {}

  void setDiffuse(const vec3f& diffuseColor) {
    this->diffuseColor = diffuseColor;
  }
  void setAlbedo(const vec3f& albedoColor) { this->albedoColor = albedoColor; }
  void setSpecular(const float& specular) { this->specularExpo = specular; }

  const vec3f& diffuse() const { return diffuseColor; }
  const vec3f& albedo() const { return albedoColor; }
  const float specular() const { return specularExpo; }

 private:
  vec3f diffuseColor;
  vec3f albedoColor;
  float specularExpo;
};
#endif