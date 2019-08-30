#ifndef RENDERER_H
#define RENDERER_H
#include <iostream>
#include <fstream>
#include "Vector3f.h"
#include "Debug.h"

using namespace std;
class Renderer {
public:
	Renderer(float width, float height) : width(width), height(height) {
		rendererStarted = false;
	}
	~Renderer() { }
public:
	void start() {
		rendererStarted = true;
	}
	void ouput(const char* fileName) {
		if (!rendererStarted) { DEBUG("RENDER NOT STARTED");  return; }
	}
private:
	bool rendererStarted;
	float width, height;
};

#endif // !RENDERER_H
