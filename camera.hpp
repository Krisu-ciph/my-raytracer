#ifndef camera_hpp
#define camera_hpp

#include <cmath>
#include "ray.hpp"

/**************************
Well, it's the camera
**************************/

// We can use a vec2 actually
vec3 random_in_unit_disk() {
    vec3 p;
    do {
        p = 2.0*vec3(drand48(), drand48(), 0) - vec3(1, 1, 0);
    } while (dot(p, p) >= 1.0);
    return p;
}

class camera {
public:
    // vfov is top to bottom in degrees
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect,
            float aperture, float focus_dist) {
        float theta = vfov*M_PI/180;
        float half_height = tan(theta/2);
        float half_width = aspect * half_height;

        origin = lookfrom;
        w = unit(lookfrom - lookat);
        u = unit(cross(vup, w));
        v = cross(w, u); // Now u, v, w form an orthonormal basis

        lower_left_corner = origin - (half_width*u - half_height*v - w) * focus_dist;
        horizontal = 2 * half_width * u * focus_dist;
        vertical = 2 * half_height * v * focus_dist;
    }
    ray get_ray(float s, float t) {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x + u * rd.y;
        return ray(origin + offset,
            lower_left_corner + s*horizontal + t*vertical - origin - offset);
    }

    // Members
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
    // The orthonormal basis of the camera "lookfrom"
    vec3 u, v, w;
    // For defocus blur
    float lens_radius;
};

#endif
