#ifndef BVH_NODE_H
#define BVH_NODE_H

#include "hitable.h"

class bvh_node : public Hitable {
    public:
        bvh_node(){}
        bvh_node(Hitable **l, int n, double time0, double time1);
        virtual bool hit(const Ray& r, double t_min, double t_max,
                                                     hit_record& rec) const;
        virtual bool bounding_box(double t0, double t1, aabb& box) const;

        Hitable *left;
        Hitable *right;
        aabb m_box;

};

int box_x_compare (const void * a, const void * b) {
        aabb box_left, box_right;
        Hitable *ah = *(Hitable**)a;
        Hitable *bh = *(Hitable**)b;
        if(!ah->bounding_box(0,0, box_left) || !bh->bounding_box(0,0, box_right))
                        std::cerr << "no bounding box in bvh_node constructor\n";
        if ( box_left.min().x() - box_right.min().x() < 0.0  )
            return -1;
        else
            return 1;
}

int box_y_compare (const void * a, const void * b)
{
        aabb box_left, box_right;
        Hitable *ah = *(Hitable**)a;
        Hitable *bh = *(Hitable**)b;
        if(!ah->bounding_box(0,0, box_left) || !bh->bounding_box(0,0, box_right))
                        std::cerr << "no bounding box in bvh_node constructor\n";
        if ( box_left.min().y() - box_right.min().y() < 0.0  )
            return -1;
        else
            return 1;
}
int box_z_compare (const void * a, const void * b)
{
        aabb box_left, box_right;
        Hitable *ah = *(Hitable**)a;
        Hitable *bh = *(Hitable**)b;
        if(!ah->bounding_box(0,0, box_left) || !bh->bounding_box(0,0, box_right))
                        std::cerr << "no bounding box in bvh_node constructor\n";
        if ( box_left.min().z() - box_right.min().z() < 0.0  )
            return -1;
        else
            return 1;
}

bvh_node::bvh_node(Hitable **l, int n, double time0, double time1)
{
    int axis = int(3 * drand48());
    if(axis == 0)
        qsort(l, n, sizeof(Hitable *), box_x_compare);
    else if(axis == 1)
        qsort(l, n, sizeof(Hitable *), box_y_compare);
    else if(axis == 2)
        qsort(l, n, sizeof(Hitable *), box_z_compare);

    if(n == 1)
    {
        left = right = l[0];
    }
    else if(n == 2)
    {
        left = l[0];
        right = l[1];
    }
    else
    {
        left = new bvh_node(l, n/2, time0, time1);
        right = new bvh_node(l + n/2, n - n/2, time0, time1);
    }
    aabb box_left, box_right;
    if(!left->bounding_box(time0, time1, box_left) ||
       !right->bounding_box(time0, time1, box_right))
        std::cerr << "no bounding box in bvh constructor\n";

    m_box = surrounding_box(box_left, box_right);
}

bool bvh_node::bounding_box(double t0, double t1, aabb& box)const
{
    box = m_box;
    return true;
}

bool bvh_node::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const
{
    if(m_box.hit(r, t_min, t_max)) 
    {
        hit_record left_rec, right_rec;
        bool hit_left = left->hit(r, t_min, t_max, left_rec);
        bool hit_right = right->hit(r, t_min, t_max, right_rec);
        if(hit_left && hit_right)
        {
            if(left_rec.t < right_rec.t)
                rec = left_rec;
            else
                rec = right_rec;
            
            return true;
        }
        else if(hit_left)
        {
            rec = left_rec;
            return true;
        }
        else if(hit_right)
        {
            rec = right_rec;
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

#endif