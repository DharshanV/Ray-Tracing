#pragma once
#include "Vector3.h"
class Material {
public:
	Material() : albedoColor(1,0,0), diffuseColor(0), specularExpo(2) { }
	Material(const Material& m) : albedoColor(m.albedoColor), diffuseColor(m.diffuseColor), specularExpo(m.specularExpo){}
	Material(const Vector3& a,const Vector3& color,float specular) : albedoColor(a), diffuseColor(color), specularExpo(specular) { }

	void setDiffuse(const Vector3& diffuseColor) {
		this->diffuseColor = diffuseColor;
	}
	const Vector3 diffuse() { return diffuseColor; }
	const Vector3 albedo() { return albedoColor; }
	const float specular() const { return specularExpo; }
private:
	Vector3 diffuseColor;
	Vector3 albedoColor;
	float specularExpo;
};