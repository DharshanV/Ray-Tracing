#include "Renderer.h"
#include "Sphere.h"

int main() {
	uint32_t width = 500;
	uint32_t height = 500;
	const char* fileName = "Rendered-Image.ppm";
	Renderer renderer(width, height,Vector3f(0,0,3));

	renderer.addModel(new Sphere(Vector3f(-2, 0, -3), 1, Material(0.4f,0.4f,0.3f,32)));
	renderer.addModel(new Sphere(Vector3f(2, 0, -5), 1, Material(0.3f, 0.2f, 0.6f,32)));
	renderer.addModel(new Sphere(Vector3f(0, -1, -7), 1, Material(0.6f, 0.2f, 0.3f,32)));
	
	renderer.addLight(new Light(20.0f, 20.0f, 20.0f, 0.7f));
	renderer.addLight(new Light(-20.0f, 20.0f, 20.0f, 1.5f));

	renderer.start();
	renderer.output(fileName);
	int result = system(fileName);
	return 0;
}