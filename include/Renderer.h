#ifndef RENDERER_H
#define RENDERER_H

#include <fstream>
#include <iostream>
#include <list>
#include <thread>
#include <vector>

#include "Cubemap.h"
#include "Objects/Light.h"
#include "Objects/Object.h"
#include "Objects/Plane.h"
#include "Objects/Sphere.h"
#include "Vector3.h"

#define PI 3.14159f
#define RENDER_DISTANCE 10000
#define BUFFER_MAX 100
#define MAX_DEPTH 4
typedef Vector3<float> vec3f;

class Renderer {
 public:
  /**
   * Initilizes the Renderer object with width, height, FOV,
   * cameraOrigin at zero, and buffer of width * height. It assigns
   * a default background of light gray.
   *
   * @param[in] width of the image
   * @param[in] height of the image
   * @param[in] background color of the 3d scene
   */
  Renderer(uint width, uint height,
           vec3f background = vec3f(0.384f, 0.333f, 0.333f));

  /**
   * Initilizes the Renderer object with width, height, FOV,
   * cameraOrigin at zero, and buffer of width * height. Background
   * can now be a cubemap.
   *
   * @param[in] width of the image
   * @param[in] height of the image
   * @param[in] faces path to the cube map faces
   */
  Renderer(uint width, uint height, std::vector<const char*> faces);

  /**
   * Renders the final image using the added objects, and
   * calculating the ray directions, its intersections, and
   * ray bounces. It saves the pixel color in the buffer, in
   * order to output to a file when called.
   */
  void render();

  /**
   * Handels rendering an image using multiple threads. The way it
   * assigns what the thread renders is by recursively splitting the
   * rectangle into equal pieces.
   * For example:
   * |-----------|    |-----------|
   * |           |    |_____|_____|
   * |           | => |     |     | => and so on and so forth
   * |-----------|    |-----------|
   * 
   * So each smaller equal rectangle is done by one thread. Here it is a
   * split count of 1.
   */
  void renderMultithreaded(uint splitCount = 0);

  /**
   * Acts the same as render but it is instead done on each
   * individual thread. Since each thread is independent but
   * share a common storage (i.e the color buffer), the instance of 
   * the Renderer is passed around to access the buffer. Also,
   * there will be on collision of data writing between threads 
   * since each pixel is unique.
   */
  static void renderThread(Renderer* r, uint topX, uint topY, uint bottomX,
                           uint bottomY) {
    for (uint y = topY; y < bottomY; y++) {
      for (uint x = topX; x < bottomX; x++) {
        uint bufferIndex = r->index(x, y);
        for (uint sample = 0; sample < r->totalSample; sample++) {
          float deltaX = r->jitterValues[2 * sample];
          float deltaY = r->jitterValues[2 * sample + 1];

          r->buffer[bufferIndex] +=
              r->castRay(r->cameraOrigin, r->getRay(x + deltaX, y + deltaY), 0);
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
   * @param[in] fileName name of the file to output to
   */
  void output(const char* fileName);

  /**
   * Add's object to a list for ray intersection.
   *
   * @param[in] object to add
   */
  void addObject(Object* object);

  /**
   * Add's lights to a list for ray intersection.
   *
   * @param[in] light to add
   */
  void addLight(Light* light);

  /**
   * Deletes all the heap allocated objects
   */
  ~Renderer();

 private:
  /**
   * Maps a 2d index to a 1d index.
   *
   * @param[in] i ith position of the 2d array
   * @param[in] j jth position of the 2d array
   * @return the index of the 1d array
   */
  uint index(uint i, uint j);

  /**
   * Calcualtes the normalized ray direction vector from screen space.
   *
   * @param[in] x screen space position
   * @param[in] y screen space position
   * @return Ray direction vector from origin to screen space
   */
  vec3f getRay(float x, float y);

  /**
   * Handels tracing the ray thru the scene recursively. Upon the
   * initial casting of the ray, it checks if this ray intersects any
   * objects in the scene. Then it handels weather to bounce the ray
   * to calcualte reflections. After max bounce or no scene intersection, 
   * it recursively returns the color value to handel reflection.
   *
   * @param[in] rayOrigin point of the ray
   * @param[in] rayDir direction of the ray
   * @return Final color value of the ray after tracing thru the scene
   */
  vec3f castRay(const vec3f& rayOrigin, const vec3f& rayDir, uint depth = 0);

  /**
   * Checks weather a given ray intersects any objects in the scene.
   * If it did, they the properties of that hit point is stored.
   *
   * @param[in] rayOrigin origin of the ray
   * @param[in] rayDir direction of the ray
   * @param[out] hitPoint where in the world did it hit
   * @param[out] N what is the normal of that hit point
   * @param[out] hitMaterial what are the properties of that hit point
   * @return A bool whether the ray intersects any objects  
   */
  bool sceneIntersect(const vec3f& rayOrigin, const vec3f& rayDir,
                      vec3f& hitPoint, vec3f& N, Material& hitMaterial);

  /**
   * Clips a float value between two float values.
   *
   * @param[in] n value to clip
   * @param[in] lower lower bound
   * @param[in] upper upper bound
   * @return the clipped value of n
   */
  float clip(float n, float lower, float upper);

  /**
   * Calcualtes the diffuse quantity using dot product between light and normal
   */
  float getDiffuse(Light* l, vec3f& N, vec3f& light);

  /**
   * Checks weather a hit point is in shadows. It does this so by checking
   * for scene intersection from that hit point.
   */
  bool inShadow(Light* l, vec3f& rayHit, vec3f hitN, vec3f& dirToLight);

  /**
   * Makes sure that the ray which hits an object does not self hit due to arithmetic erros
   * by moving the ray up or down (depending on the dot) by little.
   */
  vec3f getOriginShift(const vec3f& origin, const vec3f& dir, const vec3f& N);

  /**
   * Calcualtes the reflected ray
   * 
   * @param[in] A ray to reflect
   * @param[in] B ray to reflect around 
   * @return The refelected ray A across B
   */
  vec3f reflect(const vec3f& A, const vec3f& B);

  /**
   * Gets the specular highlight amount based on the light position
   */
  float getSpecular(Light* l, const vec3f& R, const vec3f& E,
                    const Material& mat);

 private:
  uint width;
  uint height;
  uint FOV = 90;
  bool useCubemap;
  uint totalSample = 3;
  std::vector<float> jitterValues{0.25f, 0.75f, -0.75f, 0.33f, 0.45f};
  vec3f cameraOrigin;
  vec3f background;

  std::vector<vec3f> buffer;
  std::list<Object*> objects;
  std::list<Light*> lights;
  Cubemap cubemap;
};

#endif