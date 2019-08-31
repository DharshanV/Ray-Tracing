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
#include "Object.h"
using namespace std;

struct Timer
{
	clock_t start;
	clock_t stop;
};

class Renderer {
public:
	Renderer(unsigned int width, unsigned int height) : width(width), height(height) {
		rendererStarted = false;
		buffer.resize(width*height);
		rayOrigin = Vector3f(0);
	}
	Renderer(unsigned int width, unsigned int height,Vector3f eye) : width(width), height(height) {
		rendererStarted = false;
		buffer.resize(width*height);
		rayOrigin = eye;
	}
	~Renderer() { clearObjects(); }
public:
	bool start() {
		rendererStarted = true;
		timer.start = clock();
		DEBUG("PUTTING IN BUFFER");
		for (unsigned int x = 0; x < width; x++) {
			for (unsigned int y = 0; y < height; y++) {
				float dirX = (2.0f * (y + 0.5f) / (float)width - 1)*tan(fov / 2.0f)*width / (float)height;
				float dirY = -(2.0f * (x + 0.5f) / (float)height - 1)*tan(fov / 2.0f);
				Vector3f dir = Vector3f(dirX, dirY, -1.0f).getNormalized();
				buffer[index(x, y)] = castRay(rayOrigin, dir, objects);
			}
		}
		DEBUG("BUFFER FILLED");
		return true;
	}

	Vector3f castRay(const Vector3f& origin, const Vector3f& dir,const vector<const Object*>& objects) {
		if (sceneIntersect(origin,dir, objects)) {
			return Vector3f(0.2f, 0.7f, 0.8f);
		}
		return Vector3f(0.2f, 0.3f, 0.3f);
	}

	bool sceneIntersect(const Vector3f& origin, const Vector3f& dir, const vector<const Object*>& objects) {
		float maxDistance = numeric_limits<float>::max();
		for (const Object* o : objects) {
			float sDistance;
			if (o->rayIntersect(origin, dir, sDistance) && sDistance < maxDistance) {
				maxDistance = sDistance;
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
		for (unsigned int i = 0; i < width*height; ++i) {
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
	void addObject(const Object* s) {
		objects.push_back(s);
	}
private:
	int index(int x, int y) {
		return x * height + y;
	}

	void clearObjects() {
		DEBUG("DELETING OBJECTS");
		for (const Object* o : objects) {
			if (o != NULL) { delete o; o = NULL; }
		}
		DEBUG("OBJECTS CLEARED");
	}
private:
	Timer timer;
	bool rendererStarted;
	unsigned int width, height;
	float fov = 45.0f;
	vector<Vector3f> buffer;
	vector<const Object*> objects;
	Vector3f rayOrigin;
};

#endif // !RENDERER_H
