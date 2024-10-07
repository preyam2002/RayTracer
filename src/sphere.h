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
    aabb bbox;

public:
    //moving sphere
    sphere(const point3& center_1, const point3& center_2, double radius, shared_ptr<material>mat)
            : current_center_(center_1, center_2 - center_1),radius_(fmax(0, radius)), mat_(std::move(mat)) {
        auto r_vec = vec3(radius, radius, radius);
        aabb box1 = aabb(current_center_.at(0) - r_vec, current_center_.at(0) + r_vec);
        aabb box2 = aabb(current_center_.at(1) - r_vec, current_center_.at(1) + r_vec);
        bbox = aabb(box1, box2);
    }

    //stationary sphere
    sphere(const point3& static_center, double radius, shared_ptr<material>mat)
        : current_center_(static_center,vec3(0,0,0)),radius_(fmax(0, radius)), mat_(std::move(mat)) {
        auto r_vec = vec3(radius, radius, radius);
        bbox = aabb(static_center - r_vec, static_center + r_vec);
    }

    [[nodiscard]] aabb bounding_box() const override{
        return bbox;
    }

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
        vec3 outward_normal = (record.p - current_center_.at(r.time())) / radius_;
        record.set_face_normal(r, outward_normal);
        record.mat = mat_;
        get_sphere_uv(outward_normal, record.u, record.v);
        return true;
    }

    static void get_sphere_uv(const point3& p, double& u, double& v) {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

        auto theta = std::acos(-p.y());
        auto phi = std::atan2(-p.z(), p.x()) + pi;

        u = phi / (2*pi);
        v = theta / pi;
    }
};

#endif //RAYTRACER_SPHERE_H
