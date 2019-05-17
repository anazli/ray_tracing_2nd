#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {

    public:

        Ray(){}
        Ray(const Vec3& a, const Vec3& b, double ti = 0.) : A(a), B(b), _time(ti) { }
        Vec3 origin()   const {return A;}
        Vec3 direction()const {return B;}
        double time() const {return _time;} 
        Vec3 point_at_parameter(const double& t)const{return A + t*B;}

    private:

        Vec3 A;
        Vec3 B;
        double _time;
};

#endif