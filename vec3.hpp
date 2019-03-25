#ifndef vec3_hpp
#define vec3_hpp

#include <cmath>
#include <cstdlib>
#include <iostream>

class vec3 {
public:
    vec3() { x = y = z = 0; }
    vec3(float x, float y, float z) : x(x), y(y), z(z) { }

    // Declaration only:
    inline float operator[](int i) const;
    inline float &operator[](int i);
    inline vec3 &operator+=(const vec3 &v2);
    inline vec3 &operator-=(const vec3 &v2);
    inline vec3 &operator*=(const float t);
    inline vec3 &operator/=(const float t);
    inline void makeUnitVector();

    // Decl and Implementation:
    inline const vec3 &operator+() const;
    inline vec3 operator-() const;
    inline vec3 operator+(const vec3 &v2) const;
    inline vec3 operator-(const vec3 &v2) const;
    inline vec3 operator*(const float t) const;
    inline vec3 operator/(const float t) const;

    inline float lengthSquared() const;
    float length() const;

    // Public data members:
    float x, y, z;
};

// Definitions:
/************* Member functions: **************/
inline float vec3::operator[](int i) const {
    if (i == 0) return x;
    if (i == 1) return y;
    return z;
}
inline float &vec3::operator[](int i) {
    if (i == 0) return x;
    if (i == 1) return y;
    return z;
}
inline vec3 &vec3::operator+=(const vec3 &v2) {
    x += v2.x;  y += v2.y;  z += v2.z;
    return *this;
}
inline vec3 &vec3::operator-=(const vec3 &v2) {
    x -= v2.x;  y -= v2.y;  z -= v2.z;
    return *this;
}
inline vec3 &vec3::operator*=(const float t) {
    x *= t;  y *= t;  z *= t;
    return *this;
}
inline vec3 &vec3::operator/=(const float t) {
    float t_inv = 1.f / t;
    x *= t_inv;  y *= t_inv;  z *= t_inv;
    return *this;
}
inline void vec3::makeUnitVector() {
    float len_inv = 1.f / length();
    x *= len_inv;
    y *= len_inv;
    z *= len_inv;
}
inline const vec3 &vec3::operator+() const { return *this; }
inline vec3 vec3::operator-() const { return vec3(-x, -y, -z); }
inline vec3 vec3::operator+(const vec3 &v2) const {
    return vec3(x + v2.x, y + v2.y, z + v2.z);
}
inline vec3 vec3::operator-(const vec3 &v2) const {
    return vec3(x - v2.x, y - v2.y, z - v2.z);
}
inline vec3 vec3::operator*(const float t) const {
    return vec3(x * t, y * t, z * t);
}
inline vec3 vec3::operator/(const float t) const {
    float t_inv = 1.f / t;
    return vec3(x * t_inv, y * t_inv, z * t_inv);
}
inline float vec3::lengthSquared() const {
    return x * x + y * y + z * z;
}
float vec3::length() const { return std::sqrt(lengthSquared()); }


/************* Other funcions: **************/
std::istream &operator>>(std::istream &is, vec3 &v) {
    is >> v.x >> v.y >> v.z;
    return is;
}

std::ostream &operator<<(std::ostream &os, const vec3 &v) {
    os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
    return os;
}

vec3 operator*(const float t, const vec3 &v) {
    return v * t;
}

inline float dot(const vec3 &v1, const vec3 &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline vec3 cross(const vec3 &v1, const vec3 &v2) {
    return vec3(
        v1.y * v2.z - v1.z * v2.y,
        -v1.x * v2.z + v1.z * v2.z,
        v1.x * v2.y - v1.y * v2.x
    );
}

inline vec3 unit(const vec3 &v) {
    return v / v.length();
}

#endif
