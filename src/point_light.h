#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "vec3.h"

struct point_light {
    point3 position;
    color intensity;
    point_light(const point3& pos, const color& col) : position(pos), intensity(col) {}
};

#endif