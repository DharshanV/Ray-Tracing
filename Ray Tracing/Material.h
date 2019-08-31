#pragma once
#include "Vector3f.h"
class Material {
public:
	Material() : diffuseColor(0) { }
	Material(const Material& m) : diffuseColor(m.diffuseColor) {}
	Material(const Vector3f& color) : diffuseColor(color) { }
	Material(float r,float g,float b) : diffuseColor(r,g,b) { }
	const Vector3f diffuse() { return diffuseColor; }
private:
	Vector3f diffuseColor;
};