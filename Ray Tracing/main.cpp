#include "Renderer.h"
#include "Sphere.h"

int main() {
	uint32_t width = 500;
	uint32_t height = 500;
	const char* fileName = "Rendered-Image.ppm";
	Renderer renderer(width, height,Vector3(0,0,3));

	renderer.addModel(new Sphere(Vector3(-2, 0, -3), 1, Material(0.529f, 0.850f, 1.0f,32)));
	renderer.addModel(new Sphere(Vector3(2, 0, -5), 1, Material(0.282f, 0.941f, 0.733f,32)));
	renderer.addModel(new Sphere(Vector3(0, -1, -7), 1, Material(0.960f, 0.337f, 0.419f,32)));
	
	renderer.addLight(new Light(20.0f, 20.0f, 20.0f, 0.7f));
	renderer.addLight(new Light(-20.0f, 20.0f, 20.0f, .6f));

	renderer.start();
	renderer.output(fileName);
	int result = system(fileName);
	return 0;
}