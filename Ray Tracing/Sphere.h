#ifndef SPHERE_H
#define SPHERE_H
#include "Model.h"
class Sphere : public Model {
public:
	Sphere() : position(0), radius(1) {}
	Sphere(Vector3f position) : position(position), radius(1) {}
	Sphere(Vector3f position,float r) : position(position), radius(r) {}
	~Sphere() {}
public:
	bool rayIntersect(const Vector3f& origin, const Vector3f& dir,float& t) const {
		Vector3f oToC = position - origin;
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
private:
	Vector3f position;
	float radius;
};

#endif // SPHERE_H
