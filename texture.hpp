#ifndef texture_hpp
#define texture_hpp

#include <cmath>

class texture {
public:
    virtual vec3 value(float u, float v, const vec3 &p) const = 0;
};

class constant_texture : public texture {
public:
    constant_texture() {}
    explicit constant_texture(vec3 color) : color(color) {}
    constant_texture(float r, float g, float b) : color(vec3(r, g, b)) {}

    virtual vec3 value(float u, float v, const vec3 &p) const {
        return color;
    }

    vec3 color;
};

class checker_texture : public texture {
public:
    checker_texture() {}
    checker_texture(texture *t0, texture *t1) : even(t0), odd(t1) {}

    virtual vec3 value(float u, float v, const vec3 &p) const {
        int signs = sign(10*p.x) * sign(10*p.y) * sign(10*p.z);
        // float signs = sin(10*p.x) * sin(10*p.y) * sin(10*p.z);
        if (signs < 0) {
            return odd->value(u, v, p);
        } else {
            return even->value(u, v, p);
        }
    }

    texture *odd;
    texture *even;
private:
    int sign(float x) const {
        float mx = std::fmod(x, 2*M_PI);
        mx = (mx < 0 ? mx + 2*M_PI : mx);
        return (mx < M_PI ? 1 : -1);
    }
};

#endif
