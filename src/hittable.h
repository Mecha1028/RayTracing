#ifndef HITTABLE_H
#define HITTABLE_H

//#include "ray.h"

class hit_record {
public:
    point3 p;
    vec3 normal;
    double t;

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

    //virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};
#endif