#include "Renderer.h"
#include "Sphere.h"
#include "Plane.h"

int main() {
	uint32_t width = 1600;
	uint32_t height = 900;
	const char* fileName = "Rendered-Image.ppm";

	Renderer renderer(width, height,Vector3(0,.25,0));
	renderer.backgroundColor({ "Skybox\\px.png","Skybox\\nx.png" ,"Skybox\\ny.png" ,
					 "Skybox\\py.png" ,"Skybox\\pz.png" ,"Skybox\nz.png" });

	Material redMarble(Vector3(0.6f, 0.3f, 0.5f), Vector3(0.8, 0.231, 0.231), 200);
	Material greyMarble(Vector3(0.6,0.3f, 0.1f), Vector3(0.850, 0.843, 0.843), 200);
	Material purpleMarble(Vector3(0.6, 0.3f, 0.1f), Vector3(0.686, 0.329, 0.890), 200);
	Material greenMarble(Vector3(0.6, 0.3f, 0.1f), Vector3(0.407, 0.968, 0.482), 200);
	Material blueMarble(Vector3(0.6, 0.3f, 0.1f), Vector3(0.407, 0.529, 0.968), 200);
	Material planeMat(Vector3(0.6f, 0.3f, 0.45f), Vector3(0.509, 0.509, 0.509), 350);
	Material mirror(Vector3(0.0f, 10.0f, 0.8f), Vector3(1.0f), 1425.);

	renderer.addModel(new Sphere(Vector3(2,0.15, -6), 1.5, redMarble));
	renderer.addModel(new Sphere(Vector3(-0.75, 0, -5.5), 1, greyMarble));
	renderer.addModel(new Sphere(Vector3(0.5, -0.6, -3.5), 0.4, purpleMarble));
	renderer.addModel(new Sphere(Vector3(1.75, -0.6, -3.5), 0.4, greenMarble));
	renderer.addModel(new Sphere(Vector3(-2.3, -0.3, -3.5), 0.7, blueMarble));
	renderer.addModel(new Sphere(Vector3(-2.2, -0.6, -5.5), 0.4, redMarble));
	renderer.addModel(new Sphere(Vector3(-3.35, 0.5, -8), 2, mirror));

	renderer.addModel(new Plane(Vector3(0, -1, -5), Vector3(0, 1, 0), 20, 20, planeMat));

	renderer.addLight(new Light(Vector3(-7, 7, 3), 1.5f));

	renderer.start();
	renderer.output(fileName);
	system(fileName);
	return 0;
}