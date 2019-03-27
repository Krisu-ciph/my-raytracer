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
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist, float t0, float t1) {
        time0 = t0;
        time1 = t1;

        lens_radius = aperture / 2;

        float theta = vfov*M_PI/180; // Change field of view to radius
        float half_height = tan(theta/2);
        float half_width = aspect * half_height;

        origin = lookfrom;
        w = unit(lookfrom - lookat);
        u = unit(cross(vup, w));
        v = cross(w, u); // Now u, v, w form an orthonormal basis

        lower_left_corner = origin - focus_dist * (half_width*u + half_height*v + w);

        horizontal = 2*half_width*focus_dist * u;
        vertical = 2*half_height*focus_dist * v;
    }
    ray get_ray(float s, float t) {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x + u * rd.y;

        float time = time0 + drand48()*(time1-time0);

        return ray(origin + offset,
            lower_left_corner + s*horizontal + t*vertical - origin - offset, time);
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
    // For motion blur
    float time0, time1;
};

#endif
