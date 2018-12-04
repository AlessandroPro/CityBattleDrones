//
//  Vector2D.cpp
//  CityBattleDrones
//
//

#include "Vector2D.hpp"

Vector2D::Vector2D():
x(1.0),
y(1.0)
{}

Vector2D::Vector2D(float x, float y):
x(x),
y(y)
{}

void Vector2D::set(float newX, float newY)
{
    x = newX; y = newY;
}

void Vector2D::loadZero()
{
    x = y = 0.0;
}

void Vector2D::loadOne()
{
    x = y = 1.0;
}

void Vector2D::normalize()
{
    const float norm = getLength();
    if (norm > 0)
    {
        x /= norm; y /= norm;
    }
}

float Vector2D::getLength()
{
    return (float)sqrt(x*x + y*y);
}

float Vector2D::DotProduct(const Vector2D& lhs, const Vector2D& rhs)
{
    return lhs.x*rhs.x + lhs.y*rhs.y;
}

Vector2D Vector2D::add(const Vector2D& v1, const Vector2D& v2)
{
    return Vector2D(v1.x + v2.x, v1.y + v2.y);
}

Vector2D Vector2D::subtract(const Vector2D& v1, const Vector2D& v2)
{
    return Vector2D(v1.x - v2.x, v1.y - v2.y);
}

Vector2D Vector2D::scalarMul(const Vector2D& v, float rhs)
{
    return Vector2D(v.x * rhs, v.y * rhs);
}

Vector2D Vector2D::negate(const Vector2D& v)
{
    return Vector2D(-v.x, -v.y);
}

