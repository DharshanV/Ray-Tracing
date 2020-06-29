#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <vector>
#include <fstream>
#include "Vector3.h"

using namespace std;
typedef Vector3<float> vec3f;

struct CubemapData {
    u_char* data;
    int width, height, channels;
};

class Cubemap {
   public:
    Cubemap();
    Cubemap(const vector<const char*>& faces);

    vec3f getBackground(const vec3f& direction);

   private:
    void vectorToUV(float x, float y, float z,
                    int* index, float* u, float* v);
    uint index(uint i, uint j,int width);

   private:
    vector<CubemapData> cubemaps;
};
#endif