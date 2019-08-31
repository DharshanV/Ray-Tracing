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
#include "Sphere.h"
using namespace std;

struct Timer
{
	clock_t start;
	clock_t stop;
};

class Renderer {
public:
	Renderer(float width, float height) : width(width), height(height) {
		rendererStarted = false;
		buffer.resize(width*height);
		rayOrigin = Vector3f(0);
	}
	Renderer(float width, float height,Vector3f eye) : width(width), height(height) {
		rendererStarted = false;
		buffer.resize(width*height);
		rayOrigin = eye;
	}
	~Renderer() { }
public:
	bool start() {
		rendererStarted = true;
		timer.start = clock();
		DEBUG("PUTTING IN BUFFER");
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				float dirX = (2 * (y + 0.5) / (float)width - 1)*tan(fov / 2.)*width / (float)height;
				float dirY = -(2 * (x + 0.5) / (float)height - 1)*tan(fov / 2.);
				Vector3f dir = Vector3f(dirX, dirY, -1).getNormalized();
				buffer[index(x, y)] = castRay(rayOrigin, dir, objects);
			}
		}
		DEBUG("BUFFER FILLED");
		return true;
	}

	Vector3f castRay(const Vector3f& origin, const Vector3f& dir,const vector<Sphere>& objects) {
		if (sceneIntersect(origin,dir, objects)) {
			return Vector3f(0.2, 0.7, 0.8);
		}
		return Vector3f(0.2f, 0.3f, 0.3f);
	}

	bool sceneIntersect(const Vector3f& origin, const Vector3f& dir, const vector<Sphere>& objects) {
		float maxDistance = numeric_limits<float>::max();
		for (const Sphere& s : objects) {
			float distanceI;
			if (s.rayIntersect(origin, dir, distanceI) && distanceI < maxDistance) {
				maxDistance = distanceI;
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
		for (int i = 0; i < width*height; ++i) {
			for (int j = 0; j < 3; j++) {
				out << (char)(255 * max(0.0f,min(1.0f,buffer[i][j])));
			}
		}
		out.close();
		DEBUG("OUTPUT FINISHED");
		timer.stop = clock();
		DEBUG("ELAPSED TIME: " + to_string(timer.stop - timer.start) + "ms");
		return true;
	}
public:
	void addObject(const Sphere& s) {
		objects.push_back(s);
	}
private:
	int index(int x, int y) {
		return x * height + y;
	}
private:
	Timer timer;
	bool rendererStarted;
	float width, height;
	float fov = 45.0f;
	vector<Vector3f> buffer;
	vector<Sphere> objects;
	Vector3f rayOrigin;
};

#endif // !RENDERER_H
