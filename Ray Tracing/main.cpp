#include "Renderer.h"

int main() {
	float width = 500;
	float height = 500;
	Renderer renderer(width, height);
	renderer.start();
	renderer.ouput("Rendered Image.ppm");
	return 0;
}