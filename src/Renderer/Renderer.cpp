#include "Renderer.h"

Renderer::Renderer(uint width, uint height, vec3f background) {
    this->width = width;
    this->height = height;
    this->FOV = FOV;
    this->cameraOrigin = vec3f(0);
    this->buffer.resize(width * height);
    this->useCubemap = false;
    this->background = background;
}

Renderer::Renderer(uint width, uint height, std::vector<const char*> faces) {
    this->width = width;
    this->height = height;
    this->FOV = FOV;
    this->cameraOrigin = vec3f(0);
    this->buffer.resize(width * height);
    cubemap = Cubemap(faces);
    this->useCubemap = true;
}

void Renderer::render() {
    for (uint y = 0; y < height; y++) {
        for (uint x = 0; x < width; x++) {
            uint bufferIndex = index(x, y);
            for (uint sample = 0; sample < totalSample; sample++) {
                float deltaX = jitterValues[sample];
                float deltaY = jitterValues[sample + 1];
                buffer[bufferIndex] += castRay(cameraOrigin, 
                                    getRay(x + deltaX, y + deltaY), 0);
            }
            buffer[bufferIndex] /= totalSample;
        }
    }
}

void Renderer::renderMultithreaded(uint splitCount) {
    if (splitCount == 0) {
        render();
        return;
    }
    uint threadCount = 1 << (2*splitCount);
    std::thread threads[threadCount];
    uint threadI = 0;

    uint deltaThread = 1 << splitCount;
    float deltaX = width / float(deltaThread);
    float deltaY = height / float(deltaThread);

    for (uint y = 0; y < deltaThread; y++) {
        for (uint x = 0; x < deltaThread; x++) {
            uint topX = (uint)deltaX * x;
            uint topY = (uint)deltaY * y;
            uint bottomX = topX + deltaX;
            uint bottomY = topY + deltaY;
            threads[threadI] =
                std::thread(renderThread, this, topX, topY, bottomX, bottomY);
            threadI++;
        }
    }

    for (int i = 0; i < threadCount; i++) {
        threads[i].join();
    }
}

vec3f Renderer::castRay(const vec3f& rayOrigin, const vec3f& rayDir, uint depth) {
    vec3f hitPoint, N, dir(rayDir);
    Material hitMaterial;
    if (depth > MAX_DEPTH ||
        !sceneIntersect(rayOrigin, rayDir, hitPoint, N, hitMaterial)) {
        return useCubemap ? cubemap.getBackground(rayDir) : background;
    }

    vec3f reflectDir = reflect(-dir, N);
    vec3f reflectOrigin = getOriginShift(hitPoint, reflectDir, N);
    vec3f reflectColor = castRay(reflectOrigin, reflectDir, depth + 1);

    float diffuseIntensity = 0.1f;
    float specularIntensity = 0;
    for (Light* l : lights) {
        vec3f pointToLight = (*l->getPosition() - hitPoint).getNormalized();
        vec3f reflectedLight = reflect(pointToLight, N).getNormalized();

        if (inShadow(l, hitPoint, N, pointToLight)) continue;

        //accumulate all the diffuseIntensity from each light
        diffuseIntensity += getDiffuse(l, N, pointToLight);
        specularIntensity += getSpecular(l, reflectedLight, -dir, hitMaterial);
    }

    //return the teal color based on how much light hits it
    return hitMaterial.diffuse() * diffuseIntensity * hitMaterial.albedo()[0] +
           vec3f(1) * specularIntensity * hitMaterial.albedo()[1] +
           reflectColor * hitMaterial.albedo()[2];
}

bool Renderer::sceneIntersect(const vec3f& rayOrigin, const vec3f& rayDir,
                              vec3f& hitPoint, vec3f& N, Material& hitMaterial) {
    float maxDistance = __FLT_MAX__;
    for (const Object* o : objects) {
        float t;
        if (o->rayIntersect(rayOrigin, rayDir, t) && t < maxDistance) {
            maxDistance = t;
            hitPoint = rayOrigin + rayDir * t;
            N = o->getNormal(hitPoint).getNormalized();
            hitMaterial = Material(*o->getMaterial());
        }
    }
    return maxDistance < RENDER_DISTANCE;
}

vec3f Renderer::getRay(float x, float y) {
    vec3f rayDirection;
    float aspectRatio = (float)width / height;

    //shift to middle, normalize, and screen space
    float xPrime = (2.0f * (x / width) - 1) * aspectRatio *
                   tan((FOV / 2.0f) * (PI / 180.0f));
    float yPrime = (1 - 2.0f * (y / height) *
                            tan((FOV / 2.0f) * (PI / 180.0f)));

    //get ray direction towards negative z-axis
    rayDirection = vec3f(xPrime, yPrime, -1) - cameraOrigin;
    return rayDirection.getNormalized();
}

bool Renderer::inShadow(Light* l, vec3f& rayHit, vec3f hitN, vec3f& dirToLight) {
    vec3f hitPoint, origin;
    Material hitMaterial;
    float lightDistance = (*l->getPosition()).length();

    //get the shadow ray origin. push it a bit forward or backward to
    //avoid self intersection
    origin = getOriginShift(rayHit, dirToLight, hitN);
    if (sceneIntersect(origin, dirToLight, hitPoint, hitN, hitMaterial) &&
        (hitPoint - origin).length() < lightDistance) {
        return true;
    }
    return false;
}

void Renderer::output(const char* fileName) {
    std::ofstream out(fileName, std::ofstream::binary);
    out << "P6\n"
        << width << " " << height << " 255\n";

    std::vector<u_char> outputBuffer;
    outputBuffer.reserve(BUFFER_MAX * 3);

    for (uint y = 0; y < height; y++) {
        for (uint x = 0; x < width; x++) {
            uint colorIndex = index(x, y);
            for (int i = 0; i < 3; i++) {
                float clipped = 255 * clip(buffer[colorIndex][i], 0.0f, 1.0f);
                u_char pixelValue = (u_char)clipped;
                outputBuffer.push_back(pixelValue);
            }
            if (outputBuffer.size() == outputBuffer.capacity()) {
                out.write((char*)&outputBuffer[0], outputBuffer.size());
                outputBuffer.clear();
            }
        }
    }
    out.close();
}

uint Renderer::index(uint i, uint j) {
    return i + j * width;
}

void Renderer::addObject(Object* object) {
    objects.push_back(object);
}

void Renderer::addLight(Light* light) {
    lights.push_back(light);
}

vec3f Renderer::getOriginShift(const vec3f& origin, const vec3f& dir, const vec3f& N) {
    return (dir.dot(N) >= 0) ? origin + N * .001f : origin - N * .001f;
}

float Renderer::clip(float n, float lower, float upper) {
    return std::max(lower, std::min(n, upper));
}

float Renderer::getDiffuse(Light* l, vec3f& N, vec3f& light) {
    return l->getIntensity() * std::max(0.0f, N.dot(light));
}

vec3f Renderer::reflect(const vec3f& A, const vec3f& B) {
    return 2.0f * ((B.dot(A)) * B) - A;
}

float Renderer::getSpecular(Light* l, const vec3f& R, const vec3f& E, const Material& mat) {
    return l->getIntensity() * powf(std::max(0.0f, R.dot(E)), mat.specular());
}

Renderer::~Renderer() {
    for (Object* o : objects) {
        delete o;
    }
}