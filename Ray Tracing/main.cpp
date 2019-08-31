#include "Renderer.h"
#include "Sphere.h"
int main() {
	uint32_t width = 500;
	uint32_t height = 500;
	const char* fileName = "Rendered-Image.ppm";
	Renderer renderer(width, height);

	renderer.addObject(new Sphere(Vector3f(0,0,-3), 1));

	renderer.start();
	renderer.output(fileName);
	int result = system(fileName);
	return 0;
}