#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "texture.h"

class material {
public:
    virtual ~material() = default;

    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
        return false;
    }

    virtual color emitted(double u, double v, const point3& p) const {
        return color(0, 0, 0);
    }

    virtual color albedo(double u, double v, const point3& p) const {
        return color(1, 1, 1);
    }
};

class lambertian : public material {
private:
    color albedo_color;
public:
    lambertian(const color& albedo) : albedo_color(albedo) {}

    color albedo(double u, double v, const point3& p) const override {
        return albedo_color;
    }

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        auto scatter_direction = rec.normal + random_unit_vector();
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;
        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo_color;
        return true;
    }
};

class textured_lambertian : public material {
private:
    shared_ptr<texture> tex;
public:
    textured_lambertian(shared_ptr<texture> t) : tex(t) {}

    color albedo(double u, double v, const point3& p) const override {
        return tex->value(u, v, p);
    }

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        auto scatter_direction = rec.normal + random_unit_vector();
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;
        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo(rec.u, rec.v, rec.p);
        return true;
    }
};

class metal : public material {
private:
    color albedo_color;
    double fuzz;
public:
    metal(const color& albedo, double fuzz) : albedo_color(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    color albedo(double u, double v, const point3& p) const override {
        return albedo_color;
    }

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        vec3 random_in_sphere;
        do {
            random_in_sphere = vec3::random(-1, 1);
        } while (random_in_sphere.length_squared() >= 1);
        scattered = ray(rec.p, reflected + fuzz * random_in_sphere);
        attenuation = albedo_color;
        return true;
    }
};

#endif