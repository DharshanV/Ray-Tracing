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
				float dirX = (2.0f * (i + 0.5f) / (float)width - 1)*tan(fov / 2.0f)*width / (float)height;
				float dirY = -(2.0f * (j + 0.5f) / (float)height - 1)*tan(fov / 2.0f);
				Vector3 dir = Vector3(dirX, dirY, -1).getNormalized();
				buffer[index(i, j)] = castRay(rayOrigin, dir, objects, lights);
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
			return Vector3(0.2f, 0.3f, 0.3f);
		}

		float diffuseIntensity = 0;
		float specularIntensity = 0;
		for (CLight* l : lights) {
			Vector3 lightToPoint = (*l->getPosition() - hitPoint).getNormalized();
			Vector3 reflectedLight = -reflect(-lightToPoint, N).getNormalized();
			diffuseIntensity += (l->getIntensity() * max(0.0f, N.dot(lightToPoint)));
			specularIntensity += (l->getIntensity() *
								  powf(max(0.0f, reflectedLight.dot(dir)), hitMaterial.specular()));
		}
		return hitMaterial.diffuse() * diffuseIntensity + Vector3(1) * specularIntensity;
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
		return (maxDistance < 1000);
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

	Vector3 reflect(CVector3& A, CVector3& B) {
		return A + (B * -2) * (B.dot(A));
	}
public:
	void addModel(CModel* s) {
		objects.push_back(s);
	}
	void addLight(CLight* l) {
		lights.push_back(l);
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
private:
	struct Timer { clock_t start, stop; };
	Timer timer;
private:
	vector<Vector3> buffer;
	VModel objects;
	VLight lights;
	Vector3 rayOrigin;
private:
	float fov;
	bool rendererStarted;
	uint width, height;
};

#endif // !RENDERER_H
