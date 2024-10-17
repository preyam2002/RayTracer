//
// Created by Preyam Rao on 01/09/24.
//

#ifndef RAYTRACER_INTERVAL_H
#define RAYTRACER_INTERVAL_H

#include "rayTracer.h"

class interval{
public:
    double min_, max_;
    interval() : min_(+infinity), max_(-infinity) {}
    interval(double min, double max) : min_(min), max_(max) {}
    interval(const interval& a, const interval& b){
        min_ = std::fmin(a.min_, b.min_);
        max_ = std::fmax(a.max_, b.max_);
    }

    [[nodiscard]] bool contains(double x) const{
        return min_<=x && x<=max_;
    }
    [[nodiscard]] bool surrounds(double  x) const{
        return min_<x && x<max_;
    }
    [[nodiscard]] double clamp (double x) const{
        return std::fmin(std::fmax(min_, x),max_);
    }
    [[nodiscard]] double size() const{
        return max_-min_;
    }
    [[nodiscard]] const double& max() const{
        return max_;
    }
    [[nodiscard]] const double& min() const{
        return min_;
    }
    [[nodiscard]] interval expand(double delta) const{
        auto padding = delta/2;
        return {min_-padding, max_ +padding};
    }
    static const interval empty, universe;
};

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);
interval operator +(const interval& interval1, double offset){
    return {interval1.min_+offset, interval1.max_+offset};
}
interval operator +(double offset,const interval& interval1){
    return {interval1.min_+offset, interval1.max_+offset};
}
#endif //RAYTRACER_INTERVAL_H
