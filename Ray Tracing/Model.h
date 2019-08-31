#pragma once
#include "Vector3f.h"
#include "Material.h"
class Model {
public:
	virtual bool rayIntersect(const Vector3f& origin, 
				const Vector3f& dir, float& t) const = 0;
	virtual const Material* getMaterial() const = 0;
	virtual Vector3f getNormal(const Vector3f& hit) const = 0;
};