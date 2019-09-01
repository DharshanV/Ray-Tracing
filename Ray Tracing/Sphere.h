#ifndef SPHERE_H
#define SPHERE_H
#include "Model.h"
class Sphere : public Model {
public:
	Sphere() : position(0), radius(1) {}
	Sphere(Vector3 position) : position(position), radius(1) {}
	Sphere(Vector3 position,float r,Material mat) :
			position(position), radius(r), material(mat) {}
	~Sphere() {}
public:
	bool rayIntersect(const Vector3& origin, const Vector3& dir,float& t) const {
		Vector3 oToC = position - origin;
		float t1 = dir.dot(oToC);					//t1 when ray is closest to sphere
		float rayToC = oToC.dot(oToC) - t1 * t1;
		if (rayToC > radius) return false;
		float deltaT = sqrt(radius*radius - rayToC);
		t = t1 - deltaT;							//below point
		float abovePoint = t1 + deltaT;
		//check if the origin is inside left of center
		//if it is then t will be zero since will have to travel back to meet below point
		if (t < 0) t = abovePoint;
		//if that the above point is still negative then sphere is behind origin
		if (t < 0) return false;
		return true;
	}

	const Material* getMaterial() const { return &material; }

	Vector3 getNormal(const Vector3& hit) const {
		return hit - position;
	}
private:
	Material material;
	Vector3 position;
	float radius;
};

#endif // SPHERE_H
