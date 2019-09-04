#ifndef RENDERER_H
#define RENDERER_H
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm> 
#include <limits>
#include <time.h>
#include <string>
#include "Debug.h"
#include "Model.h"
#include "Light.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define M_PI 3.14159
using namespace std;

typedef uint32_t uint;
typedef unsigned char uchar;
typedef const Vector3 CVector3;
typedef const Light CLight;
typedef const Model CModel;
typedef const Material CMaterial;
typedef vector<const Light*> VLight;
typedef vector<const Model*> VModel;
typedef const vector<const Light*> CVLight;
typedef const vector<const Model*> CVModel;

class Renderer {
public:
	Renderer(uint width, uint height, Vector3 eye = Vector3(0), float fov = M_PI/3) :
		width(width), height(height), rayOrigin(eye), fov(fov) {
		rendererStarted = false;
		buffer.resize(width*height);
		stbi_load("dasd", NULL, NULL, NULL, 0);
	}
	~Renderer() {
		clearObjects();
	}
public:
	bool start() {
		rendererStarted = true;
		timer.start = clock();
		DEBUG("PUTTING IN BUFFER");
		#pragma omp parallel for
		for (uint j = 0; j < height; j++) {
			for (uint i = 0; i < width; i++) {
				buffer[index(i, j)] = castRay(rayOrigin, getRay(i,j), objects, lights);
			}
		}
		DEBUG("BUFFER FILLED");
		timer.stop = clock();
		return true;
	}

	Vector3 castRay(CVector3& origin, CVector3& dir, CVModel& objects, CVLight& lights,uint depth = 0) {
		Vector3 hitPoint, N;
		Material hitMaterial;
		if (depth > 4 || !sceneIntersect(origin, dir, hitPoint, N, hitMaterial, objects)) {
			return getBackground(dir);
		}
		Vector3 reflectDir = -reflect(dir, N);
		Vector3 reflectOrigin = reflectDir.dot(N) < 0 ? hitPoint - N * .001f : hitPoint + N * .001f;
		Vector3 reflectColor = castRay(reflectOrigin, reflectDir, objects, lights, depth + 1);

		float diffuseIntensity = 0;
		float specularIntensity = 0;
		for (CLight* l : lights) {
			Vector3 lightToPoint = (*l->getPosition() - hitPoint).getNormalized();
			Vector3 reflectedLight = reflect(-lightToPoint, N).getNormalized();

			if (inShadow(l, hitPoint,N, lightToPoint, objects)) continue;

			diffuseIntensity += getDiffuse(l, N, lightToPoint);
			specularIntensity += getSpecular(l, reflectedLight, dir, hitMaterial);
		}
		return hitMaterial.diffuse() * diffuseIntensity * hitMaterial.albedo()[0] +
				Vector3(1) * specularIntensity * hitMaterial.albedo()[1] + 
				reflectColor * hitMaterial.albedo()[2];
	}

	bool sceneIntersect(CVector3& origin, CVector3& dir, Vector3& hitPoint,
						Vector3& N, Material& hitMaterial, CVModel& objects) {
		float maxDistance = FLT_MAX;
		for (CModel* o : objects) {
			float distanceI;
			if (o->rayIntersect(origin, dir, distanceI) && distanceI < maxDistance) {
				maxDistance = distanceI;
				hitPoint = origin + dir * distanceI;
				N = o->getNormal(hitPoint).getNormalized();
				hitMaterial = Material(*o->getMaterial());
			}
		}
		return maxDistance < 1000;
	}

	bool output(const char* fileName) {
		if (!rendererStarted) { DEBUG("RENDER NOT STARTED");  return false; }
		DEBUG("OUTPUT STARTED");
		ofstream out;
		out.open(fileName, std::ofstream::out | std::ofstream::binary);
		out << "P6\n" << width << " " << height << "\n255\n";
		uchar* outputBuffer = new uchar[width*height * 3];
		for (uint y = 0; y < height; y++) {
			for (uint x = 0; x < width; x++) {
				uint colorIndex = index(x, y);
				for (int i = 0; i < 3; i++) {
					outputBuffer[colorIndex * 3 + i] =
						(uchar)(255 * max(0.01f, min(1.0f, buffer[colorIndex][i])));
				}
			}
		}
		out << outputBuffer;
		out.close();
		delete outputBuffer;
		DEBUG("OUTPUT FINISHED");
		DEBUG("ELAPSED TIME: " + to_string(timer.stop - timer.start) + "ms");
		return true;
	}

	void addModel(CModel* s) {
		objects.push_back(s);
	}

	void addLight(CLight* l) {
		lights.push_back(l);
	}

	void backgroundColor(CVector3& color) {
		background = color;
	}

	void backgroundColor(const vector<string>& path) {
		facesPath = path;
		if (facesPath.size() != 6) return;
		uchar* data;
		int width, height, nChannels;
		for (const string& pathi : path) {
			data = stbi_load(pathi.c_str(), &width, &height, &nChannels, 3);
			cubemaps.push_back({ data,width,height,nChannels });
		}
	} 
private:
	uint index(uint i, uint j) {
		return i + j * width;
	}

	uint index(uint i, uint j,int width) {
		return i + j * width;
	}

	void clearObjects() {
		DEBUG("DELETING OBJECTS");
		for (const Model* o : objects) {
			if (o != NULL) { delete o; o = NULL; }
		}
		for (const Light* i : lights) {
			if (i != NULL) { delete i; i = NULL; }
		}
		for (Cubemap& cubemap : cubemaps) {
			stbi_image_free(cubemap.data);
			cubemap.data = NULL;
		}
		DEBUG("CLEARED OBJECTS");
	}

	Vector3 reflect(CVector3& A, CVector3& B) {
		return 2 * ( (B.dot(A)) * B ) - A;
	}

	Vector3 getRay(uint i,uint j) {
		float dirX = (i + 0.5f) - width / 2.0f;
		float dirY = -(j + 0.5f) + height / 2.0f; 
		float dirZ = -(height / (2.0f*tan(fov / 2.0f)));
		return Vector3(dirX, dirY, dirZ).getNormalized();
	}

	float getDiffuse(CLight* l,CVector3& N,CVector3& light) {
		return l->getIntensity() * max(0.0f, N.dot(light));
	}
	
	float getSpecular(CLight* l,CVector3& R,CVector3& E,CMaterial& mat) {
		return l->getIntensity() * powf(max(0.0f, R.dot(E)), mat.specular());
	}

	bool inShadow(CLight* l, CVector3& rayHit, CVector3& hitN, CVector3& dirToLight, CVModel& objects) {
		Vector3 hitPoint, N, origin;
		Material hitMaterial;
		float lightDistance = (origin - *l->getPosition()).length();
		origin = (dirToLight.dot(hitN) >= 0) ? rayHit + hitN * .001f : rayHit - hitN * .001f;
		if (sceneIntersect(origin, dirToLight, hitPoint, N, hitMaterial, objects) &&
			(hitPoint - origin).length() < lightDistance) {
			return true;
		}
		return false;
	}

	Vector3 getBackground(CVector3& direction) {
		if (cubemaps.size() == 0) return background;
		if (cubemaps.size() != 6) return Vector3(0);
		float u, v;
		int faceIndex;
		vectorToUV(direction[0], -direction[1], direction[2], &faceIndex, &u, &v);
		float RGB[3];
		uint i = uint(u * cubemaps[faceIndex].width);
		uint j = uint(v * cubemaps[faceIndex].height);
		uint dataIndex = index(i, j, cubemaps[faceIndex].width);
		for (int i = 0; i < 3; i++) 
			RGB[i] = (float)cubemaps[faceIndex].data[dataIndex * 3 + i];
		return Vector3(RGB[0],RGB[1],RGB[2])/255;
	}

	void vectorToUV(float x, float y, float z, int *index, float *u, float *v) {
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

private:
	struct Timer { clock_t start, stop; };
	struct Cubemap { uchar* data; int width, height, channels; };
	vector<Vector3> buffer;
	vector<string> facesPath;
	vector<Cubemap> cubemaps;
	bool rendererStarted;
	Vector3 rayOrigin;
	Vector3 background;
	uint width, height;
	VModel objects;
	VLight lights;
	Timer timer;
	float fov;
};

#endif // !RENDERER_H
