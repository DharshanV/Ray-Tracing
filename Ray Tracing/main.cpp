#include "Renderer.h"

int main() {
	float width = 100;
	float height = 100;
	Renderer renderer(width, height);
	renderer.start();
	renderer.ouput("Rendered Image.ppm");
	return 0;
}