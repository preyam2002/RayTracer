//
// Created by Preyam Rao on 07/10/24.
//

#ifndef RAYTRACER_QUAD_H
#define RAYTRACER_QUAD_H

#include "hittable.h"

class quad: public hittable{
private:
    point3 Q;
    vec3 u,v;
    vec3 normal;
    vec3 w;
    double D;
    shared_ptr<material>mat;
    aabb bbox;
    void set_bbox(){
        auto bbox_diagonal1 = aabb(Q, Q + u + v);
        auto bbox_diagonal2 = aabb(Q + u, Q + v);
        bbox = aabb(bbox_diagonal1, bbox_diagonal2);
    }
public:
    quad(const point3& Q, const vec3& u, const vec3& v, shared_ptr<material>mat) : Q(Q), u(u), v(v), mat(std::move(mat)) {
       set_bbox();
       auto n = cross(u,v);
       normal = unit_vector(n);
       D = dot(normal, Q);
       w = n / dot(n,n);
    }
    bool hit(const ray &r, const interval &ray_t, hit_record &record) const override{
        auto denom = dot(normal, r.direction());

        //parallel
        if(fabs(denom)<1e-8){
            return false;
        }

        auto t = (D - dot(normal, r.origin()))/denom;
        if(!ray_t.contains(t)){
            return false;
        }
        auto intersection = r.at(t);

        vec3 planar_hitpt_vector = intersection - Q;
        auto alpha = dot(w, cross(planar_hitpt_vector, v));
        auto beta = dot(w, cross(u, planar_hitpt_vector));
        if(!is_interior(alpha, beta, record)) return false;

        record.t = t;
        record.p = intersection;
        record.set_face_normal(r, normal);
        record.mat = mat;
        return true;
    }

    //can implement for other shapes
    static bool is_interior(double a, double b, hit_record& record) {
        interval unit_int = interval(0,1);
        if(!unit_int.contains(a) || !unit_int.contains(b)){
            return false;
        }
        record.u=a;
        record.v=b;
        return true;
    }

    aabb bounding_box() const override{
        return bbox;
    }
};

inline shared_ptr<hittables> box(const point3&a, const point3& b, const shared_ptr<material>&mat){
    auto sides = make_shared<hittables>();
    auto min=point3(std::fmin(a.x(), b.x()), std::fmin(a.y(), b.y()), std::fmin(a.z(), b.z()));
    auto max=point3(std::fmax(a.x(), b.x()), std::fmax(a.y(), b.y()), std::fmax(a.z(), b.z()));

    auto dx=vec3(max.x()-min.x(),0,0);
    auto dy=vec3(0,max.y()-min.y(),0);
    auto dz=vec3(0,0,max.z()-min.z());
    sides->add(make_shared<quad>(point3(min.x(), min.y(), max.z()),  dx,  dy, mat)); // front
    sides->add(make_shared<quad>(point3(max.x(), min.y(), max.z()), -dz,  dy, mat)); // right
    sides->add(make_shared<quad>(point3(max.x(), min.y(), min.z()), -dx,  dy, mat)); // back
    sides->add(make_shared<quad>(point3(min.x(), min.y(), min.z()),  dz,  dy, mat)); // left
    sides->add(make_shared<quad>(point3(min.x(), max.y(), max.z()),  dx, -dz, mat)); // top
    sides->add(make_shared<quad>(point3(min.x(), min.y(), min.z()),  dx,  dz, mat)); // bottom
    return sides;
}
#endif //RAYTRACER_QUAD_H
