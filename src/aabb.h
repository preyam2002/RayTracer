//
// Created by Preyam Rao on 19/09/24.
//

#ifndef RAYTRACER_AABB_H
#define RAYTRACER_AABB_H

#include "interval.h"

class aabb{
private:
    interval x,y,z;
    void pad_to_delta(){
        double delta=0.0001;
        if(x.size()<delta){
            x=x.expand(delta);
        }
        if(y.size()<delta){
            y=y.expand(delta);
        }
        if(z.size()<delta){
            z=z.expand(delta);
        }
    }
public:
    aabb() = default;

    aabb(const interval& x, const interval& y, const interval& z) : x(x), y(y), z(z) {
        pad_to_delta();
    }

    aabb(const point3& a, const point3& b){
        x=interval(std::fmin(a.x(), b.x()),std::fmax(a.x(), b.x()));
        y=interval(std::fmin(a.y(), b.y()),std::fmax(a.y(), b.y()));
        z=interval(std::fmin(a.z(), b.z()),std::fmax(a.z(), b.z()));
        pad_to_delta();
    }

    aabb(const aabb& a, const aabb& b){
        x = interval(a.x, b.x);
        y = interval(a.y, b.y);
        z = interval(a.z, b.z);
        pad_to_delta();
    }

    [[nodiscard]] interval get_idx(int i) const{
        if(i==0) return x;
        else if(i==1) return y;
        else return z;
    }

    [[nodiscard]] bool hit(const ray& r, interval ray_t) const{
        for(int i=0;i<3;i++){
            auto d_inv = 1.0/(r.direction()[i]);
            const interval& curr_interval = get_idx(i);
            auto ax_or = r.origin()[i];
            auto t0 = (curr_interval.min_ - ax_or)*d_inv;
            auto t1 = (curr_interval.max_ - ax_or)*d_inv;
            if(t0>t1) std::swap(t0,t1);
            ray_t.min_ = std::max(t0, ray_t.min_);
            ray_t.max_ = std::min(t1, ray_t.max_);
            if(ray_t.max() <= ray_t.min()){
                return false;
            }
        }
        return true;
    }
    [[nodiscard]] int longest_axis() const{
        if(x.size() > std::max(y.size(), z.size())){
            return 0;
        }
        if(y.size() > std::max(x.size(), z.size())){
            return 1;
        }
        return 2;
    }
    static const aabb empty, universe;
};
const aabb aabb::empty    = aabb(interval::empty,    interval::empty, interval::empty);
const aabb aabb::universe = aabb(interval::universe, interval::universe, interval::universe);
aabb operator+(const aabb& bbox, const vec3& offset){
    return {bbox.get_idx(0)+offset.x(), bbox.get_idx(1)+offset.y(), bbox.get_idx(2)+offset.z()};
}
aabb operator+(const vec3& offset, const aabb& bbox) {
    return {bbox.get_idx(0) + offset.x(), bbox.get_idx(1) + offset.y(), bbox.get_idx(2) + offset.z()};
}
#endif //RAYTRACER_AABB_H
