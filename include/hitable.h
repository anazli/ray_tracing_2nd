#ifndef HITABLE_H
#define HITABLE_H

#include "ray.h"
#include "aabb.h"

class Material;

void get_sphere_uv(const Vec3& p, double& u, double& v) {
    double phi = atan2(p.z(), p.x());
    double theta = asin(p.y());
    u = 1-(phi + M_PI) / (2*M_PI);
    v = (theta + M_PI/2) / M_PI;
}

struct hit_record {
    double t;
    double u;
    double v;
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

class Flip_normals : public Hitable {
    public:
        Flip_normals(Hitable *p): ptr(p){}
        virtual bool hit(const Ray& r, double t_min, double t_max,
                                                     hit_record& rec) const
        {
            if(ptr->hit(r, t_min, t_max, rec))
            {
                rec.normal = -rec.normal;
                return true;
            }
            else
                return false;
        }
        virtual bool bounding_box(double t0, double t1, aabb& box) const 
        {
            return ptr->bounding_box(t0, t1, box);
        }
    private:
        Hitable *ptr;
};

#endif