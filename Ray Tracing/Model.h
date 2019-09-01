#pragma once
#include "Vector3.h"
#include "Material.h"
class Model {
public:
	virtual bool rayIntersect(const Vector3& origin, 
				const Vector3& dir, float& t) const = 0;
	virtual const Material* getMaterial() const = 0;
	virtual Vector3 getNormal(const Vector3& hit) const = 0;
};