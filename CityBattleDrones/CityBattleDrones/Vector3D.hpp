//
//  Vector3D.hpp
//  CityBuilder
//
//  Created by Alessandro Profenna on 2018-11-01.
//  Copyright © 2018 Alessandro Profenna. All rights reserved.
//

#ifndef Vector3D_h
#define Vector3D_h

#include <math.h>

class Vector3D
{
    public:
    
        float x, y, z;
    
        Vector3D();
        Vector3D(float x, float y, float z);
        void set(float newX, float newY, float newZ);
        void loadZero();
        void loadOne();
        void normalize();
        float getLength();
    
        static Vector3D crossProduct(const Vector3D& lhs, const Vector3D& rhs);
        static float DotProduct(const Vector3D& lhs, const Vector3D& rhs);
        static Vector3D add(const Vector3D& v1, const Vector3D& v2);
        static Vector3D subtract(const Vector3D& v1, const Vector3D& v2);
        static Vector3D scalarMul(const Vector3D& v, const float rhs);
        static Vector3D negate(const Vector3D& v);
};

#endif /* Vector3D_h */
