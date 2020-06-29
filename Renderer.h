#ifndef RENDERER_H
#define RENDERER_H

#include <fstream>
#include <iostream>
#include <list>
#include <thread>
#include <vector>

#include "Objects/Light.h"
#include "Objects/Plane.h"
#include "Objects/Sphere.h"
#include "Utility/Cubemap.h"
#include "Utility/Object.h"
#include "Utility/Vector3.h"

using namespace std;

#define PI 3.14159f
#define RENDER_DISTANCE 10000
#define BUFFER_MAX 100
#define MAX_DEPTH 4
typedef Vector3<float> vec3f;

class Renderer {
   public:
    /**
     * Initilizes the Renderer object with width, height, FOV,
     * cameraOrigin at zero, and buffer of width * height.
     *
     * @param[in] width of the image
     * @param[in] height of the image
     * @param[in] fov of the camera
     */
    Renderer(uint width, uint height, vec3f background = vec3f(0.384f, 0.333f, 0.333f));

    Renderer(uint width, uint height, vector<const char*> faces);

    /**
     * Add's object to a list for ray intersection.
     *
     * @param[in] object to add
     */
    void addObject(Object* object);

    void addLight(Light* light);

    /**
     * Renders the final image using the added objects, and 
     * calculating the ray directions, its intersections, and
     * ray bounces. It saves the pixel color in the buffer, in
     * order to output to a file when called.
     */
    void render();

    void renderMultithreaded(uint splitCount = 0);

    static void renderThread(Renderer* r, uint topX, uint topY,
                            uint bottomX, uint bottomY) {
        for (uint y = topY; y < bottomY; y++) {
            for (uint x = topX; x < bottomX; x++) {
                uint bufferIndex = r->index(x, y);
                for (uint sample = 0; sample < r->totalSample; sample++) {
                    float deltaX = r->jitterValues[2 * sample];
                    float deltaY = r->jitterValues[2 * sample + 1];

                    r->buffer[bufferIndex] += r->castRay(r->cameraOrigin,
                                        r->getRay(x + deltaX, y + deltaY), 0);
                }
                r->buffer[bufferIndex] /= r->totalSample;
            }
        }
    }

    /**
     * After render() being called. It outputs the buffer
     * to the given file name. Instead of outputting one pixel
     * value by one, it insteads outputs by a temporary buffer 
     * of size 300 and refills when full.
     *
     * @param[in] file of the name to output to
     */
    void output(const char* fileName);

    /**
     * Deletes all the heap allocated objects
     */
    ~Renderer();

   private:
    /**
     * Maps a 2d index to a 1d index.
     *
     * @param[in] ith position of the 2d array
     * @param[in] jth position of the 2d array
     * @return the index of the 1d array
     */
    uint index(uint i, uint j);

    /**
     * Maps a 2d index to a 1d index.
     *
     * @param[in] ith position of the 2d array
     * @param[in] jth position of the 2d array
     * @return the index of the 1d array
     */
    vec3f getRay(float x, float y);
    vec3f castRay(const vec3f& rayOrigin, const vec3f& rayDir, uint depth = 0);
    bool sceneIntersect(const vec3f& rayOrigin, const vec3f& rayDir,
                        vec3f& hitPoint, vec3f& N, Material& hitMaterial);

    float clip(float n, float lower, float upper);

    float getDiffuse(Light* l, vec3f& N, vec3f& light);

    bool inShadow(Light* l, vec3f& rayHit, vec3f hitN, vec3f& dirToLight);

    vec3f getOriginShift(const vec3f& origin, const vec3f& dir, const vec3f& N);

    vec3f reflect(const vec3f& A, const vec3f& B);

    float getSpecular(Light* l, const vec3f& R, const vec3f& E, const Material& mat);

   private:
    uint width;
    uint height;
    uint FOV = 90;
    bool useCubemap;
    uint totalSample = 3;
    vector<float> jitterValues{0.25f, 0.75f, -0.75f, 0.33f,0.45f};
    vec3f cameraOrigin;
    vec3f background;

    vector<vec3f> buffer;
    list<Object*> objects;
    list<Light*> lights;
    Cubemap cubemap;
};

#endif