#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material {
public:
    virtual ~material() = default;

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const {
        return false;
    }
};

class lambertian : public material {
private:
    color albedo;
public:
    lambertian(const color& albedo) : albedo(albedo) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
        auto scatter_direction = rec.normal + random_unit_vector();

        // Catch degenerate scatter direction
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }
};

class metal : public material {
private:
    color albedo;
    double fuzz;

public:
    metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
        vec3 reflected = reflect(r_in.direction(), rec.normal);

        // Generate a random vector inside the unit sphere using rejection sampling
        vec3 random_in_sphere;
        do {
            random_in_sphere = vec3::random(-1, 1);
        } while (random_in_sphere.length_squared() >= 1);

        // Perturb the reflected ray by the fuzz factor times the random vector
        scattered = ray(rec.p, reflected + fuzz * random_in_sphere);
        attenuation = albedo;
        return true;
    }
};

#endif