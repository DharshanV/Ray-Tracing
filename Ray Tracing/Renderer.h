#ifndef RENDERER_H
#define RENDERER_H
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm> 
#include <limits>
#include <time.h>
#include <string>
#include "Vector3f.h"
#include "Debug.h"
#include "Model.h"
#include "Light.h"
using namespace std;

class Renderer {
public:
	Renderer(uint32_t width, uint32_t height,Vector3f eye = Vector3f(0),float fov = 45.0f) :
		width(width), height(height), rayOrigin(eye),fov(fov) {
		rendererStarted = false;
		buffer.resize(width*height);
	}
	~Renderer() { clearObjects(); }
public:
	bool start() {
		rendererStarted = true;
		timer.start = clock();
		DEBUG("PUTTING IN BUFFER");
		for (uint32_t j = 0; j < height; j++) {
			for (uint32_t i = 0; i < width; i++) {
				float dirX = (2.0f * (i + 0.5f) / (float)width - 1)*tan(fov / 2.0f)*width / (float)height;
				float dirY = -(2.0f * (j + 0.5f) / (float)height - 1)*tan(fov / 2.0f);
				Vector3f dir = Vector3f(dirX, dirY, -1).getNormalized();
				buffer[index(i, j)] = castRay(rayOrigin, dir, objects, lights);
			}
		}
		DEBUG("BUFFER FILLED");
		timer.stop = clock();
		return true;
	}

	Vector3f castRay(const Vector3f& origin, const Vector3f& dir,
			const vector<const Model*>& objects, const vector<const Light*>& lights) {
		Vector3f hitPoint, N;
		Material hitMaterial;
		if (!sceneIntersect(origin,dir, hitPoint, N, hitMaterial, objects)) {
			return Vector3f(0.2f, 0.3f, 0.3f);
		}

		float lightIntensity = 0;
		for (const Light* l : lights) {
			Vector3f pointToLight = (*l->getPosition()-hitPoint).getNormalized();
			lightIntensity += (l->getIntensity() * max(0.0f,N.dot(pointToLight)));
		}
		return hitMaterial.diffuse() * lightIntensity;
	}

	bool sceneIntersect(const Vector3f& origin, const Vector3f& dir, Vector3f& hitPoint,
						Vector3f& N,Material& hitMaterial, const vector<const Model*>& objects) {
		float maxDistance = numeric_limits<float>::max();
		for (const Model* o : objects) {
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
		for (uint32_t y = 0; y < height; y++) {
			for (uint32_t x = 0; x < width; x++) {
				uint32_t colorIndex = index(x, y);
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
public:
	void addModel(const Model* s) {
		objects.push_back(s);
	}

	void addLight(const Light* l) {
		lights.push_back(l);
	}
private:
	uint32_t index(uint32_t i, uint32_t j) {
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
	vector<Vector3f> buffer;
	vector<const Model*> objects;
	vector<const Light*> lights;
	Vector3f rayOrigin;
private:
	float fov;
	bool rendererStarted;
	uint32_t width, height;
};

#endif // !RENDERER_H
