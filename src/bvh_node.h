//
// Created by Preyam Rao on 19/09/24.
//

#ifndef RAYTRACER_BVH_NODE_H
#define RAYTRACER_BVH_NODE_H

#include "hittable.h"
#include "hittables.h"
#include "algorithm"

class bvh_node: public hittable{
public:
    explicit bvh_node(hittables hittable_list) : bvh_node(hittable_list.get_hittables(), 0, hittable_list.get_hittables().size()){}

    bvh_node(std::vector<shared_ptr<hittable>>& hittable_list, size_t begin, size_t end){
        bbox = aabb::empty;
        for (size_t object_index=begin; object_index < end; object_index++)
            bbox = aabb(bbox, hittable_list[object_index]->bounding_box());
        int axis = bbox.longest_axis();
        auto comparator = (axis==0) ? box_x_compare : (axis==1) ? box_y_compare : box_z_compare;
        auto length = end - begin;
        if(length==1){
            left = right = hittable_list[begin];
        }
        else if(length==2){
            left = hittable_list[begin];
            right = hittable_list[begin + 1];
        }
        else{
            auto mid = begin + length/2;
            sort(hittable_list.begin()+begin, hittable_list.begin()+end, comparator);
            left = make_shared<bvh_node>(hittable_list, begin, mid);
            right = make_shared<bvh_node>(hittable_list, mid, end);
        }
        bbox = aabb(left->bounding_box(), right->bounding_box());
    }

    bool hit(const ray &r, const interval &ray_t, hit_record &hr) const override{
        if(!bbox.hit(r, ray_t)){
            return false;
        }
        bool hit_left = left->hit(r, ray_t, hr);
        auto new_ray_t = interval(ray_t.min_, hit_left ? hr.t : ray_t.max_);
        bool hit_right = right->hit(r, new_ray_t, hr);
        return hit_right || hit_left;
    }

    [[nodiscard]] aabb bounding_box() const override{
        return bbox;
    }

    static bool box_compare(const shared_ptr<hittable>& a, const shared_ptr<hittable>& b, size_t axis){
        auto a_interval = a->bounding_box().get_idx(axis);
        auto b_interval = b->bounding_box().get_idx(axis);
        return a_interval.min() < b_interval.min();
    }

    static bool box_x_compare(const shared_ptr<hittable>& a, const shared_ptr<hittable>& b){
        return box_compare(a, b, 0);
    }

    static bool box_y_compare(const shared_ptr<hittable>& a, const shared_ptr<hittable>& b){
        return box_compare(a, b, 1);
    }

    static bool box_z_compare(const shared_ptr<hittable>& a, const shared_ptr<hittable>& b){
        return box_compare(a, b, 2);
    }

private:
    shared_ptr<hittable>left;
    shared_ptr<hittable>right;
    aabb bbox;
};
#endif //RAYTRACER_BVH_NODE_H
