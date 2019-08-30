#ifndef RENDERER_H
#define RENDERER_H
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm> 
#include <limits>
#include "Vector3f.h"
#include "Debug.h"

using namespace std;
class Renderer {
public:
	Renderer(float width, float height) : width(width), height(height) {
		rendererStarted = false;
		buffer.resize(width*height);
	}
	~Renderer() { }
public:
	bool start() {
		rendererStarted = true;
		DEBUG("PUTTING IN BUFFER");
		for (int x = 0; x < height; x++) {
			for (int y = 0; y < width; y++) {
				buffer[y+x*width] = Vector3f(x/height, y/(float)width, 0);
			}
		}
		DEBUG("BUFFER FILLED");
		return true;
	}
	bool ouput(const char* fileName) {
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
		return true;
	}
private:
	int index(int x, int y) {
		return x * height + y;
	}
private:
	bool rendererStarted;
	float width, height;
	vector<Vector3f> buffer;
};

#endif // !RENDERER_H
