#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include "hitable.h"
#include "material.h"
#include "texture.h"

class Constant_medium : public Hitable {
    public:
        Constant_medium(Hitable *b, double d, Texture *a): boundary(b),
                        density(d)
        {
            phase_function = new Isotropic(a);
        }
        virtual bool hit(const Ray& r, double t_min, double t_max,
                                                     hit_record& rec) const;
        virtual bool bounding_box(double t0, double t1, aabb& box) const
        {
            return boundary->bounding_box(t0, t1, box);
        }
    private:
        Hitable *boundary;
        double density;
        Material *phase_function;
};

bool Constant_medium::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const
{
    bool db = (drand48() < 0.00001);
    db = false;
    hit_record rec1, rec2;
    if(boundary->hit(r, -__FLT_MAX__, __FLT_MAX__, rec1))
    {
        if(boundary->hit(r, rec1.t + 0.0001, __FLT_MAX__, rec2))
        {
            if(db)std::cerr << "\nt0 t1 " << rec1.t << " " << rec2.t << "\n";
            if(rec1.t < t_min)
                rec1.t = t_min;
            if(rec2.t > t_max)
                rec2.t = t_max;
            if(rec1.t >= rec2.t)
                return false;
            if(rec1.t < 0)
                rec1.t = 0;
            
            double distance_inside_boundary = (rec2.t - rec1.t) * r.direction().length();
            double hit_distance = -(1./density)*log(drand48());
            if(hit_distance < distance_inside_boundary)
            {
                if(db)std::cerr << "hit_distance = " << hit_distance << "\n";
                rec.t = rec1.t + hit_distance / r.direction().length();
                if(db)std::cerr << "rec.t = " << rec.t << "\n";
                rec.p = r.point_at_parameter(rec.t);
                if(db)std::cerr << "rec.p = " << rec.p << "\n";
                rec.normal = Vec3(1,0,0);
                rec.mat_ptr = phase_function;
                return true;

            }
        }
    }
    return false;
}
#endif