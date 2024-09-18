//
// Created by Preyam Rao on 01/09/24.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "hittable.h"
#include <cmath>
#include <utility>

class sphere : public hittable {
private:
    ray current_center_;
    double radius_;
    shared_ptr<material> mat_;
public:
    //moving sphere
    sphere(const point3& center_1, const point3& center_2, double radius, shared_ptr<material>mat)
            : current_center_(center_1, center_2 - center_1),radius_(fmax(0, radius)), mat_(std::move(mat)) {}

    //stationary sphere
    sphere(const point3& static_center, double radius, shared_ptr<material>mat)
        : current_center_(static_center,vec3(0,0,0)),radius_(fmax(0, radius)), mat_(std::move(mat)) {}

    bool hit(const ray &r, const interval & ray_t, hit_record &record) const override {
        vec3 origin_to_center = current_center_.at(r.time()) - r.origin();
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), origin_to_center);
        auto c = origin_to_center.length_squared() - radius_ * radius_;
        auto discriminant = h * h - a * c;
        if (discriminant < 0) {
            return false;
        }
        auto sqrtd = std::sqrt(discriminant);
        auto root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (h + sqrtd) / a;
            if(!ray_t.surrounds(root)) {
                return false;
            }
        }
        record.t = root;
        record.p = r.at(root);
        record.set_face_normal(r, (record.p - current_center_.at(r.time())) / radius_);
        record.mat = mat_;
        return true;
    }
};

#endif //RAYTRACER_SPHERE_H
