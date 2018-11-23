//
//  Polygon.cpp
//  CityBuilder
//
//  Created by Alessandro Profenna on 2018-11-01.
//  Copyright © 2018 Alessandro Profenna. All rights reserved.
//

#include "Polygon.hpp"

Polygon::Polygon():
    normal(Vector3D()),
    isFrontFacing(true),
    verts(){}

void Polygon::calculateNormal()
{
    if(verts.size() >= 3)
    {
        Vector3D v1 = Vector3D::subtract(verts[0], verts[1]);
        Vector3D v2 = Vector3D::subtract(verts[0], verts[2]);
        if(isFrontFacing)
        {
            normal = Vector3D::crossProduct(v1, v2);
        }
        else
        {
            normal = Vector3D::crossProduct(v2, v1);
        }
        normal.normalize();
    }
}

void Polygon::draw()
{
    glBegin(GL_POLYGON);
    glNormal3f(normal.x, normal.y, normal.z);
    for(int i = 0; i < verts.size(); i++){
        glVertex3f(verts[i].x, verts[i].y, verts[i].z);
    }
    glEnd();
}

void Polygon::draw(int texID, vector<Vector2D> stCoordinates)
{
    glBindTexture(GL_TEXTURE_2D, texID);
    glEnable(GL_TEXTURE_2D);
    
    glBegin(GL_POLYGON);
    glNormal3f(normal.x, normal.y, normal.z);
    for(int i = 0; i < verts.size(); i++){
        glTexCoord2f(stCoordinates[i].x, stCoordinates[i].y);
        glVertex3f(verts[i].x, verts[i].y, verts[i].z);
    }
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
}
