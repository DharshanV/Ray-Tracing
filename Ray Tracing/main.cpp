#include "Renderer.h"

int main() {
	float width = 500;
	float height = 500;
	const char* fileName = "Rendered-Image.ppm";
	Renderer renderer(width, height);
	renderer.addObject(Sphere(Vector3f(0, 0, -5), 1));
	renderer.start();
	renderer.output(fileName);
	int result = system(fileName);
	return 0;
}