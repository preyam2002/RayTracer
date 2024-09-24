//
// Created by Preyam Rao on 04/09/24.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H
#include "rayTracer.h"
#include "hittable.h"
#include "material.h"
class camera{
private:
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    double pixel_samples_scale = 0.1;
    point3 pixel00_loc;
    vec3 u,v,w;
    point3 camera_center;
    vec3 defocus_disk_u;
    vec3 defocus_disk_v;
    void initialize() {
        image_height = int(image_width / aspect_ratio); ;
        image_height = std::max(image_height, 1);
        pixel_samples_scale = 1.0/samples_per_pixel;
        camera_center = look_from;

        auto theta = deg_to_rad(vfov);
        auto h = std::tan(theta/2);

        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (((double) image_width) / image_height);

        w = unit_vector(look_from - look_at);
        u = unit_vector(cross(vup, w));
        v = cross(w,u);
        auto viewport_u = viewport_width * u;
        auto viewport_v = viewport_height * -v;

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        auto viewport_upper_left = camera_center - focus_dist*w
                                   - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
        auto defocus_radius = focus_dist * std::tan(deg_to_rad(defocus_angle / 2.0));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }
public:
    int max_depth = 10;
    double aspect_ratio = 1.0;
    int image_width = 100;
    int image_height = 100;
    int samples_per_pixel = 10;
    double vfov = 90;
    point3 look_from = point3(0,0,0);
    point3 look_at = point3(0,0,-1);
    vec3 vup = vec3(0,1,0);
    double defocus_angle = 10;
    double focus_dist = 10;

    void render(const hittable& world){
        initialize();
        std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
        for (int i = 0; i < image_height; i++) {
            std::clog << "\rCurrently running line number " << i << std::flush;
            for (int j = 0; j < image_width; j++) {
                colour pixel_colour = colour(0,0,0);
                for(int sample = 0 ; sample < samples_per_pixel ; sample++){
                    auto r= get_ray(i,j);
                    pixel_colour += ray_colour(r, max_depth, world);
                }
                print_colour(std::cout, pixel_colour*pixel_samples_scale);
            }
        }
        std::clog<<"\rDONE                                   \n";
    }

    ray get_ray(int i, int j){
        auto offset = get_sample_in_square();
        auto sample_pixel = pixel00_loc +
                (i + offset.y())*pixel_delta_v +
                (j + offset.x())*pixel_delta_u;
        auto ray_origin = (defocus_angle<=0) ? camera_center : get_sample_in_defocus_disk();
        auto ray_direction = sample_pixel - ray_origin;
        auto ray_time = random_double();
        return {ray_origin, ray_direction, ray_time};
    }

    [[nodiscard]] point3 get_sample_in_defocus_disk() const{
        auto p = random_in_disk();
        return camera_center + (p[0]*defocus_disk_u) + (p[1]*defocus_disk_v);
    }

    static vec3 get_sample_in_square(){
        return {random_double()-0.5, random_double()-0.5, 0.0};
    }

    colour ray_colour(const ray& r, int curr_depth, const hittable& world){
        if(curr_depth <= 0) return {0.0, 0.0, 0.0};
        hit_record record;
        if(world.hit(r, interval(0.001, infinity) ,record)){
            ray scattered;
            colour attenuation;
            if(record.mat->scatter(r, record, attenuation, scattered)){
                return attenuation*ray_colour(scattered, curr_depth - 1, world);
            }
            return {0, 0, 0};
        }
        vec3 unit_direction = unit_vector(r.direction());
        auto a = (unit_direction.y()+1)*0.5;
        return (1-a)*colour(1.0,1.0,1.0) + a*colour(0.5,0.7,1.0);
    }
};
#endif //RAYTRACER_CAMERA_H
