#include "Renderer.h"
#include "Sphere.h"
#include "Plane.h"

int main() {
	uint32_t width = 1600;
	uint32_t height = 900;
	const char* fileName = "Rendered-Image.ppm";
	Renderer renderer(width, height,Vector3(0,0,0));
	renderer.backgroundColor({"Skybox\\right.jpg","Skybox\\left.jpg" ,"Skybox\\bottom.jpg" ,
							 "Skybox\\top.jpg" ,"Skybox\\front.jpg" ,"Skybox\\back.jpg" });

	Material ivory(Vector3(0.6f, 0.3f, 0.1f), Vector3(0.4f, 0.4f, 0.3f), 200);
	Material redRubber(Vector3(0.9f, 0.1f, 0.0f),Vector3(0.3f, 0.1f, 0.1f),10);
	Material mirror(Vector3(0.0f, 10.0f, 0.8f), Vector3(1.0f), 1425.);
	Material plane(Vector3(.4f,.3f,0.0f), Vector3(0.4f, 0.4f, 0.3f), 10);

	renderer.addModel(new Sphere(Vector3(-2.0f, 0.5f, -8.0f), 1, ivory));
	renderer.addModel(new Sphere(Vector3(-2.0f, 1.0f, -3.0f), 1, redRubber));
	renderer.addModel(new Sphere(Vector3(0.5,0.0f, -9.0f), 2, mirror));
	renderer.addModel(new Sphere(Vector3(4.0f, 3.0f, -14.0f), 4, mirror));
	renderer.addModel(new Plane(Vector3(0.0f,-2,-13.0f),Vector3(0,1,0),10,10, mirror));

	renderer.addLight(new Light(Vector3(-20.0f, 20.0f, 20.0f), 1.5f));
	renderer.addLight(new Light(Vector3(30.0f, 50.0f, -25.0f), 1.8f));
	renderer.addLight(new Light(Vector3(30.0f, 20.0f, 30.0f), 1.7f));

	renderer.start();
	renderer.output(fileName);
	int result = system(fileName);
	return 0;
}