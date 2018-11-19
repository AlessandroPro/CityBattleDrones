//
//  Vector3D.cpp
//  CityBuilder
//
//  Created by Alessandro Profenna on 2018-11-01.
//  Copyright © 2018 Alessandro Profenna. All rights reserved.
//

#include "Vector3D.hpp"

Vector3D::Vector3D():
    x(1.0),
    y(1.0),
    z(1.0){}

Vector3D::Vector3D(float x, float y, float z):
    x(x),
    y(y),
    z(z){}

void Vector3D::set(float newX, float newY, float newZ)
{
    x = newX; y = newY; z = newZ;
}

void Vector3D::loadZero()
{
    x = y = z = 0.0;
}

void Vector3D::loadOne()
{
   x = y = z = 1.0;
}

void Vector3D::normalize()
{
    const float norm = getLength();
    if (norm > 0)
    {
        x /= norm; y /= norm; z /= norm;
    }
}

float Vector3D::getLength()
{
    return (float)sqrt(x*x + y*y + z*z);
}

Vector3D Vector3D::crossProduct(const Vector3D& lhs, const Vector3D& rhs)
{
    return Vector3D(lhs.y*rhs.z - lhs.z*rhs.y, lhs.z*rhs.x - lhs.x*rhs.z, lhs.x*rhs.y - lhs.y*rhs.x);
}

float Vector3D::DotProduct(const Vector3D& lhs, const Vector3D& rhs)
{
    return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
}

Vector3D Vector3D::add(const Vector3D& v1, const Vector3D& v2)
{
    return Vector3D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector3D Vector3D::subtract(const Vector3D& v1, const Vector3D& v2)
{
    return Vector3D(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vector3D Vector3D::scalarMul(const Vector3D& v, float rhs)
{
    return Vector3D(v.x * rhs, v.y * rhs, v.z * rhs);
}

Vector3D Vector3D::negate(const Vector3D& v)
{
    return Vector3D(-v.x, -v.y, -v.z);
}
