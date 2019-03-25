#ifndef ray_hpp
#define ray_hpp

#include "vec3.hpp"

class ray {
public:
    ray() {}
    ray(const vec3 &origin, const vec3 &direction) :
        origin(origin), direction(direction) { }

    vec3 pointAt(float t) const { return origin + t * direction; }

    vec3 origin;
    vec3 direction;
};

#endif
