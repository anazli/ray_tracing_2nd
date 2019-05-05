#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

Vec3 random_in_unit_disk()
{
    Vec3 p;
    do {
        p = 2. * Vec3(drand48(), drand48(), 0) - Vec3(1., 1., 0.);
    }while(dot(p,p) >= 1.);

    return p;
}

class Camera {

    public:

        Camera(Vec3 lookfrom, Vec3 lookat, Vec3 vup, double vfov, double aspect,
                        double aperture, double focus_dist, double t0, double t1) { // vfov is top to bottom in degrees

            time0 = t0;
            time1 = t1;
            lens_radius = aperture/2.;
            double theta = vfov * M_PI /180.;
            double half_height = tan(theta/2.);
            double half_width = aspect * half_height;
            origin = lookfrom;
            w = getUnitVectorOf(lookfrom - lookat);
            u = getUnitVectorOf(cross(vup, w));
            v = cross(w,u);
            lower_left_corner = origin - half_width*focus_dist*u -
                                     half_height*focus_dist*v - focus_dist*w;
            horizontal = 2*half_width*focus_dist*u;
            vertical = 2*half_height*focus_dist*v;
        }

        Ray get_ray(double s, double t)
        {
            Vec3 rd = lens_radius * random_in_unit_disk();
            Vec3 offset = u * rd.x() + v * rd.y();
            double time = time0 + drand48() * (time1 - time0);
            return Ray(origin + offset, lower_left_corner + s*horizontal +
                                         t*vertical - origin - offset, time);
        }

        Vec3 origin;
        Vec3 lower_left_corner;
        Vec3 horizontal;
        Vec3 vertical;
        Vec3 u, v, w;
        double time0, time1;
        double lens_radius;
};

#endif