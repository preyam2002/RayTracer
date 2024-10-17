//
// Created by Preyam Rao on 11/09/24.
//

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "hittable.h"
#include "texture.h"

class material{
public:
    virtual ~material() = default;
    virtual bool scatter(const ray& incident, const hit_record& rec,
                         colour& attenuation, ray& scattered) const{
        return false;
    }
    virtual colour emitted(double u, double v, const point3& p) const {
        return {0,0,0};
    }

};

class lambertian: public material{
private:
    shared_ptr<texture>tex;
public:
    explicit lambertian(const colour& albedo) : tex(make_shared<solid_colour>(albedo)) {}
    explicit lambertian(const shared_ptr<texture>& tex) : tex(tex) {}
    bool scatter(const ray& incident, const hit_record& rec,
                         colour& attenuation, ray& scattered) const override {
        auto scatter_direction = rec.normal + random_unit_vector();
        if(scatter_direction.near_zero()){
            scatter_direction = rec.normal;
        }
        scattered = ray(rec.p, scatter_direction, incident.time());
        attenuation = tex->value(rec.u, rec.v, rec.p);
        return true;
    }
};

class metal: public material{
private:
    colour albedo;
    double fuzz;
public:
    metal(const double& fuzz, const colour& albedo) : albedo(albedo), fuzz(fuzz) {}
    bool scatter(const ray& incident, const hit_record& rec,
                 colour& attenuation, ray& scattered) const override {
        vec3 reflected = reflect(incident.direction(), rec.normal);
        reflected = unit_vector(reflected) + random_unit_vector()*fuzz;
        scattered = ray(rec.p, reflected, incident.time());
        attenuation = albedo;
        return dot(scattered.direction(), rec.normal) > 0;
    }
};

class dielectric: public material{
private:
    double refractive_index;
    static double reflectance(double cosine, double refraction_index) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0*r0;
        return r0 + (1-r0)*std::pow((1 - cosine),5);
    }
public:
    explicit dielectric(const double& refractive_index) : refractive_index(refractive_index) {}

    bool scatter(const ray& incident, const hit_record& rec,
                 colour& attenuation, ray& scattered) const override {
        double ri = rec.front_face ? (1.0/refractive_index) : refractive_index;
        vec3 u_direction = unit_vector(incident.direction());
        double cos_theta = std::fmin(dot(-u_direction, rec.normal), 1);
        double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);
        bool cannot_refract = ri*sin_theta>1.0;
        vec3 direction;
        if(cannot_refract || reflectance(cos_theta, ri) > random_double()){
            direction = reflect(u_direction, rec.normal);
        }
        else{
            direction = refract(u_direction, rec.normal, ri);
        }
        scattered = ray(rec.p, direction, incident.time());
        attenuation = colour(1, 1, 1);
        return true;
    }
};

class diffuse_light:public material{
public:
    explicit diffuse_light(const colour& albedo) : tex(make_shared<solid_colour>(albedo)) {}
    explicit diffuse_light(const shared_ptr<texture>& tex) : tex(tex) {}
    colour emitted(double u, double v, const point3& p) const override {
        return tex->value(u,v,p);
    }
private:
    shared_ptr<texture>tex;
};


class isotropic:public material{
public:
    explicit isotropic(const colour& albedo):tex(make_shared<solid_colour>(albedo)) {}
    explicit isotropic(shared_ptr<texture>tex) : tex(std::move(tex)) {}
    bool scatter(const ray &incident, const hit_record &rec, colour &attenuation, ray &scattered) const override{
        scattered = ray(rec.p, random_unit_vector(), incident.time());
        attenuation = tex->value(rec.u, rec.v, rec.p);
        return true;
    }

private:
    shared_ptr<texture>tex;
};
#endif //RAYTRACER_MATERIAL_H
