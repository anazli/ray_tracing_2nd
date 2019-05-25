#include<iostream>
#include<fstream>
#include "../include/camera.h"
#include "../include/ray.h"
#include "../include/material.h"
#include "../include/sphere.h"
#include "../include/moving_sphere.h"
#include "../include/cylinder.h"
#include "../include/plane.h"
#include "../include/disk.h"
#include "../include/cone.h"
#include "../include/hitable.h"
#include "../include/hitable_list.h"
#include "../include/texture.h"
#include "../include/rectangle.h"
#include "../include/box.h"
#include "../include/translate.h"
#include "../include/constant_medium.h"
#include "../include/bvh_node.h"
#include "../include/surface_texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#include "../include/stb_image_write.h"

using namespace std;

Hitable *scene2();
Hitable *scene1();
Hitable *scene();
Hitable *final();
Hitable *cornell();
Hitable *cornell_box();
Hitable *simple_light();
Hitable *spheres();
Hitable *two_spheres_perlin();
Hitable *random_scene();

Vec3 color(const Ray& r, Hitable *world, int depth)
{
    hit_record rec;
    if(world->hit(r, 0.001, MAXFLOAT, rec))
    {
        Ray scattered;
        Vec3 attenuation;
        Vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
        if(depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            return emitted + attenuation * color(scattered, world, depth+1);
        }
        else
        {
            return emitted;
        }
    }
    else
        return Vec3(0.08, 0.08, 0.17);
    //Vec3 unit_direction = getUnitVectorOf(r.direction());
    //double t = 0.5 * (unit_direction.y() + 1.);
    //return (1. - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.2, 0.7, 1.0);
}

int main()
{
    int nx = 800;
    int ny = 600;
    int ns = 20;
    ofstream out;
    out.open("image.ppm");

    Hitable *world = scene2();

    //Vec3 lookfrom(278., 278., -800.);
    //Vec3 lookat(278., 278., 0.);
    Vec3 lookfrom(0,2.,9);
    Vec3 lookat(0,0,0);
    double dist_to_focus = 10.;
    double aperture = 0.0;
    double vfov = 70.;

    Camera cam(lookfrom, lookat, Vec3(0., 1., 0.), vfov, double(nx)/double(ny),
                                                     aperture, dist_to_focus, 0., 1.);

    out << "P3\n" << nx << " " << ny << "\n255\n";
    for(int j = ny ; j >= 0 ; --j)
    {
        for(int i = 0 ; i < nx ; ++i)
        {
            Vec3 col(0.,0.,0.);
            for(int s = 0 ; s < ns ; ++s)
            {
                double u = double(i + drand48())/double(nx);
                double v = double(j + drand48())/double(ny);

                Ray r = cam.get_ray(u,v);
                col = col + color(r, world, 0);
            }
            col = col/ double(ns);
            col = Vec3(sqrt(col.x()), sqrt(col.y()), sqrt(col.z()));
            int ir = int(255.99*col.x());
            int ig = int(255.99*col.y());
            int ib = int(255.99*col.z());

            out << ir << " " << ig << " " << ib << endl;
        }
    }

    out.close();

    return 0;
}

Hitable *scene2()
{
    int Nobj = 20;
    int i = 0;
    Hitable **list = new Hitable*[Nobj];
    Material *light = new Diffuse_light(new Constant_texture(Vec3(0.7, 0.1, 0.1)));
    Texture *pertext = new Noise_texture(2.);

    list[i++] = new Plane(Vec3(0.,0.,0.), Vec3(0., 1., 0.) , new Lambertian(new Constant_texture(Vec3(0.4,0.4,0.4))));
    list[i++] = new Sphere(Vec3(0., 6., -7.), 6., new Lambertian(pertext));

    Vec3 center1(2., 0., 0.);
    Vec3 center2(0., 0., 0.);
    Vec3 center3(-2., 0., 0.);
    for(i = 1 ; i < 18 ; i += 6)
    {
        double r = 0.2;
        double height = 2.;
        
        list[i+1] = new Cylinder(center1, height, 0.1, new Dielectric(1.5));
        list[i+2] = new Sphere(Vec3(center1[0], height+r, center1[2]), r, light);
        list[i+3] = new Cylinder(center2, 2., 0.1, new Dielectric(1.5));
        list[i+4] = new Sphere(Vec3(center2[0], height+r, center2[2]), r, light);
        list[i+5] = new Cylinder(center3, 2., 0.1, new Dielectric(1.5));
        list[i+6] = new Sphere(Vec3(center3[0], height+r, center3[2]), r, light);
        center1[2] += 3;
        center2[2] += 3;
        center3[2] += 3;
    }

    return new Hitable_list(list, Nobj);
}
Hitable *scene1()
{
    int Nobj = 8;
    Hitable **list = new Hitable*[Nobj];
    int i = 0;
    Material *ground = new Lambertian( new Constant_texture(Vec3(0.48, 0.83, 0.53)) );
    Material *light = new Diffuse_light(new Constant_texture(Vec3(0.5, 0.5, 0.5)));
    list[i++] = new xz_rect(-2, 2, -2, 2, 0, light);
    list[i++] = new Plane(Vec3(0.,0.,0.), Vec3(0., 1., 0.) , ground);

}

Hitable *scene()
{
    int Nobj = 7;
    Hitable **list = new Hitable*[Nobj];
    int i = 0;
    Texture *pertext = new Noise_texture(2.);
    Material *red = new Lambertian(new Constant_texture(Vec3(0.65, 0.05, 0.05)));
    Material *white = new Lambertian(new Constant_texture(Vec3(0.73, 0.73, 0.73)));
    Material *light = new Diffuse_light(new Constant_texture(Vec3(0.5, 0.5, 0.5)));
    list[i++] = new Plane(Vec3(0.,0.,0.), Vec3(0., 1., 0.) , new Lambertian(new Constant_texture(Vec3(0.4,0.4,0.4))));
    list[i++] = new Sphere(Vec3(0.6, 1.9, 0.6), 0.7, new Dielectric(1.5));
    Hitable *b1 = new Translate(new Rotate_y(new Box(Vec3(0, 0, 0), Vec3(1.2, 1.2, 1.2), light), 10), Vec3(0, 0, 0));
    list[i++] = b1;
    list[i++] = new Cone(Vec3(-2.2, 0., -1.), 4.0, 1.5, new Metal(Vec3(0.3,0.3,0.3), 0.01));
    //list[i++] = new Constant_medium(b1, 0.01, new Constant_texture(Vec3(1.,1.,0.)));
    light = new Diffuse_light(new Constant_texture(Vec3(0.1, 0.1, 2.7)));
    list[i++] = new Sphere(Vec3(-1.3, 0.3, 3.), 0.3, light);
    list[i++] = new Sphere(Vec3(6., 5., -6.), 5., new Lambertian(pertext));
    light = new Diffuse_light(new Constant_texture(Vec3(0.1, 2.7, 0.1)));
    list[i++] = new Sphere(Vec3(1., 0.3, 3.), 0.3, light);
    //Hitable *b2 = new Translate(new Rotate_y(new Box(Vec3(0., 0, 0.), Vec3(0.6, 0.6, 0.6), white), -18), Vec3(0.7, 0, 2.7));
    //list[i++] = new Constant_medium(b2, 0.8, new Constant_texture(Vec3(0.7,0.1,0.1)));
    return new Hitable_list(list, Nobj);
}


Hitable *final() {
    int nb = 20;
    Hitable **list = new Hitable*[30];
    Hitable **boxlist = new Hitable*[10000];
    Hitable **boxlist2 = new Hitable*[10000];
    Material *white = new Lambertian( new Constant_texture(Vec3(0.73, 0.73, 0.73)) );
    Material *ground = new Lambertian( new Constant_texture(Vec3(0.48, 0.83, 0.53)) );
    int b = 0;
    for (int i = 0; i < nb; i++) {
        for (int j = 0; j < nb; j++) {
            float w = 100;
            float x0 = -1000 + i*w;
            float z0 = -1000 + j*w;
            float y0 = 0;
            float x1 = x0 + w;
            float y1 = 100*(drand48()+0.01);
            float z1 = z0 + w;
            boxlist[b++] = new Box(Vec3(x0,y0,z0), Vec3(x1,y1,z1), ground);
        }
    }
    int l = 0;
    list[l++] = new bvh_node(boxlist, b, 0, 1);
    Material *light = new Diffuse_light( new Constant_texture(Vec3(7, 7, 7)) );
    list[l++] = new xz_rect(123, 423, 147, 412, 554, light);
    Vec3 center(400, 400, 200);
    list[l++] = new Moving_sphere(center, center+Vec3(30, 0, 0), 0, 1, 50, new Lambertian(new Constant_texture(Vec3(0.7, 0.3, 0.1))));
    list[l++] = new Sphere(Vec3(260, 150, 45), 50, new Dielectric(1.5));
    list[l++] = new Sphere(Vec3(0, 150, 145), 50, new Metal(Vec3(0.8, 0.8, 0.9), 10.0));
    Hitable *boundary = new Sphere(Vec3(360, 150, 145), 70, new Dielectric(1.5));
    list[l++] = boundary;
    list[l++] = new Constant_medium(boundary, 0.2, new Constant_texture(Vec3(0.2, 0.4, 0.9)));
    boundary = new Sphere(Vec3(0, 0, 0), 5000, new Dielectric(1.5));
    list[l++] = new Constant_medium(boundary, 0.0001, new Constant_texture(Vec3(1.0, 1.0, 1.0)));
    int nx, ny, nn;
    //unsigned char *tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
    //Material *emat =  new Lambertian(new image_texture(tex_data, nx, ny));
    list[l++] = new Sphere(Vec3(400,200, 400), 100, new Dielectric(1.5));
    Texture *pertext = new Noise_texture(0.1);
    list[l++] =  new Sphere(Vec3(220,280, 300), 80, new Lambertian( pertext ));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxlist2[j] = new Sphere(Vec3(165*drand48(), 165*drand48(), 165*drand48()), 10, white);
    }
    list[l++] =   new Translate(new Rotate_y(new bvh_node(boxlist2,ns, 0.0, 1.0), 15), Vec3(-100,270,395));
    return new Hitable_list(list,l);
}

Hitable *cornell()
{
    Hitable **list = new Hitable*[8];
    int i = 0;

    Material *red = new Lambertian(new Constant_texture(Vec3(0.65, 0.05, 0.05)));
    Material *white = new Lambertian(new Constant_texture(Vec3(0.73, 0.73, 0.73)));
    Material *green = new Lambertian(new Constant_texture(Vec3(0.12, 0.45, 0.15)));
    Material *light = new Diffuse_light(new Constant_texture(Vec3(7, 7, 7)));

    list[i++] = new Flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
    int nx, ny, nn;
    unsigned char *tex_data = stbi_load("images/julia.jpg", &nx, &ny, &nn, 0);
    Material *emat =  new Lambertian(new image_texture(tex_data, nx, ny));
    list[i++] = new yz_rect(0, 555, 0, 555, 0, emat);
    list[i++] = new xz_rect(113, 443, 127, 432, 554, light);
    list[i++] = new Flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
    list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
    list[i++] = new Flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
    Hitable *b1 = new Translate(new Rotate_y(new Box(Vec3(0, 0, 0), Vec3(165, 165, 165), white), -18), Vec3(130, 0, 65));
    Hitable *b2 = new Translate(new Rotate_y(new Box(Vec3(0, 0, 0), Vec3(165, 330, 165), white), 15), Vec3(265, 0, 295));
    list[i++] = new Sphere(Vec3(0,0.5,0), 0.5, emat);
    list[i++] = new Constant_medium(b2, 0.01, new Constant_texture(Vec3(0.,0.,0.)));

    return new Hitable_list(list, i);
}


Hitable *cornell_box()
{
    Hitable **list = new Hitable*[8];
    int i = 0;
    Material *red = new Lambertian(new Constant_texture(Vec3(0.65, 0.05, 0.05)));
    Material *white = new Lambertian(new Constant_texture(Vec3(0.73, 0.73, 0.73)));
    Material *green = new Lambertian(new Constant_texture(Vec3(0.12, 0.45, 0.15)));
    Material *light = new Diffuse_light(new Constant_texture(Vec3(7, 7, 7)));
    list[i++] = new Flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
    list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
    list[i++] = new xz_rect(113, 443, 127, 432, 554, light);
    list[i++] = new Flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
    list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
    list[i++] = new Flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
    Hitable *b1 = new Translate(new Rotate_y(new Box(Vec3(0, 0, 0), Vec3(165, 165, 165), white), -18), Vec3(130, 0, 65));
    Hitable *b2 = new Translate(new Rotate_y(new Box(Vec3(0, 0, 0), Vec3(165, 330, 165), white), 15), Vec3(265, 0, 295));
    list[i++] = new Constant_medium(b1, 0.01, new Constant_texture(Vec3(1.,1.,0.)));
    list[i++] = new Constant_medium(b2, 0.01, new Constant_texture(Vec3(0.,0.,0.)));

    return new Hitable_list(list, i);
}

Hitable *simple_light()
{
    Texture *pertext = new Noise_texture(4);
    Hitable **list = new Hitable*[4];
    list[0] = new Sphere(Vec3(0.,-1000.,0.), 1000, new Lambertian(pertext));
    list[1] = new Sphere(Vec3(0., 2., 0.), 2., new Lambertian(pertext));
    list[2] = new Sphere(Vec3(0., 7., 0.), 2., new Diffuse_light(new Constant_texture(Vec3(4,4,4))));
    list[3] = new xy_rect(3, 5, 1, 3., -2, new Diffuse_light(new Constant_texture(Vec3(4,4,4))));
    return new Hitable_list(list, 4);

}

Hitable *spheres()
{
    int Nobj = 2;
    int i = 0;
    int nx, ny, nn;
    Hitable **list = new Hitable*[Nobj];
    Texture *pertext = new Noise_texture(4);
    unsigned char *tex_data = stbi_load("julia.jpg", &nx, &ny, &nn, 0);
    //Material *emat =  new Lambertian(new image_texture(tex_data, nx, ny));
    list[i++] = new Sphere(Vec3(0.,-1000.,0.), 1000, new Lambertian(pertext));
    list[i++] = new Cylinder(Vec3(-1, 0.5, 1), 1.0, 0.5, new Diffuse_light(new Constant_texture(Vec3(5,5,5))));
    //Hitable *b = new Sphere(Vec3(3,1,0), 1, new Lambertian(new Constant_texture(Vec3(1,1,1))));
    //list[i++] = new Constant_medium(b, 0.06, new Constant_texture(Vec3(1,1,1)));
    return new Hitable_list(list, i);

}

Hitable *two_spheres_perlin()
{
    int Nobj = 2;
    Hitable **list = new Hitable*[Nobj];
    Texture *pertext = new Noise_texture(4);
    list[0] = new Sphere(Vec3(0.,-1000.,0.), 1000, new Lambertian(pertext));
    list[1] = new Sphere(Vec3(0., 2.0, 0.), 2.0, new Lambertian(pertext));
    //Hitable *world = new Hitable_list(list, Nobj);
    return new Hitable_list(list, Nobj);

}

Hitable *random_scene()
{
    int n = 500;
    Hitable **list = new Hitable*[n+1];
    list[0] = new Sphere(Vec3(0., -1000., 0.), 1000, new Lambertian(new Constant_texture(Vec3(0.5, 0.5, 0.5))));
    int i = 1;
    for(int a = -11 ; a < 11 ; a++)
    {
        for(int b = -11 ; b < 11 ; b++)
        {
            double choose_mat = drand48();
            Vec3 center(a + 0.9*drand48(), 0.2, b + 0.9*drand48());
            if((center - Vec3(4., 0.2, 0.)).length() > 0.9)
            {
                if(choose_mat < 0.8) //diffuse
                {
                    list[i++] = new Sphere(center, 0.2,
                         new Lambertian(new Constant_texture(Vec3(drand48()*drand48(),
                                     drand48()*drand48(), drand48()*drand48()))));
                }
                else if(choose_mat < 0.95) //metal
                {
                    list[i++] = new Sphere(center, 0.2,
                         new Metal(Vec3(0.5 * (1. + drand48()),
                                     0.5 * (1. + drand48()), 0.5 * (1. + drand48())), 0.5*drand48()));
                    
                }
                else
                {
                    list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new Sphere(Vec3(0., 1., 0.), 1., new Dielectric(1.5));
    list[i++] = new Sphere(Vec3(-4., 1., 0.), 1., new Lambertian(new Constant_texture(Vec3(0.4, 0.2, 0.1))));
    list[i++] = new Sphere(Vec3(4., 1., 0.), 1., new Metal(Vec3(0.7, 0.6, 0.5), 0.));

    return new Hitable_list(list, i);
}
