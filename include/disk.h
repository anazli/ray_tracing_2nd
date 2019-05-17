#ifndef DISK_H
#define DISK_H

#include "hitable.h"

class Disk : public Hitable {

    public:

        Disk(){}
        Disk(Vec3 ce, double the, double ph, double r, Material *m) :
         center(ce), theta(the*M_PI/180.), phi(ph*M_PI/180.), radius(r), mat_ptr(m) {}
        
        virtual bool hit(const Ray& r, double t_min, double t_max,
                                                     hit_record& rec) const;

        Vec3 center;
        double theta;
        double phi;
        double radius;
        Material *mat_ptr;
};


bool Disk::hit(const Ray& r, double t_min, double t_max,
                                             hit_record& rec)const
{
    double x = cos(phi)*cos(theta);
    double y = cos(phi)*sin(theta);
    double z = sin(phi);
    Vec3 normal1(x,y,z);
    Vec3 normal2 = -normal1;

    Vec3 normal;
    if(dot(r.direction(), normal1) < 0)
        normal = normal1;
    else if(dot(r.direction(), normal2) < 0)
        normal = normal2;

    double intersection = dot(normal, center - r.origin())/dot(normal, r.direction());
    
    if(intersection >= 0)
    {
        if(intersection < t_max && intersection > t_min)
        {
            Vec3 point = r.point_at_parameter(intersection);
            if(dot(point - center, point - center) <= radius*radius)
            {
                rec.t = intersection;
                rec.p = r.point_at_parameter(rec.t);
                rec.normal = normal;
                rec.mat_ptr = mat_ptr;
                return true;
            }
        }
    }
    return false;
}

#endif