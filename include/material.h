#ifndef MATERIAL_H
#define MATERIAL_H

struct hit_record;

#include "ray.h"
#include "hitable.h"

double schlick(double cosine, double ref_idx)
{
    double r0 = (1. - ref_idx)/(1. + ref_idx);
    r0 = r0 * r0;
    return r0 + (1. - r0) * pow( (1. - cosine), 5);
}

Vec3 reflect(const Vec3& v, const Vec3& n)
{
    return v - 2. * dot(v,n)*n;
}

bool refract(const Vec3& v, const Vec3& n, double ni_over_nt, Vec3& refracted)
{
    Vec3 uv = getUnitVectorOf(v);
    double dt = dot(uv, n);
    double discriminant = 1. - ni_over_nt * ni_over_nt * (1. - dt*dt);
    if(discriminant > 0)
    {
        refracted = ni_over_nt * (uv - n*dt) - n*sqrt(discriminant);
        return true;
    }
    else
        return false;
}

class Material {
    
    public:

        virtual bool scatter(const Ray& r_in, const hit_record& rec,
                             Vec3& attenuation, Ray& scattered) const = 0;

};

class Lambertian : public Material {

    public:

        Lambertian(const Vec3& a) : albedo(a){}
        virtual bool scatter(const Ray& r_in, const hit_record& rec,
                             Vec3& attenuation, Ray& scattered) const 
        {
            Vec3 target = rec.p + rec.normal + randomVectorOnUnitSphere();
            scattered = Ray(rec.p, target - rec.p);
            attenuation = albedo;
            return true;
        }

        Vec3 albedo;
};

class Metal : public Material {

    public:

        Metal(const Vec3& a, double f) : albedo(a)
        {
            if(f < 1.)
                fuzz = f;
            else
                fuzz = 1.;
        }
        virtual bool scatter(const Ray& r_in, const hit_record& rec,
                             Vec3& attenuation, Ray& scattered) const 
        {
            Vec3 reflected = reflect(getUnitVectorOf(r_in.direction()), rec.normal);
            scattered = Ray(rec.p, reflected + fuzz * randomVectorOnUnitSphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

        Vec3 albedo;
        double fuzz;
};

class Dielectric : public Material {

    public:

        Dielectric(double ri) : ref_idx(ri){}

        virtual bool scatter(const Ray& r_in, const hit_record& rec,
                             Vec3& attenuation, Ray& scattered) const 
        {
            Vec3 outward_normal;
            Vec3 reflected = reflect(r_in.direction(), rec.normal);
            double ni_over_nt;
            attenuation = Vec3(1., 1., 1.);
            Vec3 refracted;
            double reflect_prob;
            double cosine;
            if(dot(r_in.direction(), rec.normal) > 0)
            {
                outward_normal = -rec.normal;
                ni_over_nt = ref_idx;
                //cosine = ref_idx * dot(r_in.direction(), rec.normal)/
                                       // r_in.direction().magnitude();
                cosine = dot(r_in.direction(), rec.normal) / r_in.direction().magnitude();
                cosine = sqrt(1 - ref_idx*ref_idx*(1-cosine*cosine));
            }
            else
            {
                outward_normal = rec.normal;
                ni_over_nt = 1./ref_idx;
                cosine = -dot(r_in.direction(), rec.normal)/
                                        r_in.direction().magnitude();
            }

            if(refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
                reflect_prob = schlick(cosine, ref_idx);
            else
                reflect_prob = 1.;

            if(drand48() < reflect_prob)
                scattered = Ray(rec.p, reflected);
            else
                scattered = Ray(rec.p, refracted);

            return true;
        }

    double ref_idx;      
};

#endif