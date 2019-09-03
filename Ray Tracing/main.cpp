#include "Renderer.h"
#include "Sphere.h"
#include "Plane.h"
int main() {
	uint32_t width = 1600;
	uint32_t height = 900;
	const char* fileName = "Rendered-Image.ppm";
	Renderer renderer(width, height,Vector3(0,0,5));
	renderer.backgroundColor(Vector3(0.705f, 0.858f, 0.980f));;

	renderer.addModel(new Sphere(Vector3(-2, 0, -3), 1, Material(Vector3(1),Vector3(0.529f, 0.850f, 1.0f),32)));
	renderer.addModel(new Sphere(Vector3(2, 0, -5), 1, Material(Vector3(1),Vector3(0.282f, 0.941f, 0.733f),32)));
	renderer.addModel(new Sphere(Vector3(0, -1, -7), 1, Material(Vector3(1),Vector3(0.960f, 0.337f, 0.419f),32)));

	renderer.addModel(new Plane(Vector3(0,-5,0), Vector3(0, 1, 0), Material(Vector3(1), Vector3(0.474, 0.588, 0.6), 32)));
	renderer.addModel(new Plane(Vector3(-10, 0, 0), Vector3(1, 0, 0), Material(Vector3(1), Vector3(0.474, 0.588, 0.6), 32)));
	renderer.addModel(new Plane(Vector3(10, 0, 0), Vector3(-1, 0, 0), Material(Vector3(1), Vector3(0.474, 0.588, 0.6), 32)));
	renderer.addModel(new Plane(Vector3(0, 0, -20), Vector3(0, 0, 1), Material(Vector3(1), Vector3(0.474, 0.588, 0.6), 2 << 7)));

	renderer.addLight(new Light(6, 10, 0, 0.5f));
	renderer.addLight(new Light(-6, 13, 0, 0.5f));

	renderer.start();
	renderer.output(fileName);
	int result = system(fileName);
	return 0;
}