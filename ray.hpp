#ifndef ray_hpp
#define ray_hpp

#include "vec3.hpp"

class ray {
public:
    ray() = default;
    ray(const vec3 &origin, const vec3 &direction, float time = 0.0) :
        origin(origin), direction(direction), time(time) { }

    vec3 pointAt(float t) const { return origin + t * direction; }

    vec3 origin;
    vec3 direction;
    float time;
};

#endif
