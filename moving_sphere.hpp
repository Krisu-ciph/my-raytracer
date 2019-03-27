#ifndef moving_sphere_hpp
#define moving_sphere_hpp

#include "hitable.hpp"

class moving_sphere: public hitable {
public:
    moving_sphere() = delete;
    moving_sphere(vec3 cen0, vec3 cen1, float t0, float t1, float r, material *m) :
     center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m) {}

    virtual bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const;
    vec3 center(float time) const;

    // Data members:
    vec3 center0, center1;
    float time0, time1;
    float radius;
    material *mat_ptr;
};

vec3 moving_sphere::center(float time) const {
    return center0 + ((time - time0) / (time1 - time0))*(center1 - center0);
}

bool moving_sphere::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    vec3 oc = r.origin - center(r.time);
    float a = r.direction.lengthSquared();
    float b = 2 * dot(r.direction, oc);
    float c = oc.lengthSquared() - radius * radius;
    float discriminant = b*b - 4*a*c;

    if (discriminant > 0) {
        float temp = (-b - sqrt(discriminant)) / (2*a);
        if (temp > t_min && temp < t_max) {
            rec.t = temp;
            rec.p = r.pointAt(rec.t);
            rec.normal = (rec.p - center(r.time)) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-b + sqrt(discriminant)) / (2*a);
        if (temp > t_min && temp < t_max) {
            rec.t = temp;
            rec.p = r.pointAt(rec.t);
            rec.normal = (rec.p - center(r.time)) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
};

#endif
