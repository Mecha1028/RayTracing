#include "rtweekend.h"
#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "triangle.h"
#include "sphere.h"
#include "bvh.h"
#include "texture.h"
#include "point_light.h"
#include "dielectric.h"

int main() {
    hittable_list world;

    auto checker = make_shared<checker_texture>(0.2, color(0.8, 0.8, 0.8), color(0.2, 0.2, 0.2));
    auto ground_mat = make_shared<textured_lambertian>(checker);
    auto red_mat = make_shared<lambertian>(color(0.8, 0.2, 0.2));
    auto grey_mat = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    auto metal_mat = make_shared<metal>(color(0.9, 0.9, 0.9), 0.1);
    auto glass_mat = make_shared<dielectric>(1.5);

    // Single large triangle for ground (covers entire visible area from camera)
    point3 g0(-100, -2, -100), g1(100, -2, -100), g2(100, -2, 100);
    world.add(make_shared<triangle>(g0, g1, g2, ground_mat));

    // Red triangle
    point3 v0(-1.5, -1, -5), v1(1.5, -1, -5), v2(0, 2, -5);
    world.add(make_shared<triangle>(v0, v1, v2, 0, 0, 1, 0, 0.5, 1, red_mat));

    // Spheres
    world.add(make_shared<sphere>(point3(-2.5, -1, -4), 0.8, grey_mat));
    world.add(make_shared<sphere>(point3(2.5, -1, -4), 0.8, metal_mat));
    world.add(make_shared<sphere>(point3(0, -0.8, -3.5), 0.6, glass_mat));

    bvh_node bvh(world);
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 50;
    cam.max_depth = 10;
    cam.lights.push_back(point_light(point3(5, 5, 0), color(1.2, 1.2, 1.2)));
    cam.lights.push_back(point_light(point3(-3, 4, -2), color(0.8, 0.8, 1)));
    cam.render(bvh);
    return 0;
}