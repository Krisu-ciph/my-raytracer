#include <iostream>
#include <limits>
#include <fstream>

#include "vec3.hpp"
#include "ray.hpp"
#include "hitable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"

vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0*vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
    } while (p.lengthSquared() >= 1.0);
    return p;
}

vec3 color_at(const ray &r, hitable *world) {
    hit_record rec;
    if (world->hit(r, 0.001, std::numeric_limits<float>::max(), rec)) {
        // hit point + normal == origin of
        // a unit sphere tangent to the hit surface
        // plus another vector to get a random pos in the unit sphere
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        // send another ray recursively
        return 0.5 * color_at(ray(rec.p, target - rec.p), world);
    } else {
        vec3 unit_dir = unit(r.direction);
        float k = 0.5 * (unit_dir.y + 1.0);
        return (1.f-k)*vec3(1.0, 1.0, 1.0) + k*vec3(0.5, 0.7, 1.0);
    }
}

int main() {
    const int nx = 400;
    const int ny = 200;
    const int ns = 100;

    // Scene Configuration
    hitable *list[2];
    list[0] = new sphere(vec3(0.0, 0.0, -1.0), 0.5f);
    list[1] = new sphere(vec3(0.0, -100.5f, -1.0), 100.0);
    hitable *world = new hitable_list(list, 2);

    // Camera
    camera cam;

    // Rendering
    std::fstream fout("result.ppm", std::fstream::trunc | std::fstream::out);
    fout << "P3\n" << nx << " " << ny << "\n255\n";
    for (int y = ny-1; y >= 0; y--) {
        for (int x = 0; x < nx; x++) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float u = float(x + drand48()) / float(nx);
                float v = float(y + drand48()) / float(ny);

                ray r = cam.get_ray(u, v);
                col += color_at(r, world);
            }
            col /= float(ns);
            // gamma correction:
            col = vec3(std::sqrt(col.x), std::sqrt(col.y), std::sqrt(col.z));

            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);

            fout << ir << " " << ig << " " << ib << "\n";
        }
    }

}
