#ifndef TRANSLATE_H
#define TRANSLATE_H

#include "hitable.h"

class Translate : public Hitable {
    public:
        Translate(Hitable *p, const Vec3& displacement): ptr(p), offset(displacement){}
        virtual bool hit(const Ray& r, double t_min, double t_max,
                                                     hit_record& rec) const;
        virtual bool bounding_box(double t0, double t1, aabb& box) const;

    private:
        Hitable *ptr;
        Vec3 offset;
};

bool Translate::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const
{
    Ray moved_r(r.origin() - offset, r.direction(), r.time());
    if(ptr->hit(moved_r, t_min, t_max, rec))
    {
        rec.p = rec.p + offset;
        return true;
    }
    else
        return false;
}

bool Translate::bounding_box(double t0, double t1, aabb& box) const
{
    if(ptr->bounding_box(t0, t1, box))
    {
        box = aabb(box.min() + offset, box.max() + offset);
        return true;
    }
    else
        return false;
}

class Rotate_y : public Hitable {
    public:
        Rotate_y(Hitable *p, double angle);
        virtual bool hit(const Ray& r, double t_min, double t_max,
                                                     hit_record& rec) const;
        virtual bool bounding_box(double t0, double t1, aabb& box) const
        {
            box = bbox;
            return hasbox;
        }
    private:
        Hitable *ptr;
        double sin_theta;
        double cos_theta;
        bool hasbox;
        aabb bbox;
};

Rotate_y::Rotate_y(Hitable *p, double angle): ptr(p)
{
    double radians = (M_PI/180.) * angle;
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    hasbox = ptr->bounding_box(0, 1, bbox);
    Vec3 min(__FLT_MAX__, __FLT_MAX__, __FLT_MAX__);
    Vec3 max(-__FLT_MAX__, -__FLT_MAX__, -__FLT_MAX__);
    for(int i = 0 ; i < 2 ; ++i)
    {
        for(int j = 0 ; j < 2 ; ++j)
        {
            for(int k = 0 ; k < 2 ; ++k)
            {
                double x = i * bbox.max().x() + (1-i) * bbox.min().x();
                double y = j * bbox.max().y() + (1-j) * bbox.min().y();
                double z = k * bbox.max().z() + (1-k) * bbox.min().z();
                double newx = cos_theta * x + sin_theta * z;
                double newz = -sin_theta * x + cos_theta * z;
                Vec3 tester(newx, y, newz);
                if(tester.x() > max.x())
                    max.setX(tester.x());
                if(tester.x() < min.x())
                    min.setX(tester.x());

                if(tester.y() > max.y())
                    max.setY(tester.y());
                if(tester.y() < min.y())
                    min.setY(tester.y());

                if(tester.z() > max.z())
                    max.setZ(tester.z());
                if(tester.z() < min.z())
                    min.setZ(tester.z());
            }
        }
    }
    bbox = aabb(min, max);
}


bool Rotate_y::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const
{
    Vec3 origin = r.origin();
    Vec3 direction = r.direction();
    origin.setX(cos_theta*r.origin().x() - sin_theta*r.origin().z());
    origin.setZ(sin_theta*r.origin().x() + cos_theta*r.origin().z());
    direction.setX(cos_theta*r.direction().x() - sin_theta*r.direction().z());
    direction.setZ(sin_theta*r.direction().x() + cos_theta*r.direction().z());
    Ray rotated_r(origin, direction, r.time());
    if(ptr->hit(rotated_r, t_min, t_max, rec))
    {
        Vec3 p = rec.p;
        Vec3 normal = rec.normal;
        p.setX(cos_theta * rec.p.x() + sin_theta*rec.p.z());
        p.setZ(-sin_theta * rec.p.x() + cos_theta * rec.p.z());
        normal.setX(cos_theta * rec.normal.x() + sin_theta * rec.normal.z());
        normal.setZ(-sin_theta * rec.normal.x() + cos_theta * rec.normal.z());
        rec.p = p;
        rec.normal = normal;
        return true;
    }
    else
        return false;
}
                                                    

#endif