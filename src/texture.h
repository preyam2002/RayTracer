//
// Created by Preyam Rao on 24/09/24.
//

#ifndef RAYTRACER_TEXTURE_H
#define RAYTRACER_TEXTURE_H

#include "vec3.h"
#include "colour.h"
#include "image_loader.h"
#include "perlin.h"

class texture{
public:
    virtual ~texture() = default;
    virtual colour value(double u, double v, const point3& p) const = 0;
};

class solid_colour : public texture{
public:
    explicit solid_colour(const colour& col) : albedo(col) {}
    solid_colour(double r, double g, double b) : albedo(colour(r,g,b)) {}
    colour value(double u, double v, const point3 &p) const override{
        return albedo;
    }
private:
    colour albedo;
};

class checker_texture : public texture{
public:
    checker_texture(double scale, const shared_ptr<texture>& even, const shared_ptr<texture>& odd):inv_scale(1.0/scale), even(even), odd(odd) {}
    checker_texture(double scale, const colour& even, const colour& odd):inv_scale(1/scale),
                even(make_shared<solid_colour>(even)), odd(make_shared<solid_colour>(odd)) {}
    colour value(double u, double v, const point3 &p) const override{
        auto x_int = int(floor(p.x()*inv_scale));
        auto y_int = int(floor(p.y()*inv_scale));
        auto z_int = int(floor(p.z()*inv_scale));
        auto isEven = (x_int + y_int + z_int)%2;
        return isEven ? even->value(u,v,p) : odd->value(u,v,p);
    }
private:
    double inv_scale;
    shared_ptr<texture>even;
    shared_ptr<texture>odd;
};

class image_texture : public texture{
public:
    explicit image_texture(const std::string& file_name) : image(file_name) {}
    [[nodiscard]] colour value(double u, double v, const point3 &p) const override{
        if(image.get_height()==0){
            return {0,1,1};
        }
        u = interval(0,1).clamp(u);
        v = 1.0 - interval(0,1).clamp(v);

        auto i = int(u * image.get_width());
        auto j = int(v * image.get_height());
        auto pixel = image.pixel_data(i,j);

        auto color_scale = 1.0 / 255.0;
        return {color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]};
    }

private:
    image_loader image;
};


class noise_texture: public texture{
public:
    noise_texture() : scale(1.0) {}

    explicit noise_texture(double scale):scale(scale) {}

    colour value(double u, double v, const point3 &p) const override{
//        marbled texture
//        return colour(.5, .5, .5) * (1 + std::sin(scale * p.z() + 10 * noise.turb(p, 7)));
        return colour(1,1,1) * noise.turb(p, 7);
    }
private:
    perlin noise;
    double scale;
};
#endif //RAYTRACER_TEXTURE_H
