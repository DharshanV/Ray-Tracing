#include "Renderer.h"
#include "Sphere.h"
int main() {
	float width = 500;
	float height = 500;
	const char* fileName = "Rendered-Image.ppm";
	Renderer renderer(width, height);

	renderer.addObject(new Sphere(Vector3f(-2, 0, -5), .5));
	renderer.addObject(new Sphere(Vector3f(2, 0, -5), .5));

	renderer.start();
	renderer.output(fileName);
	int result = system(fileName);
	return 0;
}