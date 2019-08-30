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
	}
	~Renderer() { }
public:
	bool start() {
		rendererStarted = true;
		timer.start = clock();
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
		timer.stop = clock();
		DEBUG("ELAPSED TIME: " + to_string(ceil((timer.stop - timer.start)/1000)) + "s");
		return true;
	}
private:
	int index(int x, int y) {
		return x * height + y;
	}
private:
	Timer timer;
	bool rendererStarted;
	float width, height;
	vector<Vector3f> buffer;
};

#endif // !RENDERER_H
