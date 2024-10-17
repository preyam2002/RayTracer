//
// Created by Preyam Rao on 31/08/24.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

class ray{
private:
    point3 origin_;
    vec3 direction_;
    double time_;
public:
    ray() : origin_(), direction_(), time_() {}
    ray(const point3& origin, const vec3& direction, double time): origin_(origin), direction_(direction), time_(time){}
    ray(const point3& origin, const vec3& direction): origin_(origin), direction_(direction), time_(0){}
    ray(const ray& r, const vec3& offset): origin_(r.origin_-offset), direction_(r.direction_), time_(r.time_) {}
    [[nodiscard]] const point3& origin() const{
        return origin_;
    }
    [[nodiscard]] double time() const{
        return time_;
    }
    [[nodiscard]] const vec3& direction() const{
        return direction_;
    }
    [[nodiscard]] point3 at(const double& t) const{
        return origin_ + t*direction_;
    }
};
#endif //RAYTRACER_RAY_H
