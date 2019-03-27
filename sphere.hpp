#ifndef sphere_hpp
#define sphere_hpp

#include "ray.hpp"
#include "hitable.hpp"
#include "material.hpp"

class sphere: public hitable {
public:
    sphere() = delete;
    sphere(vec3 center, float r, material *mat_ptr) : center(center), radius(r), mat_ptr(mat_ptr) {}
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;
    virtual bool bounding_box(float t0, float t1, aabb &box) const;

    material *mat_ptr;
    vec3 center;
    float radius;
};

/*********************
dot(O + t*D - C, O + t*D - C) == R  -->
a*t^2 + b*t + c == 0 -->
discriminant = b^2 - 4*a*c > 0 == hit
*********************/
bool sphere::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    vec3 oc = r.origin - center;
    float a = r.direction.lengthSquared();
    float b = 2 * dot(r.direction, oc);
    float c = oc.lengthSquared() - radius * radius;
    float discriminant = b*b - 4*a*c;

    if (discriminant > 0) {
        float temp = (-b - sqrt(discriminant)) / (2*a);
        if (temp > t_min && temp < t_max) {
            rec.t = temp;
            rec.p = r.pointAt(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-b + sqrt(discriminant)) / (2*a);
        if (temp > t_min && temp < t_max) {
            rec.t = temp;
            rec.p = r.pointAt(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
};

bool sphere::bounding_box(float t0, float t1, aabb &box) const {
    box = aabb(center - vec3(radius, radius, radius),
               center + vec3(radius, radius, radius));
    return true;
}

#endif
