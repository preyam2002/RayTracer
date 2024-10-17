//
// Created by Preyam Rao on 14/10/24.
//

#ifndef RAYTRACER_CONSTANT_MEDIUM_H
#define RAYTRACER_CONSTANT_MEDIUM_H

#include <utility>

#include "hittable.h"

class constant_medium: public hittable{
public:
    constant_medium(shared_ptr<hittable>boundary, shared_ptr<texture>mat, double density) : boundary(std::move(boundary)), phase_function(
            make_shared<isotropic>(std::move(mat))), neg_inv_density(-1.0/density) {}

    constant_medium(shared_ptr<hittable>boundary, const colour& albedo, double density) : boundary(std::move(boundary)), phase_function(
            make_shared<isotropic>(albedo)), neg_inv_density(-1.0/density) {}

    bool hit(const ray &r, const interval &ray_t, hit_record &hr) const override{
        hit_record rec1, rec2;
        if(!boundary->hit(r, interval::universe, rec1)){
            return false;
        }
        if(!boundary->hit(r, interval(rec1.t+0.0001, infinity), rec2)){
            return false;
        }
        rec1.t=std::fmax(rec1.t, ray_t.min());
        rec2.t=std::fmin(rec2.t, ray_t.max());
        if (rec1.t >= rec2.t)
            return false;
        if (rec1.t<0) rec1.t=0;
        auto ray_length = r.direction().length();
        auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
        auto hit_distance = neg_inv_density * std::log(random_double());

        if (hit_distance > distance_inside_boundary)
            return false;

        hr.t = rec1.t + hit_distance / ray_length;
        hr.p = r.at(hr.t);

        hr.normal = vec3(1,0,0);  // arbitrary
        hr.front_face = true;     // also arbitrary
        hr.mat = phase_function;

        return true;
    }

    aabb bounding_box() const override{
        return boundary->bounding_box();
    }
private:
    aabb bbox;
    shared_ptr<hittable> boundary;
    double neg_inv_density;
    shared_ptr<material> phase_function;
};
#endif //RAYTRACER_CONSTANT_MEDIUM_H
