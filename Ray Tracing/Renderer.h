#ifndef RENDERER_H
#define RENDERER_H
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm> 
#include <limits>
#include <time.h>
#include <string>
#include "Debug.h"
#include "Model.h"
#include "Light.h"
using namespace std;

typedef uint32_t uint;
typedef const Vector3 CVector3;
typedef const Light CLight;
typedef const Model CModel;
typedef const Material CMaterial;
typedef vector<const Light*> VLight;
typedef vector<const Model*> VModel;
typedef const vector<const Light*> CVLight;
typedef const vector<const Model*> CVModel;

class Renderer {
public:
	Renderer(uint width, uint height, Vector3 eye = Vector3(0), float fov = 45.0f) :
		width(width), height(height), rayOrigin(eye), fov(fov) {
		rendererStarted = false;
		buffer.resize(width*height);
	}
	~Renderer() {
		clearObjects();
	}
public:
	bool start() {
		rendererStarted = true;
		timer.start = clock();
		DEBUG("PUTTING IN BUFFER");
		for (uint j = 0; j < height; j++) {
			for (uint i = 0; i < width; i++) {
				buffer[index(i, j)] = castRay(rayOrigin, getRay(i,j), objects, lights);
			}
		}
		DEBUG("BUFFER FILLED");
		timer.stop = clock();
		return true;
	}

	Vector3 castRay(CVector3& origin, CVector3& dir, CVModel& objects, CVLight& lights) {
		Vector3 hitPoint, N;
		Material hitMaterial;
		if (!sceneIntersect(origin, dir, hitPoint, N, hitMaterial, objects)) {
			return background;
		}

		float diffuseIntensity = 0;
		float specularIntensity = 0;
		for (CLight* l : lights) {
			Vector3 lightToPoint = (*l->getPosition() - hitPoint).getNormalized();
			Vector3 reflectedLight = reflect(-lightToPoint, N).getNormalized();

			if (inShadow(l, hitPoint,N, lightToPoint, objects)) continue;

			diffuseIntensity += getDiffuse(l, N, lightToPoint);
			specularIntensity += getSpecular(l, reflectedLight, dir, hitMaterial);
		}
		return hitMaterial.diffuse() * diffuseIntensity * hitMaterial.albedo()[0] +
				Vector3(1) * specularIntensity * hitMaterial.albedo()[1];
	}

	bool sceneIntersect(CVector3& origin, CVector3& dir, Vector3& hitPoint,
						Vector3& N, Material& hitMaterial, CVModel& objects) {
		float maxDistance = FLT_MAX;
		for (CModel* o : objects) {
			float distanceI;
			if (o->rayIntersect(origin, dir, distanceI) && distanceI < maxDistance) {
				maxDistance = distanceI;
				hitPoint = origin + dir * distanceI;
				N = o->getNormal(hitPoint).getNormalized();
				hitMaterial = Material(*o->getMaterial());
			}
		}
		return maxDistance < 1000;
	}

	bool output(const char* fileName) {
		if (!rendererStarted) { DEBUG("RENDER NOT STARTED");  return false; }
		DEBUG("OUTPUT STARTED");
		ofstream out;
		out.open(fileName, std::ofstream::out | std::ofstream::binary);
		out << "P6\n" << width << " " << height << "\n255\n";
		char* outputBuffer = new char[width*height * 3];
		for (uint y = 0; y < height; y++) {
			for (uint x = 0; x < width; x++) {
				uint colorIndex = index(x, y);
				for (int i = 0; i < 3; i++) {
					outputBuffer[colorIndex * 3 + i] =
						(char)(255 * max(0.01f, min(1.0f, buffer[colorIndex][i])));
				}
			}
		}
		out << outputBuffer;
		out.close();
		delete outputBuffer;
		DEBUG("OUTPUT FINISHED");
		DEBUG("ELAPSED TIME: " + to_string(timer.stop - timer.start) + "ms");
		return true;
	}

	void addModel(CModel* s) {
		objects.push_back(s);
	}

	void addLight(CLight* l) {
		lights.push_back(l);
	}

	void backgroundColor(CVector3& color) {
		background = color;
	}
private:
	uint index(uint i, uint j) {
		return i + j * width;
	}

	void clearObjects() {
		DEBUG("DELETING OBJECTS");
		for (const Model* o : objects) {
			if (o != NULL) { delete o; o = NULL; }
		}
		for (const Light* i : lights) {
			if (i != NULL) { delete i; i = NULL; }
		}
		DEBUG("CLEARED OBJECTS");
	}

	Vector3 reflect(CVector3& A, CVector3& B) {
		return 2 * ( (B.dot(A)) * B ) - A;
	}

	Vector3 getRay(uint i,uint j) {
		float dirX = (2.0f * (i + 0.5f) / (float)width - 1)*tan(fov / 2.0f)*width / (float)height;
		float dirY = -(2.0f * (j + 0.5f) / (float)height - 1)*tan(fov / 2.0f);
		return Vector3(dirX, dirY, -1).getNormalized();
	}

	float getDiffuse(CLight* l,CVector3& N,CVector3& light) {
		return l->getIntensity() * max(0.0f, N.dot(light));
	}
	
	float getSpecular(CLight* l,CVector3& R,CVector3& E,CMaterial& mat) {
		return l->getIntensity() * powf(max(0.0f, R.dot(E)), mat.specular());
	}

	bool inShadow(CLight* l, CVector3& rayHit, CVector3& hitN, CVector3& dirToLight, CVModel& objects) {
		Vector3 hitPoint, N, origin;
		Material hitMaterial;
		float lightDistance = (origin - *l->getPosition()).length();
		origin = (dirToLight.dot(hitN) >= 0) ? rayHit + hitN * .001f : rayHit - hitN * .001f;
		if (sceneIntersect(origin, dirToLight, hitPoint, N, hitMaterial, objects) &&
			(hitPoint - origin).length() < lightDistance) {
			return true;
		}
		return false;
	}
private:
	struct Timer { clock_t start, stop; };
	vector<Vector3> buffer;
	bool rendererStarted;
	Vector3 rayOrigin;
	Vector3 background;
	uint width, height;
	VModel objects;
	VLight lights;
	Timer timer;
	float fov;
};

#endif // !RENDERER_H
