#ifndef BOX_H
#define BOX_H

#include "hitable.h"
#include "rectangle.h"
#include "hitable_list.h"

class Box : public Hitable {
    public:
        Box(){}
        Box(const Vec3& p0, const Vec3& p1, Material *ptr);
        virtual bool hit(const Ray& r, double t_min, double t_max,
                                                     hit_record& rec) const;
        virtual bool bounding_box(double t0, double t1, aabb& box) const
        {
            box = aabb(pmin, pmax);
            return true;
        }
    private:
        Vec3 pmin, pmax;
        Hitable *list_ptr;
};

Box::Box(const Vec3& p0, const Vec3& p1, Material *ptr)
{
    pmin = p0;
    pmax = p1;
    Hitable **list = new Hitable*[6];
    list[0] = new xy_rect(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr);
    list[1] = new Flip_normals(new xy_rect(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));
    list[2] = new xz_rect(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr);
    list[3] = new Flip_normals(new xz_rect(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));
    list[4] = new yz_rect(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr);
    list[5] = new Flip_normals(new yz_rect(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
    list_ptr = new Hitable_list(list, 6);
}

bool Box::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const
{
    return list_ptr->hit(r, t_min, t_max, rec);
}

#endif