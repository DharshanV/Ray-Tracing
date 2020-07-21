#ifndef SPHERE_H
#define SPHERE_H

#include "Object.h"
#include "Vector3.h"

typedef Vector3<float> vec3f;

class Sphere : public Object {
   public:
    Sphere() : position(0), radius(1) {}
    Sphere(const vec3f& position) : position(position), radius(1) {}
    Sphere(const vec3f& position,float r) : position(position), radius(r) {}
    Sphere(const vec3f& position, float r, const Material& mat) : 
        position(position), radius(r), material(mat) {}
    ~Sphere() {}

   public:
    bool rayIntersect(const vec3f& origin, const vec3f& dir, float& t) const {
        vec3f oToC = position - origin;
        float t1 = dir.dot(oToC);  //t1 when ray is closest to sphere
        float rayToC = oToC.dot(oToC) - t1 * t1;
        //Don't need to check for sqrt or radius squared
        //since the inequality stays the same
        if (rayToC > radius) return false;
        float deltaT = sqrt(radius * radius - rayToC);
        t = t1 - deltaT;  //below point
        float abovePoint = t1 + deltaT;
        //check if the origin is inside left of center
        //if it is then t will be zero since will have to travel back to meet below point
        if (t < 0) t = abovePoint;
        //if that the above point is still negative then sphere is behind origin
        if (t < 0) return false;
        return true;
    }

    const Material* getMaterial() const { return &material; }

    vec3f getNormal(const vec3f& hit) const {
        return hit - position;
    }

   private:
    Material material;
    vec3f position;
    float radius;
};

#endif