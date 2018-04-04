#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <vector>
#include <iostream>
#include <cassert>

#include "geometry.hpp"

const int MAX_RAY_DEPTH = 5;
const float PI = acos(-1.0);
const float INF_F = std::numeric_limits<float>::max();

inline float mix(const float a, const float b, const float mix) {
    return b * mix + a * (1 - mix);
}

Vec3f trace(const Vec3f &rayorig, const Vec3f &rayDir, const
            std::vector<Sphere> &spheres, const int depth) {
    float tnear = INF_F;
    const Sphere *sphere = nullptr;
    // Find intersection of this ray with the sphere in the scene
    for (int i = 0; i < spheres.size(); ++i) {
        float t0 = INF_F, t1 = INF_F;
        if (spheres[i].intersect(rayorig, rayDir, t0, t1)) {
            if (t0 < 0)
                t0 = t1;
            if (t0 < tnear) {
                tnear = t0;
                sphere = &spheres[i];
            }
        }
    }
    // If there's no intersection, return black or backgound color
    if (!sphere)
        return Vec3f(2.f);
    Vec3f surfaceColor(0.f);
    Vec3f pIsec = rayorig + rayDir * tnear; // point of intersection
    Vec3f nIsec = pIsec - sphere->center; // normal at the intersection point
    nIsec.normalize();
    const float bias = 1e-4;
    bool inside = false;
    // If the view direction and normal are opposite to each other, reverse
    // the normal direction. It
    if (rayDir.dot(nIsec) > 0) {
        nIsec = -nIsec;
        inside = true;
    }
    // Need to trace again
    if ((sphere->transparency > 0 || sphere->reflection > 0) && depth < MAX_RAY_DEPTH) {
        float facingRatio = -rayDir.dot(nIsec);
        // change the mix value to tweak the effect
        float fresnelEffect = mix(pow(1 - facingRatio, 3), 1, 0.1);
        // compute relection direction
        Vec3f reflDir = rayDir - nIsec * 2 * rayDir.dot(nIsec);
        reflDir.normalize();
        Vec3f reflection = trace(pIsec + nIsec * bias, reflDir, spheres, depth + 1);
        Vec3f refraction = Vec3f(0);
        // if sphere is also transparent, compute refraction ray
        // all sphere are reflective, of course...
        if (sphere->transparency) {
            float ior = 1.1;
            float eta = inside ? ior : 1.f / ior; // inside or outside
            float cosi = -nIsec.dot(rayDir);
            float k = 1 - eta * eta * (1.f - cosi * cosi);
            Vec3f refrDir = rayDir * eta + nIsec * (eta * cosi - sqrt(k));
            refrDir.normalize();
            refraction = trace(pIsec - nIsec * bias, refrDir, spheres, depth + 1);
        }
        // the result is a mix of reflection and refraction
        surfaceColor =
           (reflection * fresnelEffect +
            refraction * (1 - fresnelEffect) * sphere->transparency) *
            sphere->surfaceColor;
    } else {
        // it's a diffuse object, no need to raytrace any further
        // find a light source and trace
        for (int i = 0; i < spheres.size(); ++i) {
            if (spheres[i].emissionColor.x +
                spheres[i].emissionColor.y +
                spheres[i].emissionColor.z > 0.f) {
                // this is a light
                Vec3f transmission = Vec3f(1.f);
                Vec3f lightDir = spheres[i].center - pIsec;
                lightDir.normalize();
                for (int j = 0; j < spheres.size(); ++j) {
                    if (i != j) {
                        float t0, t1;
                        if (spheres[j].intersect(pIsec + nIsec * bias, lightDir, t0, t1)) {
                            transmission = Vec3f(0.f);
                            break;
                        }
                    }
                }
                surfaceColor += sphere->surfaceColor * transmission * std::max(0.f, nIsec.dot(lightDir)) * spheres[i].emissionColor;
            }
        }
    }

    return surfaceColor + sphere->emissionColor;
}

void render(const std::vector<Sphere> &spheres) {
    const unsigned width = 1080, height = 720;
    Vec3f *image = new Vec3f[width * height];
    Vec3f *pixel = image;
    float invWidth = 1.f / width, invHeight = 1.f / height;
    float fov = 30.f, aspectRatio = float(width) / height;
    float angle = tan(PI * 0.5 * fov / 180.);
    // Trace rays
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x, ++pixel) {
            float xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectRatio;
            float yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle;
            Vec3f rayDir(xx, yy, -1);
            rayDir.normalize();
            *pixel = trace(Vec3f(0), rayDir, spheres, 0);
        }
    }
    // Save result to a PPM image
    std::ofstream ofs("./result.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (int i = 0; i < width * height; ++i) {
        ofs << (unsigned char)(std::min(1.f, image[i].x) * 255) <<
               (unsigned char)(std::min(1.f, image[i].y) * 255) <<
               (unsigned char)(std::min(1.f, image[i].z) * 255);
    }
    ofs.close();
    delete [] image;
}

int main(int argc, char **argv) {
    srand48(13);
    std::vector<Sphere> spheres;
    // posiition, radius, surface color, relectivity, transparency, emission color
    spheres.push_back(Sphere(Vec3f( 0.0, -10004, -20), 10000, Vec3f(0.20, 0.20, 0.20), 0, 0.0));
    spheres.push_back(Sphere(Vec3f( 0.0,      0, -20),     4, Vec3f(1.00, 0.32, 0.36), 1, 0.5));
    spheres.push_back(Sphere(Vec3f( 5.0,     -1, -15),     2, Vec3f(0.90, 0.76, 0.46), 1, 0.0));
    spheres.push_back(Sphere(Vec3f( 5.0,      0, -25),     3, Vec3f(0.65, 0.77, 0.97), 1, 0.0));
    spheres.push_back(Sphere(Vec3f(-5.5,      0, -15),     3, Vec3f(0.90, 0.90, 0.90), 1, 0.0));
    // light
    spheres.push_back(Sphere(Vec3f( 0.0,     20, -30),     3, Vec3f(0.00, 0.00, 0.00), 0, 0.0, Vec3f(3)));
    render(spheres);

    return 0;
}
