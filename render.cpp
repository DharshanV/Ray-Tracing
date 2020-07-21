#include <fstream>
#include <iostream>

#include "Renderer.h"

using namespace std;

bool validArgs(uint& w, uint& h, uint& splitCount,
               const char*& fileName, int argc, char* argv[]);

int main(int argc, char* argv[]) {
    uint width;
    uint height;
    uint splitCount;
    const char* fileName;

    if (!validArgs(width, height, splitCount, fileName, argc, argv)) {
        return 1;
    }

    Renderer renderer(width, height);

    Material redMarble(vec3f(0.6f, 0.3f, 0.5f), vec3f(0.8, 0.231, 0.231), 200);
    Material greyMarble(vec3f(0.6, 0.3f, 0.1f), vec3f(0.850, 0.843, 0.843), 200);
    Material purpleMarble(vec3f(0.6, 0.3f, 0.1f), vec3f(0.686, 0.329, 0.890), 200);
    Material greenMarble(vec3f(0.6, 0.3f, 0.1f), vec3f(0.407, 0.968, 0.482), 200);
    Material blueMarble(vec3f(0.6, 0.3f, 0.1f), vec3f(0.407, 0.529, 0.968), 200);
    Material planeMat(vec3f(0.6f, 0.3f, 0.45f), vec3f(0.1f), 350);
    Material mirror(vec3f(0.0f, 10.0f, 0.8f), vec3f(1.0f), 1425.);

    renderer.addObject(new Sphere(vec3f(2, 0.5, -8), 3, mirror));
    renderer.addObject(new Sphere(vec3f(-0.75, 0, -5.5), 1, greyMarble));
    renderer.addObject(new Sphere(vec3f(0.5, -0.6, -3.5), 0.4, purpleMarble));
    renderer.addObject(new Sphere(vec3f(1.75, -0.6, -3.5), 0.4, greenMarble));
    renderer.addObject(new Sphere(vec3f(-2.3, -0.3, -3.5), 0.7, blueMarble));
    renderer.addObject(new Sphere(vec3f(-2.2, -0.6, -5.5), 0.4, redMarble));
    renderer.addObject(new Sphere(vec3f(-3.35, 0.5, -8), 2, mirror));

    renderer.addObject(new Plane(vec3f(0, -1, -5), vec3f(0, 1, 0), 20, 20, planeMat));

    renderer.addLight(new Light(vec3f(-7, 7, 3), 1.5f));

    renderer.renderMultithreaded(splitCount);
    renderer.output(fileName);

    return 0;
}

bool validArgs(uint& w, uint& h, uint& splitCount,
               const char*& fileName, int argc, char* argv[]) {
    switch (argc) {
        case 4:
            w = stoi(argv[1]);
            h = stoi(argv[2]);
            splitCount = 0;
            fileName = argv[3];
            return true;
        case 5:
            w = stoi(argv[1]);
            h = stoi(argv[2]);
            splitCount = stoi(argv[3]);
            fileName = argv[4];
            return true;
        default:
            cout << "Invalid arguments" << endl;
            cout << "Expected: <width> <height> <fileName> OR " << endl;
            cout << "          <width> <height> <splitCount> <fileName>" << endl;
            return false;
    }
}