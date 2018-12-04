//
//  Vector2D.hpp
//  CityBattleDrones
//
//

#ifndef Vector2D_hpp
#define Vector2D_hpp

#include <math.h>

class Vector2D
{
public:
    
    float x, y;
    
    Vector2D();
    Vector2D(float x, float y);
    void set(float newX, float newY);
    void loadZero();
    void loadOne();
    void normalize();
    float getLength();
    
    static float DotProduct(const Vector2D& lhs, const Vector2D& rhs);
    static Vector2D add(const Vector2D& v1, const Vector2D& v2);
    static Vector2D subtract(const Vector2D& v1, const Vector2D& v2);
    static Vector2D scalarMul(const Vector2D& v, const float rhs);
    static Vector2D negate(const Vector2D& v);
};

#endif /* Vector2D_hpp */
