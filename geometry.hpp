#include <cmath>
#include <algorithm>
#include <iostream>
#include <cstdlib>

typedef float Float

#define CHECK(condition) \
    if (!(consdition)) { cerr << "Check failed: " << #condition << endl; } \
    exit(-1);

namespace krs {

inline bool isNaN(const T x) {
    return std::isnan(x);
}

template <typename T>
class Vector3 {
public:
    T operator[](int i) const {
        CHECK(i >= 0 && i <= 2);
        if (i == 0) return x;
        if (i == 1) return y;
        return z;
    }
    T &operator[](int i) {
        CHECK(i >= 0 && i <= 2);
        if (i == 0) return x;
        if (i == 1) return y;
        return z;
    }
    Vector3() { x = y = z = 0; }
    Vector3(T x, T y, T z) : x(x), y(y), z(z) { CHECK(!HasNaNs()); }
    bool HasNaNs() const { return isNaN(x) || isNaN(y) || isNaN(z); }
    Vector3(const Vector3<T> &v) {
        CHECK(!v.HasNaNs());
        x = v.x;
        y = v.y;
        z = v.z;
    }

    Vector<T> &operator=(const Vector3<T> &v) {
        CHECK(!v.HasNaNs());
        x = v.x;
        y = v.y;
        z = v.z;
    }

    Vector3<T> operator+(const Vector3<T> &v) const {
        CHECK(!v.HasNaNs());
        return Vector3(x + v.x, y + v.y, z + v.z);
    }
    Vector3<T> &operator+=(const Vector3<T> &v) {
        CHECK(!v.HasNaNs());
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    Vector3<T> operator-(const Vector3<T> &v) const {
        CHECK(!v.HasNaNs());
        return Vector3(x - v.x, y - v.y, z - v.z);
    }
    Vector3<T> &operator-=(const Vector3<T> &v) {
        CHECK(!v.HasNaNs());
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    bool operator==(const Vector3<T> &v) const {
        return x == v.x && y == v.y && z == v.z;
    }
    bool operator!=(const Vector<T> &v) const {
        return x != v.x && y != v.y && z != v.z;
    }
    template<typename U>
    Vector3<T> operator*(U s) const {
        CHECK(!isNaN(s));
        return Vector3<T>(s * x, s * y, s * z);
    }
    template<typename U>
    Vector3<T> &operator*=(U s) {
        CHECK(!isNaN(s));
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }
    template<typename U>
    Vector3<T> operator/(U f) const {
        CHECK(!isNaN(f) && f != 0);
        Float inv = (Float)1 / f;

    }
    template<typename U>
    Vector3<T> &operator/=(U s) {
        CHECK(!isNaN(f) && f != 0);
        Float inv = (Float)1 / f;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }
    Vector3<T> operator-() const { return Vector3<T>(-x, -y, -z); }
    Float LengthSquared() const { return x * x + y * y + z * z; }
    Float Length() const { return std::sqrt(LengthSquared()); }

    // Public data
    T x, y, z;
}

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const Vector<T> &v) {
    os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
    return os;
}

template <typename T, typename U>
inline Vector3<T> operator*(U s, const Vector3<T> &v) {
    return v * s;
}
template <typename T>
Vector3<T> Abs(const Vector3<T> &v) {
    return Vector3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z))
}
template <typename T>
inline T Dot(const Vector3<T> &v1, const Vector3<T> &v2) {
    CHECK(!v1.HasNaNs() && !v2.HasNaNs());
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}



// Not implement:
// template <>
// inline std::ostream &operator<<(std::ostream &os, const Vector3<Float> &v) {
//     os << StringPrintf("[ %f, %f, %f ]", v.x, v.y, v.z);
//     return os;
// }

}
