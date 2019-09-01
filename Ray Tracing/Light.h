#pragma once
#include "Vector3.h"
class Light {
public:
	Light(const Vector3& position,float intensity) : 
		position(position),intensity(intensity) { }
	Light(float x,float y,float z, float intensity) :
		position(x,y,z), intensity(intensity) { }
	~Light() {}

	const Vector3* getPosition() const { return &position; }
	float getIntensity() const { return intensity; }
private:
	Vector3 position;
	float intensity;
};