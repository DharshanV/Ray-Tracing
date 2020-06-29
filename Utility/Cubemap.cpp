#include "Cubemap.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Cubemap::Cubemap() {}

Cubemap::Cubemap(const vector<const char*>& faces) {
    if (faces.size() != 6) return;
    int width, height, nChannels;
    for (const char* pathi : faces) {
        u_char* data = stbi_load(pathi, &width, &height, &nChannels, 3);
        cubemaps.push_back({data, width, height, nChannels});
    }
}

vec3f Cubemap::getBackground(const vec3f& direction) {
    //check for invalid cubemap
    if (cubemaps.size() == 0 || cubemaps.size() != 6) return vec3f(0);
    float u, v;
    int faceIndex;
    vectorToUV(direction[0], -direction[1], direction[2], &faceIndex, &u, &v);
    float RGB[3];
    uint i = uint(u * cubemaps[faceIndex].width);
    uint j = uint(v * cubemaps[faceIndex].height);
    uint dataIndex = index(i, j, cubemaps[faceIndex].width);
    for (int i = 0; i < 3; i++)
        RGB[i] = (float)cubemaps[faceIndex].data[dataIndex * 3 + i];
    return vec3f(RGB[0], RGB[1], RGB[2]) / 255.0f;
}

uint Cubemap::index(uint i, uint j, int width) {
    return i + j * width;
}

void Cubemap::vectorToUV(float x, float y, float z,
                         int* index, float* u, float* v) {
    float absX = fabs(x);
    float absY = fabs(y);
    float absZ = fabs(z);

    int isXPositive = x > 0 ? 1 : 0;
    int isYPositive = y > 0 ? 1 : 0;
    int isZPositive = z > 0 ? 1 : 0;

    float maxAxis, uc, vc;

    // POSITIVE X
    if (isXPositive && absX >= absY && absX >= absZ) {
        // u (0 to 1) goes from +z to -z
        // v (0 to 1) goes from -y to +y
        maxAxis = absX;
        uc = -z;
        vc = y;
        *index = 0;
    }
    // NEGATIVE X
    if (!isXPositive && absX >= absY && absX >= absZ) {
        // u (0 to 1) goes from -z to +z
        // v (0 to 1) goes from -y to +y
        maxAxis = absX;
        uc = z;
        vc = y;
        *index = 1;
    }
    // POSITIVE Y
    if (isYPositive && absY >= absX && absY >= absZ) {
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from +z to -z
        maxAxis = absY;
        uc = x;
        vc = -z;
        *index = 2;
    }
    // NEGATIVE Y
    if (!isYPositive && absY >= absX && absY >= absZ) {
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from -z to +z
        maxAxis = absY;
        uc = x;
        vc = z;
        *index = 3;
    }
    // POSITIVE Z
    if (isZPositive && absZ >= absX && absZ >= absY) {
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from -y to +y
        maxAxis = absZ;
        uc = x;
        vc = y;
        *index = 4;
    }
    // NEGATIVE Z
    if (!isZPositive && absZ >= absX && absZ >= absY) {
        // u (0 to 1) goes from +x to -x
        // v (0 to 1) goes from -y to +y
        maxAxis = absZ;
        uc = -x;
        vc = y;
        *index = 5;
    }

    // Convert range from -1 to 1 to 0 to 1
    *u = 0.5f * (uc / maxAxis + 1.0f);
    *v = 0.5f * (vc / maxAxis + 1.0f);
}