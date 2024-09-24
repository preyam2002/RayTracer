#include "rayTracer.h"
#include "hittable.h"
#include "hittables.h"
#include "sphere.h"
#include "bvh_node.h"

int main() {
    const auto start_time= std::chrono::high_resolution_clock::now();
    freopen("image.ppm","w",stdout);
    hittables world;

//    auto R = std::cos(pi/4);
//    auto material_left  = make_shared<lambertian>(colour(0,0,1));
//    auto material_right = make_shared<lambertian>(colour(1,0,0));
//    world.add(make_shared<sphere>(point3(-R, 0, -1), R, material_left));
//    world.add(make_shared<sphere>(point3( R, 0, -1), R, material_right));

//    auto material_ground = make_shared<lambertian>(colour(0.8, 0.8, 0.0));
//    auto material_center = make_shared<lambertian>(colour(0.1, 0.2, 0.5));
//    auto material_left   = make_shared<dielectric>(1.5);
//    auto material_bubble = make_shared<dielectric>(1.00 / 1.5);
//    auto material_right  = make_shared<metal>(1.0, colour(0.8, 0.6, 0.2));
//    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
//    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.2),   0.5, material_center));
//    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.4, material_bubble));
//    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
//    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    auto ground_material = make_shared<lambertian>(colour(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());
            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;
                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = colour::random() * colour::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    //moving spheres
//                    auto center2 = center + vec3(0, random_double(0,.5), 0);
//                    world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = colour::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(fuzz,albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(colour(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(0.0,colour(0.7, 0.6, 0.5));
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
    world = hittables(make_shared<bvh_node>(world));
    camera cam;
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 50;
    cam.max_depth         = 50;
    cam.vfov     = 20;
    cam.look_from = point3(13,2,3);
    cam.look_at   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);
    cam.defocus_angle = 0.60;
    cam.focus_dist    = 10.0;

    cam.render(world);
    auto time_taken = std::chrono::high_resolution_clock::now() - start_time;
    std::clog<<"Time taken is "<<(std::chrono::duration_cast<std::chrono::milliseconds>(time_taken).count())<<"ms";
    return 0;
}
