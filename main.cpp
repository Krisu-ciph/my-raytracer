#include <iostream>
#include <limits>
#include <fstream>

#include "vec3.hpp"
#include "ray.hpp"
#include "hitable_list.hpp"
#include "sphere.hpp"
#include "moving_sphere.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "pcs_bar.hpp"
#include "scene_config.hpp"
#include "bvh.hpp"

/*****
This function use Path Tracing to calculate color at certain pixels.
The mat_ptr(pointer pointed to material) is set during the intersection
test(hit)
*****/
vec3 color_at(const ray &r, hitable *world, int depth) {
    hit_record rec;
    if (world->hit(r, 0.001, std::numeric_limits<float>::max(), rec)) {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            vec3 col = color_at(scattered, world, depth + 1);
            // object reflects the color of their own...
            return vec3(attenuation.x * col.x, attenuation.y * col.y,
                        attenuation.z * col.z);
        } else {
            return vec3(0, 0, 0);
        }
    } else {
        vec3 unit_dir = unit(r.direction);
        float k = 0.5 * (unit_dir.y + 1.0);
        return (1.f-k)*vec3(1.0, 1.0, 1.0) + k*vec3(0.5, 0.7, 1.0);
    }
}

int main(int argc, char **argv) {
    const int nx = 720;
    const int ny = 480;
    const int ns = 100;

    // Scene Configuration
    hitable *world = two_perlin_spheres();

    // Camera
    vec3 lookfrom(13,2,3);
    vec3 lookat(0,0,0);
    float dist_to_focus = 10.0;
    float aperture = 0.0;
    camera cam(lookfrom, lookat, vec3(0,1,0), 20, float(nx)/float(ny),
                aperture, dist_to_focus, 0.0, 1.0);

    // Rendering and Output
    pcs_bar bar;
    std::cout << "\033[1;33mStart rendering...\033[0m\n";
    std::fstream fout("result.ppm", std::fstream::trunc | std::fstream::out);
    fout << "P3\n" << nx << " " << ny << "\n255\n";
    for (int y = ny-1; y >= 0; y--) {
        for (int x = 0; x < nx; x++) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float u = float(x + drand48()) / float(nx);
                float v = float(y + drand48()) / float(ny);

                ray r = cam.get_ray(u, v);
                col += color_at(r, world, 0);
            }
            col /= float(ns);
            // gamma correction:
            col = vec3(std::sqrt(col.x), std::sqrt(col.y), std::sqrt(col.z));

            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);

            fout << ir << " " << ig << " " << ib << "\n";

            bar.update(((ny-y-1)*nx+x) * 100.0 / (ny * nx));
        }
    }
    return 0;
}

// Previous test scene:
// hitable *list[4];
// list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
// list[1] = new sphere(vec3(0, -100.5, -1), 100.0, new lambertian(vec3(0.8, 0.8, 0.0)));
// list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
// list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
// hitable *world = new hitable_list(list, 4);
