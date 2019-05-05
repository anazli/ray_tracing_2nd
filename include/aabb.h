#ifndef AABB_H
#define AABB_H

#include "ray.h"
#include<vector>

inline double ffmin(double a, double b) {return a < b ? a : b;}
inline double ffmax(double a, double b) {return a > b ? a : b;}

class aabb {
    public:
        aabb(){}
        aabb(const Vec3& a, const Vec3& b): _min(a), _max(b){}

        Vec3 min() const {return _min;}
        Vec3 max() const {return _max;}

        bool hit(const Ray& r, double tmin, double tmax) const 
        {
            //There's one problem! I don't use vector or array to implement
            // the Vec3 class. So I can't iterate otherwise over a vec's components
            std::vector<double> vorigin{r.origin.x(),r.origin.y(),r.origin.z()};
            std::vector<double> vdirect{r.direction.x(),r.direction.y(),r.direction.z()};
            std::vector<double> vmin{_min.x(), _min.y(), _min.z()};
            std::vector<double> vmax{_max.x(), _max.y(), _max.z()};
            for(int i = 0 ; i < 3 ; ++i)
            {
                double t0 = ffmin( (vmin[i] - vorigin[i])/vdirect[i],
                                   (vmax[i] - vorigin[i])/vdirect[i]);
                double t1 = ffmax( (vmin[i] - vorigin[i])/vdirect[i],
                                   (vmax[i] - vorigin[i])/vdirect[i]);        
                
                tmin = ffmax(t0, tmin);
                tmax = ffmin(t1, tmax);

                if(tmax <= tmin)
                    return false;
            }

            return true;
        }

        Vec3 _min;
        Vec3 _max;
};


aabb surrounding_box(aabb box0, aabb box1) 
{
    Vec3 small( fmin(box0.min().x(), box1.min().x()),
                fmin(box0.min().y(), box1.min().y()),
                fmin(box0.min().z(), box1.min().z()));

    Vec3 big  ( fmin(box0.max().x(), box1.max().x()),
                fmin(box0.max().y(), box1.max().y()),
                fmin(box0.max().z(), box1.max().z()));

    return aabb(small, big);
}

#endif