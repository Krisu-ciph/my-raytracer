#ifndef camera_hpp
#define camera_hpp

#include <cmath>
#include "ray.hpp"

/**************************

**************************/
class camera {
public:
    // vfov is top to bottom in degrees
    camera(float vfov, float aspect) {
        float theta = vfov*M_PI/180;
        float half_height = tan(theta/2);
        float half_width = aspect * half_height;

        lower_left_corner = vec3(-half_width, -half_height, -1.0);
        horizontal = vec3(2*half_width, 0.0, 0.0);
        vertical = vec3(0.0, 2*half_height, 0.0);
        origin = vec3(0.0, 0.0, 0.0);
    }
    ray get_ray(float u, float v) {
        return ray(origin,
            lower_left_corner + u*horizontal + v*vertical - origin);
    }

    // Members
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
};

#endif
