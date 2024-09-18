//
// Created by Preyam Rao on 01/09/24.
//

#ifndef RAYTRACER_INTERVAL_H
#define RAYTRACER_INTERVAL_H

#include "rayTracer.h"

class interval{
private:
    double min_, max_;
public:
    interval() : min_(+infinity), max_(-infinity) {}
    interval(double min, double max) : min_(min), max_(max) {}

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
    static const interval empty, universe;
};

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);

#endif //RAYTRACER_INTERVAL_H
