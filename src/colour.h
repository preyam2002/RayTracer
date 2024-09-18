//
// Created by Preyam Rao on 31/08/24.
//

#ifndef RAYTRACER_COLOUR_H
#define RAYTRACER_COLOUR_H

#include "interval.h"

using colour = vec3;

inline double linear_to_gamma(double col){
    if(col>0){
        return std::sqrt(col);
    }
    return 0;
}

void print_colour(std::ostream & out, const colour& pixel_colour){
    double r = pixel_colour.x();
    double g = pixel_colour.y();
    double b = pixel_colour.z();

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);
    static const interval intensity(0.000,0.999);
    int r_byte = int(255.9999 * intensity.clamp(r));
    int g_byte = int(255.9999 * intensity.clamp(g));
    int b_byte = int(255.9999 * intensity.clamp(b));
    out<<r_byte<<" "<< g_byte<<" "<<b_byte<<"\n";
}

#endif //RAYTRACER_COLOUR_H
