//
// Created by Preyam Rao on 01/09/24.
//

#ifndef RAYTRACER_HITTABLES_H
#define RAYTRACER_HITTABLES_H

#include<vector>
#include "hittable.h"

using std::make_shared;
using std::shared_ptr;

class hittables: public hittable{
private:
    aabb bbox;
public:
    std::vector<shared_ptr<hittable>> hittables_;
    hittables() = default;

    explicit hittables(const shared_ptr<hittable>& hittable){
        add(hittable);
    }

    void add(const shared_ptr<hittable>& hittable){
        hittables_.push_back(hittable);
        bbox = aabb(bbox, hittable->bounding_box());
    }

    void clear(){
        hittables_.clear();
    }

    [[nodiscard]] aabb bounding_box() const override{
        return bbox;
    }

    std::vector<shared_ptr<hittable>>& get_hittables(){
        return hittables_;
    }
    bool hit(const ray& r, const interval& ray_t, hit_record& hr) const override{
        hit_record temp_rec;
        bool has_hit = false;
        auto closest_so_far = ray_t.max();

        for(const auto& hittable: hittables_){
            if(hittable->hit(r, interval(ray_t.min(), closest_so_far), temp_rec)){
                has_hit = true;
                closest_so_far = temp_rec.t;
                hr = temp_rec;
            }
        }
        return has_hit;
    }
};


#endif //RAYTRACER_HITTABLES_H
