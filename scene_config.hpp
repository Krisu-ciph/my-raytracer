#ifndef scene_config_hpp
#define scene_config_hpp

hitable *random_scene_moving() {
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
                    list[i++] =
                    new moving_sphere(center, center+vec3(0, 0.5*drand48(), 0), 0.0, 1.0, 0.2, new lambertian(vec3(_temp, _temp, _temp)));
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

#endif
