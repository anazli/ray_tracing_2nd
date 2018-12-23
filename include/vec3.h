#ifndef VECTOR_H
#define VECTOR_H

#include<iostream>
#include<random>

/*! \brief A simple 3D vector class.
 *
 *  The Vec3 class creates a 3D vector (x,y,z) and provides some useful operations
 *  such as dot, cross product, normalization, random vectors. It also supports all
 *  the operations such as multiplication, addition, subtraction, division etc.
 *
 */

class Vec3 {

public:


    //! Constructor with default arguments. If no parameters
    //! are provided, a vector (0,0,0) is created. Otherwise
    //! a vector (x,y,z) is created.
    Vec3(double p1 = 0., double p2 = 0., double p3 = 0.):
    m_x{p1},
    m_y{p2},
    m_z{p3}
    {   }

    //! Copy Constructor. Creates a vector that is a copy
    //! of an existing vector provided as argument.
    Vec3(const Vec3 &v):
    m_x{v.x()},
    m_y{v.y()},
    m_z{v.z()}
    {   }


    // Getters
    double x()const {return m_x;} //!< returns the x component.
    double y()const {return m_y;} //!< returns the y component.
    double z()const {return m_z;} //!< returns the z component.

    // Setters
    void setX(const double& p){m_x = p;} //!< sets the x component.
    void setY(const double& p){m_y = p;} //!< sets the y component.
    void setZ(const double& p){m_z = p;} //!< sets the z component.


    void setXYZ(const double& num)
    {setX(num);setY(num);setZ(num);} //!< sets the three components to a specific value.

    void setXYZ(const double& p1, const double& p2, const double& p3)
    {setX(p1);setY(p2);setZ(p3);} //!< Every component gets a unique value.


    Vec3  operator+() const{return Vec3(m_x, m_y, m_z);}; //!< Overloaded unary (+) operator. Returns the same vector.
    Vec3  operator-() const{return Vec3(-m_x, -m_y, -m_z);} //!< Overloaded unary (-) operator. Returns the opposite vector.


    // Prefix
    Vec3& operator++(); //!< Overloaded prefix (++) operator. Returns the vector and increments its components by one.
    Vec3& operator--(); //!< Overloaded prefix (--) operator. Returns the vector and decrements its components by one.
    // Postfix
    Vec3 operator++(int); //!< Overloaded postfix (++) operator. Increments the vectors components by one and returns it.
    Vec3 operator--(int); //!< Overloaded postfix (--) operator. Decrements the vectors components by one and returns it.


    Vec3& operator+=(const double& num); //!< Overloaded unary (+=) operator. Increments the vector's components by a value num.
    Vec3& operator-=(const double& num); //!< Overloaded unary (-=) operator. Decrements the vector's components by a value num.
    Vec3& operator*=(const double& num); //!< Overloaded unary (*=) operator. Multiplies the vector's components by a value num.


    Vec3& normalize(); //!< Makes the vector a unit vector of length one.
    double magnitude()const; //!< Returns the length of the vector.


private:

    double m_x;
    double m_y;
    double m_z;

};

Vec3 operator+(const Vec3&, const Vec3&); //!< Overloaded binary (+) operator. Addition of two vectors.
Vec3 operator+(const Vec3&, const double&); //!< Overloaded binary (+) operator. Addition of a vector and a number.
Vec3 operator+(const double&, const Vec3&); //!< Overloaded binary (+) operator. Addition of a vector and a number.

Vec3 operator-(const Vec3&, const Vec3&); //!< Overloaded binary (-) operator. Subtraction of two vectors.
Vec3 operator-(const Vec3&, const double&); //!< Overloaded binary (-) operator. Subtraction of a vector and a number.
Vec3 operator-(const double&, const Vec3&);  //!< Overloaded binary (-) operator. Subtraction of a vector and a number.

Vec3 operator*(const Vec3&, const Vec3&); //!< Overloaded binary (*) operator. Multiplication of two vectors.
Vec3 operator*(const Vec3&, const double&); //!< Overloaded binary (*) operator. Multiplication of a vector and a number.
Vec3 operator*(const double&, const Vec3&); //!< Overloaded binary (*) operator. Multiplication of a vector and a number.

Vec3 operator/(const Vec3&, const Vec3&); //!< Overloaded binary (/) operator. Division of two vectors component by component.
Vec3 operator/(const Vec3&, const double&); //!< Overloaded binary (/) operator. Division of a vector's components by a number.

bool operator==(const Vec3&, const Vec3&); //!< Overloaded binary (==) operator. Checks if two vectors are identical.
bool operator!=(const Vec3&, const Vec3&); //!< Overloaded binary (!=) operator. Checks if two vectors are not identical.

std::istream& operator>>(std::istream& in, Vec3& v); //!< Overloaded binary (>>) operator. Reads a vector's components from a stream.
std::ostream& operator<<(std::ostream&, const Vec3&); //!< Overloaded binary (<<) operator. Prints a vector's components to a stream.

double dot(const Vec3&, const Vec3&); //!< The dot product of two vectors. Returns a number of type double.
Vec3 cross(const Vec3& v1, const Vec3& v2); //!< The cross product of two vectors. Returns a vector perpendicular to the surface of them.

Vec3 getUnitVectorOf(const Vec3&); //!< Gets the unit vector of a vector without changing the provided vector.
Vec3 randomVector(const double& a = 0., const double& b = 1.); //!< Returns a vector with uniformly random components in the interval [a,b].
Vec3 randomVectorOnUnitSphere(); //!< Returns a random vector on a unit sphere.


#endif // VECTOR_H
