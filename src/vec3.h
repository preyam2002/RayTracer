//
// Created by Preyam Rao on 31/08/24.
//

#ifndef RAYTRACER_VEC3_H
#define RAYTRACER_VEC3_H

#include <cmath>
#include <iostream>

class vec3 {
private:
    double coordinates[3];
public:
    vec3() : coordinates{0, 0, 0} {}

    vec3(double x, double y, double z) : coordinates{x, y, z} {}

    [[nodiscard]] double x() const {
        return coordinates[0];
    }

    [[nodiscard]] double y() const {
        return coordinates[1];
    }

    [[nodiscard]] double z() const {
        return coordinates[2];
    }

    vec3 operator-() const {
        return {-coordinates[0], -coordinates[1], -coordinates[2]};
    }

    double operator[](int i) const {
        return coordinates[i];
    }

    double &operator[](int i) {
        return coordinates[i];
    }

    vec3 &operator+=(const vec3 &other_vector) {
        coordinates[0] += other_vector[0];
        coordinates[1] += other_vector[1];
        coordinates[2] += other_vector[2];
        return *this;
    }

    vec3 &operator*=(double x) {
        coordinates[0] *= x;
        coordinates[1] *= x;
        coordinates[2] *= x;
        return *this;
    }

    vec3 &operator/=(double x) {
        return *this *= (1 / x);
    }

    [[nodiscard]] double length_squared() const {
        return x() * x() + y() * y() + z() * z();
    }

    [[nodiscard]] double length() const {
        return sqrt(length_squared());
    }

    [[nodiscard]] bool near_zero() const{
        auto s = 1e-8;
        return fabs(x())<s && fabs(y())<s && fabs(z())<s;
    }
    static vec3 random(){
        return {random_double(), random_double(), random_double()};
    }

    static vec3 random(double min, double max){
        return {random_double(min, max), random_double(min, max), random_double(min, max)};
    }
};

using point3 = vec3;

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
    return out << v[0] << " " << v[1] << " " << v[2] << "\n";
}

inline vec3 operator+(const vec3 &v1, const vec3 &v2) {
    return {v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z()};
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2) {
    return {v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z()};
}

inline vec3 operator*(const vec3 &v, double t) {
    return {v.x() * t, v.y() * t, v.z() * t};
}

inline vec3 operator*(double t, const vec3 &v) {
    return v * t;
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2) {
    return {v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z()};
}

inline vec3 operator/(const vec3 &v, double t) {
    return v * (1 / t);
}

inline double dot(const vec3 &v1, const vec3 &v2) {
    return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
}

inline vec3 cross(const vec3 &v1, const vec3 &v2) {
    return {v1.y() * v2.z() - v1.z() * v2.y(),
            v1.z() * v2.x() - v1.x() * v2.z(),
            v1.x() * v2.y() - v1.y() * v2.x()};
}

inline vec3 unit_vector(const vec3 &v) {
    return v / v.length();
}

inline vec3 random_unit_vector(){
    while(true){
        auto p = vec3::random(-1,1);
        auto lensq = p.length_squared();
        if(1e-160<lensq and lensq <=1){
            return p/sqrt(lensq);
        }
    }
}

inline vec3 random_in_disk(){
    while(true){
        auto p = vec3(random_double(-1,1), random_double(-1,1),0);
        if(p.length_squared()<1){
            return p;
        }
    }
}

inline vec3 random_on_hemisphere(const vec3& normal){
    auto v = random_unit_vector();
    if(dot(random_unit_vector(),normal)>0.0){
        return v;
    }
    return -v;
}

inline vec3 reflect(const vec3& v, const vec3& n){
    return v-2*dot(v,n)*n;
}

inline vec3 refract(const vec3&uv, const vec3& n, double eta_by_eta_prime){
    auto cos_theta = std::fmin(dot(-uv,n), 1.0);
    vec3 r_per = eta_by_eta_prime*(uv + cos_theta*n);
    vec3 r_par = -sqrt(fabs(1-r_per.length_squared()))*n;
    return r_per + r_par;
}
#endif //RAYTRACER_VEC3_H
