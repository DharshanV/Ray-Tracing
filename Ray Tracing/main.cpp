#include "Renderer.h"
#include "Sphere.h"

int main() {
	unsigned int width = 500;
	unsigned int height = 500;
	const char* fileName = "Rendered-Image.ppm";
	Renderer renderer(width, height,Vector3f(0,0,-1));

	renderer.addObject(new Sphere(Vector3f(-2, 0, -5), .5));
	renderer.addObject(new Sphere(Vector3f(2, 0, -5), .5));

	renderer.start();
	renderer.output(fileName);
	int result = system(fileName);
	return 0;
}