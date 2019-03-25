#ifndef camera_hpp
#define camera_hpp

/**************************
Camera to a 200 * 100 image
The film is a rectangle whose middle point at (0, 0, -1)
**************************/
class camera {
public:
    camera() {
        lower_left_corner = vec3(-2.0, -1.0, -1.0);
        horizontal = vec3(4.0, 0.0, 0.0);
        vertical = vec3(0.0, 2.0, 0.0);
        origin = vec3(0.0, 0.0, 0.0);
    }
    ray get_ray(float u, float v) { return ray(origin,
        lower_left_corner + u*horizontal + v*vertical - origin); }

    // Members
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
};

#endif
