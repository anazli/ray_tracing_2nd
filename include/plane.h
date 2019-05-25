#ifndef PLANE_H
#define PLANE_H

#include "hitable.h"

class Plane : public Hitable {

    public:

        Plane(){}
        Plane(Vec3 po, Vec3 n, Material *m) :
         point(po), normal(n), mat_ptr(m) {}
        
        virtual bool hit(const Ray& r, double t_min, double t_max,
                                                     hit_record& rec) const;
        virtual bool bounding_box(double t0, double t1, aabb& box) const
        {return true;}

        Vec3 point;
        Vec3 normal;
        Material *mat_ptr;
};


bool Plane::hit(const Ray& r, double t_min, double t_max,
                                             hit_record& rec)const
{
    double intersection = dot(normal, point - r.origin())/dot(normal, r.direction());
    if(intersection >= 0)
    {
        if(intersection < t_max && intersection > t_min)
        {
            rec.t = intersection;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = normal;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

#endif