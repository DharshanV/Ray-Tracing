#pragma once
#include "Vector3f.h"
class Material {
public:
	Material() : diffuseColor(0) { }
	Material(const Material& m) : diffuseColor(m.diffuseColor), specularExpo(m.specularExpo){}
	Material(const Vector3f& diffuse) : diffuseColor(diffuse) { }
	Material(float r,float g,float b,float specular) : diffuseColor(r,g,b), specularExpo(specular) { }
	const Vector3f diffuse() { return diffuseColor; }
	const float specular() { return specularExpo; }
private:
	Vector3f diffuseColor;
	float specularExpo;
};