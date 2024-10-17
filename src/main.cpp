#include "rayTracer.h"
#include "hittable.h"
#include "hittables.h"
#include "sphere.h"
#include "bvh_node.h"
#include "quad.h"
#include "constant_medium.h"

void simple(){
    hittables world;
    auto material_ground = make_shared<lambertian>(colour(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(colour(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.5);
    auto material_bubble = make_shared<dielectric>(1.00 / 1.5);
    auto material_right  = make_shared<metal>(1.0, colour(0.8, 0.6, 0.2));
    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.2),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.4, material_bubble));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));
    camera cam;
    cam.background        = colour(0.70, 0.80, 1.00);
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.vfov     = 20;
    cam.look_from = point3(-2,2,1);
    cam.look_at   = point3(0,0,-1);
    cam.vup      = vec3(0,1,0);
    cam.defocus_angle = 10.0;
    cam.focus_dist    = 3.4;
    cam.render(world);
}

void spheres(bool isBounce) {
    hittables world;
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
                    // moving spheres
                    if(isBounce) {
                        auto center2 = center + point3(0, random_double(0, .5), 0);
                        world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
                    }
                    else{
                        world.add(make_shared<sphere>(center, 0.2, sphere_material));
                    }
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
    cam.background        = colour(0.70, 0.80, 1.00);
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
}

void checkered_spheres() {
    hittables world;
    auto checker = make_shared<checker_texture>(0.32, colour(.2, .3, .1), colour(.9, .9, .9));
    world.add(make_shared<sphere>(point3(0,-10, 0), 10, make_shared<lambertian>(checker)));
    world.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));
    camera cam;
    cam.background        = colour(0.70, 0.80, 1.00);
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.vfov     = 20;
    cam.look_from = point3(13,2,3);
    cam.look_at   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);
    cam.defocus_angle = 0;
    cam.render(world);
}

void earth() {
    auto earth_texture = make_shared<image_texture>("worldmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0,0,0), 2, earth_surface);

    camera cam;
    cam.background        = colour(0.70, 0.80, 1.00);
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 50;
    cam.max_depth         = 50;
    cam.vfov     = 20;
    cam.look_from = point3(0,0,12);
    cam.look_at   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);
    cam.defocus_angle = 0;
    cam.render(hittables(globe));
}

void perlin_spheres() {
    hittables world;

    auto pertext = make_shared<noise_texture>(4);
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<lambertian>(pertext)));

    camera cam;
    cam.background        = colour(0.70, 0.80, 1.00);
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1000;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.vfov     = 20;
    cam.look_from = point3(13,2,3);
    cam.look_at   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);
    cam.defocus_angle = 0;
    cam.render(world);
}

void quads() {
    hittables world;

    // Materials
    auto left_red     = make_shared<lambertian>(colour(1.0, 0.2, 0.2));
    auto back_green   = make_shared<lambertian>(colour(0.2, 1.0, 0.2));
    auto right_blue   = make_shared<lambertian>(colour(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(colour(1.0, 0.5, 0.0));
    auto lower_teal   = make_shared<lambertian>(colour(0.2, 0.8, 0.8));

    // Quads
    world.add(make_shared<quad>(point3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), left_red));
    world.add(make_shared<quad>(point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.add(make_shared<quad>(point3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(make_shared<quad>(point3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), lower_teal));

    camera cam;
    cam.background        = colour(0.70, 0.80, 1.00);
    cam.aspect_ratio      = 1.0;
    cam.image_width       = 1000;
    cam.samples_per_pixel = 500;
    cam.max_depth         = 500;
    cam.vfov     = 80;
    cam.look_from = point3(0,0,9);
    cam.look_at   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);
    cam.defocus_angle = 0;
    cam.render(world);
}

void simple_light() {
    hittables world;

    auto pertext = make_shared<noise_texture>(4);

    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<lambertian>(pertext)));

    auto difflight = make_shared<diffuse_light>(colour(4,4,4));
    world.add(make_shared<sphere>(point3(0,7,0), 2, difflight));
    world.add(make_shared<quad>(point3(3,1,-2), vec3(2,0,0), vec3(0,2,0), difflight));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1000;
    cam.samples_per_pixel = 500;
    cam.max_depth         = 50;
    cam.background        = colour(0,0,0);
    cam.vfov     = 20;
    cam.look_from = point3(26,3,6);
    cam.look_at   = point3(0,2,0);
    cam.vup      = vec3(0,1,0);
    cam.defocus_angle = 0;

    cam.render(world);
}

void cornell_box() {
    hittables world;

    auto red   = make_shared<lambertian>(colour(.65, .05, .05));
    auto white = make_shared<lambertian>(colour(.73, .73, .73));
    auto green = make_shared<lambertian>(colour(.12, .45, .15));
    auto light = make_shared<diffuse_light>(colour(15, 15, 15));

    world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light));
    world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));
    shared_ptr<hittable> box1 = box(point3(0,0,0), point3(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));
    world.add(box1);

    shared_ptr<hittable> box2 = box(point3(0,0,0), point3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));
    world.add(box2);

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 1000;
    cam.samples_per_pixel = 400;
    cam.max_depth         = 500;
    cam.background        = colour(0,0,0);
    cam.vfov     = 40;
    cam.look_from = point3(278, 278, -800);
    cam.look_at   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);
    cam.defocus_angle = 0;
    cam.render(world);
}

void cornell_smoke() {
    hittables world;

    auto red   = make_shared<lambertian>(colour(.65, .05, .05));
    auto white = make_shared<lambertian>(colour(.73, .73, .73));
    auto green = make_shared<lambertian>(colour(.12, .45, .15));
    auto light = make_shared<diffuse_light>(colour(7, 7, 7));

    world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(make_shared<quad>(point3(113,554,127), vec3(330,0,0), vec3(0,0,305), light));
    world.add(make_shared<quad>(point3(0,555,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    shared_ptr<hittable> box1 = box(point3(0,0,0), point3(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));

    shared_ptr<hittable> box2 = box(point3(0,0,0), point3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));

    world.add(make_shared<constant_medium>(box1, colour(0,0,0),0.01));
    world.add(make_shared<constant_medium>(box2, colour(1,1,1),0.01));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 1000;
    cam.samples_per_pixel = 200;
    cam.max_depth         = 50;
    cam.background        = colour(0,0,0);

    cam.vfov     = 40;
    cam.look_from = point3(278, 278, -800);
    cam.look_at   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}
void final_scene(int image_width, int samples_per_pixel, int max_depth) {
    hittables boxes1;
    auto ground = make_shared<lambertian>(colour(0.48, 0.83, 0.53));

    int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i*w;
            auto z0 = -1000.0 + j*w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1,101);
            auto z1 = z0 + w;

            boxes1.add(box(point3(x0,y0,z0), point3(x1,y1,z1), ground));
        }
    }

    hittables world;

    world.add(make_shared<bvh_node>(boxes1));

    auto light = make_shared<diffuse_light>(colour(7, 7, 7));
    world.add(make_shared<quad>(point3(123,554,147), vec3(300,0,0), vec3(0,0,265), light));

    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30,0,0);
    auto sphere_material = make_shared<lambertian>(colour(0.7, 0.3, 0.1));
    world.add(make_shared<sphere>(center1, center2, 50, sphere_material));

    world.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
    world.add(make_shared<sphere>(
            point3(0, 150, 145), 50, make_shared<metal>(1.0,colour(0.8, 0.8, 0.9))
    ));

    auto boundary = make_shared<sphere>(point3(360,150,145), 70, make_shared<dielectric>(1.5));
    world.add(boundary);
    world.add(make_shared<constant_medium>(boundary, colour(0.2, 0.4, 0.9), 0.2));
    boundary = make_shared<sphere>(point3(0,0,0), 5000, make_shared<dielectric>(1.5));
    world.add(make_shared<constant_medium>(boundary, colour(1,1,1), .0001));

    auto emat = make_shared<lambertian>(make_shared<image_texture>("worldmap.jpg"));
    world.add(make_shared<sphere>(point3(400,200,400), 100, emat));
    auto pertext = make_shared<noise_texture>(0.2);
    world.add(make_shared<sphere>(point3(220,280,300), 80, make_shared<lambertian>(pertext)));

    hittables boxes2;
    auto white = make_shared<lambertian>(colour(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(point3::random(0,165), 10, white));
    }

    world.add(make_shared<translate>(
                      make_shared<rotate_y>(
                              make_shared<bvh_node>(boxes2), 15),
                      vec3(-100,270,395)
              )
    );

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = image_width;
    cam.samples_per_pixel = samples_per_pixel;
    cam.max_depth         = max_depth;
    cam.background        = colour(0,0,0);

    cam.vfov     = 40;
    cam.look_from = point3(478, 278, -600);
    cam.look_at   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

int main(){
    const auto start_time= std::chrono::high_resolution_clock::now();
    freopen("image.ppm","w",stdout);
    switch(11){
        case 1: simple();break;
        case 2: spheres(false);break;
        case 3: checkered_spheres();break;
        case 4: earth();break;
        case 5: perlin_spheres();break;
        case 6: quads();break;
        case 7: simple_light();break;
        case 8: cornell_box();break;
        case 9: cornell_smoke();break;
        case 10: final_scene(400,   250,  4); break;
        case 11:  final_scene(800, 10000, 40); break;
    }
    auto time_taken = std::chrono::high_resolution_clock::now() - start_time;
    std::clog<<"Time taken is "<<(std::chrono::duration_cast<std::chrono::milliseconds>(time_taken).count())<<"ms";
    return 0;
}
