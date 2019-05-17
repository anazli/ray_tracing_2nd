#ifndef SURFTEXTH
#define SURFTEXTH

#include "texture.h"

class image_texture : public Texture {
    public:
        image_texture() {}
        image_texture(unsigned char *pixels, int A, int B) : data(pixels), nx(A), ny(B) {}
                virtual Vec3 value(double u, double v, const Vec3& p) const;
        unsigned char *data;
        int nx, ny;
};

Vec3 image_texture::value(double u, double v, const Vec3& p) const {
     int i = (  u)*nx;
     int j = (1-v)*ny-0.001;
     if (i < 0) i = 0;
     if (j < 0) j = 0;
     if (i > nx-1) i = nx-1;
     if (j > ny-1) j = ny-1;
     double r = int(data[3*i + 3*nx*j]  ) / 255.0;
     double g = int(data[3*i + 3*nx*j+1]) / 255.0;
     double b = int(data[3*i + 3*nx*j+2]) / 255.0;
     return Vec3(r, g, b);
}

#endif