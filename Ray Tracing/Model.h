#pragma once
#include "Vector3f.h"
class Model {
public:
	virtual bool rayIntersect(const Vector3f& origin, 
				const Vector3f& dir, float& t) const = 0;
};