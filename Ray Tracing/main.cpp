#include "Renderer.h"
#include "Sphere.h"
#include "Plane.h"
int main() {
	uint32_t width = 500;
	uint32_t height = 500;
	const char* fileName = "Rendered-Image.ppm";
	Renderer renderer(width, height,Vector3(0));
	renderer.backgroundColor(Vector3(0.2, 0.7, 0.8));

	Material mirror(Vector3(0.0, 10.0, 0.8), Vector3(1), 1425.0f);
	Material ivory(Vector3(0.6, 0.3,.2), Vector3(0.4, 0.4, 0.3), 50);
	Material planeMat(Vector3(.3,0, 0), Vector3(0.909, 0.619, 0.619), 10);
	Material red_rubber(Vector3(0.9, 0.1,0), Vector3(0.3, 0.1, 0.1), 10.);

	renderer.addModel(new Sphere(Vector3(-2, 0, -6), 1, ivory));
	renderer.addModel(new Sphere(Vector3(0, 2, -10), 3, mirror));
	renderer.addModel(new Sphere(Vector3(0, -2, -15), 4, mirror));
	renderer.addModel(new Sphere(Vector3(2, 0, -6), 1, red_rubber));
	renderer.addModel(new Plane(Vector3(0, -7, 0), Vector3(0, 1, 0), planeMat));

	renderer.addLight(new Light(-20, 20, 20, 1.5f));
	renderer.addLight(new Light(30, 50, -25, 1.8f));
	renderer.addLight(new Light(30, 20, 30, 1.7f));

	renderer.start();
	renderer.output(fileName);
	int result = system(fileName);
	return 0;
}