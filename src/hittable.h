#ifndef HITTABLE_H
#define HITTABLE_H

#include <memory>
#include "ray.h"

class aabb {
public:
    point3 min, max;
    aabb() : min(infinity, infinity, infinity), max(-infinity, -infinity, -infinity) {}
    aabb(const point3& a, const point3& b) : min(a), max(b) {}
    aabb(const aabb& box1, const aabb& box2) {
        min = point3(fmin(box1.min.x(), box2.min.x()), fmin(box1.min.y(), box2.min.y()), fmin(box1.min.z(), box2.min.z()));
        max = point3(fmax(box1.max.x(), box2.max.x()), fmax(box1.max.y(), box2.max.y()), fmax(box1.max.z(), box2.max.z()));
    }
    bool hit(const ray& r, interval ray_t) const {
        for (int a = 0; a < 3; a++) {
            auto invD = 1.0 / r.direction()[a];
            auto t0 = (min[a] - r.origin()[a]) * invD;
            auto t1 = (max[a] - r.origin()[a]) * invD;
            if (invD < 0.0) std::swap(t0, t1);
            if (t0 > ray_t.min) ray_t.min = t0;
            if (t1 < ray_t.max) ray_t.max = t1;
            if (ray_t.max <= ray_t.min) return false;
        }
        return true;
    }
};

class material;

class hit_record {
public:
    point3 p;
    vec3 normal;
    std::shared_ptr<material> mat;
    double t;
	double u, v;

    // if the intersected face orients to the viewer
    bool front_face;

    // set normal points to the viewer 
    void set_face_normal(const ray& r, const vec3& outward_normal) {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }

};

// an abstract class for objects can be hit.
// all hittable objects, such as sphere, triangle, should inherit from it
class hittable {
public:
    virtual ~hittable() = default;
	virtual aabb bounding_box() const = 0;

    //virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};
#endif