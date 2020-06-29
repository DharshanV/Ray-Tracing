#ifndef RENDERING_UTILITIES
#define RENDERING_UTILITIES
#include <iostream>

#include "Vector3.h"

#define PI 3.14159f
typedef Vector3<float> vec3f;

vec3f getRayDirection(uint x, uint y, uint width, uint height, uint fov) {
    vec3f rayDirection;
    float aspectRatio = (float)width / height;

    //shift to middle, normalize, and screen space
    float xPrime = (2.0f * ((x + 0.5f) / width) - 1) * aspectRatio *
                   tan((fov / 2.0f) * (PI / 180.0f));
    float yPrime = (1 - 2.0f * ((y + 0.5f) / height) *
                            tan((fov / 2.0f) * (PI / 180.0f)));

    //get ray direction towards negative z-axis
    vec3f cameraOrigin(0);
    rayDirection = vec3f(xPrime, yPrime, -1) - cameraOrigin;
    return rayDirection.getNormalized();
}

void writeColor(ofstream& out, const vec3f& color) {
    for (int i = 0; i < 3; i++) {
        out << uint(255 * max(0.01f, min(1.0f, color[i]))) << " ";
    }
}

#endif