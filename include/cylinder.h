#ifndef CYLINDER_H
#define CYLINDER_H

#include "hitable.h"
#include<iostream>
#include<cmath>

class Cylinder : public Hitable {

    public:

        Cylinder(){}
        Cylinder(Vec3 cen, double h, double r, Material *m) :
         center(cen), height(h), radius(r), mat_ptr(m) {}
        
        virtual bool hit(const Ray& r, double t_min, double t_max,
                                                     hit_record& rec) const;
        virtual bool bounding_box(double t0, double t1, aabb& box) const
        {return true;}

        Vec3 center;
        double height;
        double radius;
        Material *mat_ptr;
};


bool Cylinder::hit(const Ray& r, double t_min, double t_max,
                                             hit_record& rec)const
{
    Vec3 oc = r.origin() - center;
    Vec3 orientation(1.,0.,1.);
    double ymin = center.y();
    double ymax = ymin + height;
    double a = dot(r.direction() * orientation, r.direction());
    double b = 2. * dot(oc * orientation, r.direction());
    double c = dot(oc * orientation, oc) - radius*radius;
    double discriminant = b*b - 4.*a*c;
    if(discriminant >= 0)
    {
        double temp = (-b - sqrt(discriminant))/(2.*a);
        if(temp < t_max && temp > t_min)
        {
            double l;
            Vec3 temp_vec = r.point_at_parameter(temp);
            l = temp_vec.y();
            if(l > ymin && l < ymax)
            {
                rec.t = temp;
                rec.p = temp_vec;
                Vec3 normal;
                Vec3 c = center;c.setY(rec.p.y());
                Vec3 n1 = (rec.p - c)/radius;
                Vec3 n2(-n1.x(), n1.y(), -n1.z());

                if(dot(r.direction(), n1) < 0)
                    normal = n1;
                else if(dot(r.direction(), n2) < 0)
                    normal = n2;

                rec.normal = normal;
                rec.mat_ptr = mat_ptr;
                return true;
            }
        }
        temp = (-b + sqrt(discriminant))/(2.*a);
        if(temp < t_max && temp > t_min)
        {
            double l;
            Vec3 temp_vec = r.point_at_parameter(temp);
            l = temp_vec.y();
            if(l > ymin && l < ymax)
            {
                rec.t = temp;
                rec.p = temp_vec;
                Vec3 normal;
                Vec3 c = center;c.setY(rec.p.y());
                Vec3 n1 = (rec.p - c)/radius;
                Vec3 n2(-n1.x(), n1.y(), -n1.z());
                if(dot(r.direction(), n1) < 0)
                    normal = n1;
                else if(dot(r.direction(), n2) < 0)
                    normal = n2;

                rec.normal = normal;
                rec.mat_ptr = mat_ptr;
                return true;
            }
        }
    }
    return false;
}

#endif