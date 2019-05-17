#ifndef SPHERE_H
#define SPHERE_H

#include "hitable.h"

class Sphere : public Hitable {

    public:

        Sphere(){}
        Sphere(Vec3 cen, double r, Material *m) : center(cen), radius(r),
                                                             mat_ptr(m) {}
        
        virtual bool hit(const Ray& r, double t_min, double t_max,
                                                     hit_record& rec) const;
        
        virtual bool bounding_box(double t0, double t1, aabb& box) const;

        Vec3 center;
        double radius;
        Material *mat_ptr;
};


bool Sphere::hit(const Ray& r, double t_min, double t_max,
                                             hit_record& rec)const
{
    Vec3 oc = r.origin() - center;
    double a = dot(r.direction(), r.direction());
    double b = dot(oc, r.direction());
    double c = dot(oc, oc) - radius*radius;
    double discriminant = b*b - a*c;
    if(discriminant > 0)
    {
        double temp = (-b - sqrt(discriminant))/a;
        if(temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center)/radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-b + sqrt(discriminant))/a;
        if(temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center)/radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

bool Sphere::bounding_box(double t0, double t1, aabb& box) const
{
    box = aabb(center - Vec3(radius, radius, radius),
               center + Vec3(radius, radius, radius));

    return true;
}


#endif