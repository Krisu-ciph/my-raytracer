#include <iostream>
#include <limits>
#include <fstream>

#include "vec3.hpp"
#include "ray.hpp"
#include "hitable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "pcs_bar.hpp"

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

hitable *random_scene();

int main(int argc, char **argv) {
    const int nx = 1200;
    const int ny = 800;
    const int ns = 100;

    // Scene Configuration
    hitable *world = random_scene();

    // Camera
    vec3 lookfrom(13,2,3);
    vec3 lookat(0,0,-1);
    float dist_to_focus = (lookfrom - lookat).length();
    float aperture = 0.0;
    camera cam(lookfrom, lookat, vec3(0,1,0), 20, float(nx)/float(ny),
                aperture, dist_to_focus);

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


hitable *random_scene() {
    int n = 500;
    hitable **list = new hitable*[n + 1];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));

    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = drand48(); // Randomly choose material
            vec3 center(a+0.9*drand48(), 0.2, b+0.9*drand48());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                if (choose_mat < 0.5) { // Diffuse
                    #define _temp (drand48() * drand48())
                    list[i++] = new sphere(center, 0.2,
                        new lambertian(vec3(_temp, _temp, _temp)));
                    #undef _temp
                } else if (choose_mat < 0.7) {// Metal
                    #define _temp (0.5*(1 + drand48()))
                    list[i++] = new sphere(center, 0.2,
                        new metal(vec3(_temp, _temp, _temp), 0.5*drand48()));
                    #undef _temp
                } else { // Glass
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new hitable_list(list, i);
}

// Previous test scene:
// hitable *list[4];
// list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
// list[1] = new sphere(vec3(0, -100.5, -1), 100.0, new lambertian(vec3(0.8, 0.8, 0.0)));
// list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
// list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
// hitable *world = new hitable_list(list, 4);
