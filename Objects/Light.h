#ifndef LIGHT_H
#define LIGHT_H

#include "../Utility/Object.h"

class Light {
   public:
    Light(const vec3f& position, float intensity) : 
            position(position), intensity(intensity) { }
    ~Light() { }

    const vec3f* getPosition() const { return &position; }
	float getIntensity() const { return intensity; }

   private:
    vec3f position;
    float intensity;
};

#endif