#ifndef BVH_H
#define BVH_H

#include "hittable.h"
#include "hittable_list.h"
#include <algorithm>

// Comparator functions for BVH
inline bool box_x_compare(const shared_ptr<hittable>& a, const shared_ptr<hittable>& b) {
    return a->bounding_box().min.x() < b->bounding_box().min.x();
}
inline bool box_y_compare(const shared_ptr<hittable>& a, const shared_ptr<hittable>& b) {
    return a->bounding_box().min.y() < b->bounding_box().min.y();
}
inline bool box_z_compare(const shared_ptr<hittable>& a, const shared_ptr<hittable>& b) {
    return a->bounding_box().min.z() < b->bounding_box().min.z();
}

class bvh_node : public hittable {
public:
    shared_ptr<hittable> left;
    shared_ptr<hittable> right;
    aabb bbox;

    bvh_node() {}
    bvh_node(hittable_list& list) : bvh_node(list.objects, 0, list.objects.size()) {}

    bvh_node(std::vector<shared_ptr<hittable>>& objects, size_t start, size_t end) {
        int axis = random_int(0, 2);
        auto comparator = (axis == 0) ? box_x_compare : (axis == 1) ? box_y_compare : box_z_compare;
        size_t object_span = end - start;
        if (object_span == 1) {
            left = right = objects[start];
        }
        else if (object_span == 2) {
            if (comparator(objects[start], objects[start + 1])) {
                left = objects[start];
                right = objects[start + 1];
            }
            else {
                left = objects[start + 1];
                right = objects[start];
            }
        }
        else {
            std::sort(objects.begin() + start, objects.begin() + end, comparator);
            auto mid = start + object_span / 2;
            left = make_shared<bvh_node>(objects, start, mid);
            right = make_shared<bvh_node>(objects, mid, end);
        }
        bbox = aabb(left->bounding_box(), right->bounding_box());
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        if (!bbox.hit(r, ray_t)) return false;
        hit_record left_rec, right_rec;
        bool hit_left = left->hit(r, ray_t, left_rec);
        bool hit_right = right->hit(r, interval(ray_t.min, hit_left ? left_rec.t : ray_t.max), right_rec);
        if (hit_right) {
            rec = right_rec;
            return true;
        }
        if (hit_left) {
            rec = left_rec;
            return true;
        }
        return false;
    }

    aabb bounding_box() const override { return bbox; }
};

#endif