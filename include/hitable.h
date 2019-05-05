#ifndef HITABLE_H
#define HITABLE_H

#include "ray.h"
#include "aabb.h"

class Material;

struct hit_record {
    double t;
    Vec3 p;
    Vec3 normal;
    Material *mat_ptr;
};

class Hitable {

    public:

        virtual bool hit(const Ray& r, double t_min, double t_max,
                                                     hit_record& rec) const = 0;
        virtual bool bounding_box(double t0, double t1, aabb& box) const = 0;
};

#endif