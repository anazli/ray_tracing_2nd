#include "../include/vec3.h"
#include<cmath>

using std::cout; using std::endl;
using std::ostream; using std::istream;
using std::uniform_real_distribution;
std::mt19937 gen(13759327u);

Vec3& Vec3::operator++()
{
    *this += 1; //use of += member operator
    return *this;
}

Vec3& Vec3::operator--()
{
    *this -= 1;
    return *this;
}

Vec3 Vec3::operator++(int)
{
    Vec3 temp(m_x, m_y, m_z);
    ++(*this); //use of prefix ++ operator
    return temp;
}

Vec3 Vec3::operator--(int)
{
    Vec3 temp(m_x, m_y, m_z);
    --(*this);
    return temp;
}

Vec3& Vec3::operator+=(const double& num)
{
    *this = (*this) + num;  //use of binary + operator
    return *this;
}

Vec3& Vec3::operator-=(const double& num)
{
    *this = (*this) - num;
    return *this;
}

Vec3& Vec3::operator*=(const double& num)
{
    *this = (*this) * num;
    return *this;
}

Vec3& Vec3::normalize()
{
    double norm = this->magnitude();
    if(norm == 0.)
        throw "Division by zero!";
    *this = (*this)/norm;
    return *this;
}

//--------------------------------------------
// Overloaded I/O operators (input, output)
//--------------------------------------------

istream& operator>>(istream& in, Vec3& v)
{
    double x,y,z;
    in >> x >> y >> z;
    v.setXYZ(x,y,z);
}

ostream& operator<<(ostream& out, const Vec3& v)
{
    out << "(" << v.x() << "," << v.y() << "," << v.z() << ")";
}


//--------------------------------------------
// Overloaded operators as normal functions
// Binary operator (+, -, *)
//--------------------------------------------

Vec3 operator+(const Vec3& v1, const Vec3& v2)
{
    return Vec3(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z());
}

Vec3 operator+(const Vec3& v, const double& num)
{
    return Vec3(v.x() + num, v.y() + num, v.z() + num);
}

Vec3 operator+(const double& num, const Vec3& v)
{
    return v + num;
}

Vec3 operator-(const Vec3& v1, const Vec3& v2)
{
    return Vec3(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
}

Vec3 operator-(const Vec3& v, const double& num)
{
    return Vec3(v.x() - num, v.y() - num, v.z() - num);
}

Vec3 operator-(const double& num, const Vec3& v)
{
    return v - num;
}

Vec3 operator*(const Vec3& v1, const Vec3& v2)
{
    return Vec3(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z());
}

Vec3 operator*(const Vec3& v, const double& num)
{
    return Vec3(v.x() * num, v.y() * num, v.z() * num);
}

Vec3 operator*(const double& num, const Vec3& v)
{
    return v * num;
}

Vec3 operator/(const Vec3& v1, const Vec3& v2)
{
    if(v2.x() == 0. || v2.y() == 0. || v2.z() == 0.)
        throw "Division by zero!";
    return Vec3(v1.x()/v2.x(), v1.y()/v2.y(), v1.z()/v2.z());
}

Vec3 operator/(const Vec3& v, const double& num)
{
    if(num == 0.)
        throw "Division by zero!";
    return Vec3(v.x()/num, v.y()/num, v.z()/num);
}


// Comparison operators

bool operator==(const Vec3& v1, const Vec3& v2)
{
    return (v1.x() == v2.x() && v1.y() == v2.y() &&
            v1.z() == v2.z());
}

bool operator!=(const Vec3& v1, const Vec3& v2)
{
    return !(v1 == v2);
}


//--------------------------------------------
// General methods (dot product, magnitude)
//--------------------------------------------

double dot(const Vec3& v1, const Vec3& v2)
{
    Vec3 v = v1 * v2;
    return v.x() + v.y() + v.z();
}

Vec3 cross(const Vec3& v1, const Vec3& v2)
{
    double x = v1.y()*v2.z() - v1.z()*v2.y();
    double y = v1.z()*v2.x() - v1.x()*v2.z();
    double z = v1.x()*v2.y() - v1.y()*v2.x();
    return Vec3(x,y,z);
}

double Vec3::magnitude()const
{
    return sqrt( x() * x() + y() * y() + z() * z() );
}

Vec3 getUnitVectorOf(const Vec3& v)
{
    if(v.magnitude() == 0.)
        throw "Division by zero!";
    return v/v.magnitude();
}

Vec3 randomVector(const double& a, const double& b)
{
    uniform_real_distribution<double> rand(a, b);
    return Vec3(rand(gen), rand(gen), rand(gen));
}

Vec3 randomVectorOnUnitSphere()
{
    uniform_real_distribution<double> rand(0., 1.);
    double xi1, xi2, dsq = 2;
	while(dsq >= 1.)
    {
		xi1 = 1. - 2. * rand(gen);
		xi2 = 1. - 2. * rand(gen);
		dsq = xi1 * xi1 + xi2 * xi2;
	}

	double ranh = 2. * sqrt(1. - dsq);
	double dmx = xi1 * ranh;
	double dmy = xi2 * ranh;
	double dmz = 1. - 2. * dsq;

	Vec3 ret(dmx,dmy,dmz);
	return ret;
}


