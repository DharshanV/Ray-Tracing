#ifndef PLANE_H
#define PLANE_H
#include "Model.h"
typedef const Material CMaterial;
typedef const Vector3 CVector3;

class Plane : public Model{
public:
	Plane(CVector3& center, CVector3& N,CMaterial& mat) : p0(center), N(N),material(mat) {

	}
	bool rayIntersect(const Vector3& origin, const Vector3& dir, float& t) const {
		float denom = dir.dot(N);
		if (fabs(denom) > .001) {
			Vector3 hitToP0 = p0 - origin;
			hitToP0 /= denom;
			t = hitToP0.dot(N);
			return (t >= 0 && t <= 100);
		}
		return false;
	}

	const Material* getMaterial() const {
		return &material;
	}
	Vector3 getNormal(const Vector3& hit) const {
		return N;
	}

private:
	Vector3 p0, N;
	Material material;
};
#endif // !PLANE_H
