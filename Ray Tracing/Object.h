#ifndef OBJECT_H
#define OBJECT_H
#include "Vector3f.h"
class Object {
public:
	virtual bool rayIntersect(const Vector3f& origin, const Vector3f& dir, float& t) const = 0;
};

#endif // !OBJECT_H
