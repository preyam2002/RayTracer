//
// Created by Preyam Rao on 01/09/24.
//

#ifndef RAYTRACER_HITTABLE_H
#define RAYTRACER_HITTABLE_H

#include "vec3.h"
#include "ray.h"
#include "interval.h"
#include <cmath>
#include <iostream>

class material;
class hit_record{
public:
    point3 p;
    vec3 normal;
    double t{};
    bool front_face{};
    shared_ptr<material> mat;
    void set_face_normal(const ray& r, const vec3& outward_normal){
        front_face = dot(r.direction(), outward_normal)<0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable{
public:
    virtual ~hittable() = default;
    virtual bool hit(const ray& r, const interval& ray_t , hit_record& hr) const = 0;
};
#endif //RAYTRACER_HITTABLE_H
