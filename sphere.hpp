#ifndef sphere_hpp
#define sphere_hpp

#include "ray.hpp"
#include "hitable.hpp"

class sphere: public hitable {
public:
    sphere() {}
    sphere(vec3 center, float r) : center(center), radius(r) {}
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;

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
            return true;
        }
        temp = (-b + sqrt(discriminant)) / (2*a);
        if (temp > t_min && temp < t_max) {
            rec.t = temp;
            rec.p = r.pointAt(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
    }
    return false;
};

#endif
