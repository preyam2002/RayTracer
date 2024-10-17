//
// Created by Preyam Rao on 01/09/24.
//

#ifndef RAYTRACER_RAYTRACER_H
#define RAYTRACER_RAYTRACER_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
using std::shared_ptr;
using std::make_shared;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

static std::uniform_real_distribution<double> distribution(0.0, 1.0);
std::random_device rd;
static std::mt19937 generator(rd());

inline double deg_to_rad(double degrees){
    return (degrees*pi)/180.0;
}

inline double random_double() {
    return distribution(generator);
}
//inline double random_double() {
//    return std::rand() / (RAND_MAX + 1.0);
//}
inline double random_double(double min, double max){
    return min + (max - min)*(random_double());
}

inline int random_int(int min, int max) {
    return (int)random_double(min,max);
}

#include "vec3.h"
#include "colour.h"
#include "ray.h"
#include "camera.h"
#endif //RAYTRACER_RAYTRACER_H
