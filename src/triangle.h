#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"

class triangle : public hittable {
private:
    point3 v0, v1, v2;
    vec3 normal;
    shared_ptr<material> mat;
    double tu0, tv0, tu1, tv1, tu2, tv2;

public:
    triangle(const point3& a, const point3& b, const point3& c,
        shared_ptr<material> m)
        : v0(a), v1(b), v2(c), mat(m), tu0(0), tv0(0), tu1(1), tv1(0), tu2(0), tv2(1) {
        normal = unit_vector(cross(v1 - v0, v2 - v0));
    }

    triangle(const point3& a, const point3& b, const point3& c,
        double ua, double va, double ub, double vb, double uc, double vc,
        shared_ptr<material> m)
        : v0(a), v1(b), v2(c), mat(m),
        tu0(ua), tv0(va), tu1(ub), tv1(vb), tu2(uc), tv2(vc) {
        normal = unit_vector(cross(v1 - v0, v2 - v0));
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        vec3 e1 = v1 - v0;
        vec3 e2 = v2 - v0;
        vec3 pvec = cross(r.direction(), e2);
        double det = dot(e1, pvec);
        if (fabs(det) < 1e-8) return false;
        double inv_det = 1.0 / det;
        vec3 tvec = r.origin() - v0;
        double u = dot(tvec, pvec) * inv_det;
        if (u < 0 || u > 1) return false;
        vec3 qvec = cross(tvec, e1);
        double v = dot(r.direction(), qvec) * inv_det;
        if (v < 0 || u + v > 1) return false;
        double t = dot(e2, qvec) * inv_det;
        if (!ray_t.contains(t)) return false;

        rec.t = t;
        rec.p = r.at(t);
        rec.set_face_normal(r, normal);
        rec.mat = mat;
        rec.u = (1 - u - v) * tu0 + u * tu1 + v * tu2;
        rec.v = (1 - u - v) * tv0 + u * tv1 + v * tv2;
        return true;
    }

    aabb bounding_box() const override {
        aabb box;
        box.min = point3(fmin(fmin(v0.x(), v1.x()), v2.x()),
            fmin(fmin(v0.y(), v1.y()), v2.y()),
            fmin(fmin(v0.z(), v1.z()), v2.z()));
        box.max = point3(fmax(fmax(v0.x(), v1.x()), v2.x()),
            fmax(fmax(v0.y(), v1.y()), v2.y()),
            fmax(fmax(v0.z(), v1.z()), v2.z()));
        return box;
    }
};

#endif