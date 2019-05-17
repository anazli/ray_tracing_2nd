#ifndef CONE_H
#define CONE_H

#include "hitable.h"

class Cone : public Hitable {

    public:

        Cone(){}
        Cone(Vec3 cen, double h, double r, Material *m) : 
        center(cen), height(h), radius(r), mat_ptr(m) {}
        
        virtual bool hit(const Ray& r, double t_min, double t_max,
                                                     hit_record& rec) const;

        Vec3 center;
        double height;
        double radius;
        Material *mat_ptr;
};


bool Cone::hit(const Ray& r, double t_min, double t_max,
                                             hit_record& rec)const
{
    Vec3 oc = r.origin() - center;
    Vec3 axis(0., 1., 0.);
    double ratio = radius/height;
    double ratio2 = ratio * ratio;
    double ymin = center.y();
    double ymax = ymin + height;
    double a = dot(r.direction().x(), r.direction().x())
             + dot(r.direction().z(), r.direction().z())
             - dot(r.direction().y(), r.direction().y())*ratio2;
    double b = 2. * dot(oc.x(), r.direction().x())
             + 2. * dot(oc.z(), r.direction().z())
             - 2. * ratio2 * (dot(oc.y(), r.direction().y()) - r.direction().y()*height);
    double c = dot(oc.x(), oc.x())
             + dot(oc.z(), oc.z())
             - (dot(oc.y(), oc.y()) - 2. * oc.y() * height + height*height)*ratio2;

    
    double discriminant = b*b - 4.*a*c;
    if(discriminant >= 0)
    {
        double temp = (-b - sqrt(discriminant))/(2.*a);
        if(temp < t_max && temp > t_min)
        {
            Vec3 temp_vec = r.point_at_parameter(temp);
            if(temp_vec.y() > ymin && temp_vec.y() < ymax)
            {
                rec.t = temp;
                rec.p = r.point_at_parameter(rec.t);
                Vec3 c = center;c.setY(rec.p.y());
                Vec3 v = getUnitVectorOf(rec.p-c);
                v = v/ratio;
                v.setY(ratio);
                rec.normal = v;
                rec.mat_ptr = mat_ptr;
                return true;
            }
        }
        temp = (-b + sqrt(discriminant))/(2.*a);
        if(temp < t_max && temp > t_min)
        {
            Vec3 temp_vec = r.point_at_parameter(temp);
            if(temp_vec.y() > ymin && temp_vec.y() < ymax)
            {
                rec.t = temp;
                rec.p = r.point_at_parameter(rec.t);
                Vec3 c = center;c.setY(rec.p.y());
                Vec3 v = getUnitVectorOf(rec.p-c);
                v = v/ratio;
                v.setY(ratio);
                rec.normal = v;
                rec.mat_ptr = mat_ptr;
                return true;
            }
        }
    }
    return false;
}

#endif