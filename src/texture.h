#ifndef TEXTURE_H
#define TEXTURE_H

#include "rtweekend.h"

class texture {
public:
    virtual ~texture() = default;
    virtual color value(double u, double v, const point3& p) const = 0;
};

class checker_texture : public texture {
private:
    double scale;
    color even;
    color odd;
public:
    checker_texture(double scale, const color& even, const color& odd)
        : scale(scale), even(even), odd(odd) {
    }
    color value(double u, double v, const point3& p) const override {
        auto sines = sin(scale * p.x()) * sin(scale * p.y()) * sin(scale * p.z());
        if (sines < 0)
            return odd;
        else
            return even;
    }
};
#endif