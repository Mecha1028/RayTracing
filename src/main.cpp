#include "rtweekend.h"

// ================================
#include "camera.h"
// ================================

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"


int main() {


    // Image
    //auto aspect_ratio = 16.0 / 9.0;
    //int image_width = 400;

    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    // Calculate the image height, and ensure that it's at least 1.
    //int image_height = int(image_width / aspect_ratio);
    //image_height = (image_height < 1) ? 1 : image_height;

    // =====================================================
    // World
    // adding two spheres to the world
    // you need to add triangle models in your coursework 2

    hittable_list world;

    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));
    // =====================================================

    cam.render(world);

}