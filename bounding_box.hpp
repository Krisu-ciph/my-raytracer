#ifndef bounding_box_hpp
#define bounding_box_hpp

#include <algorithm>

class aabb {
public:
    aabb() = default;
    aabb(const vec3 &a, const vec3 &b) { min = a; max = b; }

    inline bool hit(const ray &r, float tmin, float tmax) const {
        for (int i = 0; i < 3; i++) {
            // Find t-interval
            float invD = 1.0f/r.direction[i];
            float t0 = (min[i]-r.origin[i]) * invD;
            float t1 = (max[i]-r.origin[i]) * invD;
            if (t0 > t1) {
                std::swap(t0, t1);
            }

            tmin = std::max(t0, tmin);
            tmax = std::min(t1, tmax);

            if (tmax <= tmin) {
                return false;
            }
        }
        return true;
    }

    vec3 min;
    vec3 max;
};

aabb box_union(const aabb &box0, const aabb &box1) {
    vec3 small(
        std::min(box0.min.x, box1.min.x),
        std::min(box0.min.y, box1.min.y),
        std::min(box0.min.z, box1.min.z)
    );
    vec3 big (
        std::max(box0.max.x, box1.max.x),
        std::max(box0.max.y, box1.max.y),
        std::max(box0.max.z, box1.max.z)
    );
    return aabb(small, big);
}

#endif
