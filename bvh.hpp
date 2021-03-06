#ifndef bvh_hpp
#define bvh_hpp

#include "hitable.hpp"

class bvh_node : public hitable {
public:
    bvh_node() = default;
    bvh_node(hitable **l, int n, float time0, float time1);

    virtual bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const;
    virtual bool bounding_box(float t0, float t1, aabb &box) const;

    hitable *left;
    hitable *right;
    aabb box;
};

bvh_node::bvh_node(hitable **l, int n, float time0, float time1) {
    // Randomly choose axis to sort by
    int axis = int(3*drand48());
    auto compare_by_axis =
    [axis](hitable *a, hitable *b) -> bool {
        aabb box_left, box_right;
        if (!a->bounding_box(0, 0, box_left) ||
            !b->bounding_box(0, 0, box_right)) {
            std::cerr << "No bounding box in bvh_node constructor\n";
            exit(-1);
        }
        return (box_left.min[axis] - box_right.min[axis] < 0.0);
    };
    std::sort(l, l + n, compare_by_axis);

    if (n == 1) {
        left = right = l[0];
    } else if (n == 2) {
        left = l[0];
        right = l[1];
    } else {
        // Recursively construct a bvh tree
        left = new bvh_node(l, n/2, time0, time1);
        right = new bvh_node(l + n/2, n - n/2, time0, time1);
    }
    // Calculating bounding box
    aabb box_left, box_right;
    if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right)) {
        std::cerr << "no bounding box in bvh_node constructor\n";
        exit(-1);
    }
    box = box_union(box_left, box_right);
}

bool bvh_node::hit(const ray &r, float tmin, float tmax, hit_record &rec) const {
    if (box.hit(r, tmin, tmax)) {
        hit_record left_rec, right_rec;
        bool hit_left = left->hit(r, tmin, tmax, left_rec);
        bool hit_right = right->hit(r, tmin, tmax, right_rec);

        if (hit_left && hit_right) {
            // Does hitting the bounding box earlier ensure that hitting object in the box earlier?
            if (left_rec.t < right_rec.t) {
                rec = left_rec;
            } else {
                rec = right_rec;
            }
            return true;
        } else if (hit_left) {
            rec = left_rec;
            return true;
        } else if (hit_right) {
            rec = right_rec;
            return true;
        } else {
            return false;
        }
    }
    return false;
}

bool bvh_node::bounding_box(float t0, float t1, aabb &b) const {
    b = box;
    return true;
}

#endif
