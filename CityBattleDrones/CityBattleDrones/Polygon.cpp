//
//  Polygon.cpp
//  CityBuilder
//
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
    if(isFrontFacing)
    {
        for(int i = 0; i < verts.size(); i++)
        {
            glVertex3f(verts[i].x, verts[i].y, verts[i].z);
        }
    }
    else
    {
        for(int i = (int)verts.size(); i > 0; i--)
        {
            glVertex3f(verts[i-1].x, verts[i-1].y, verts[i-1].z);
        }
    }
    glEnd();
}

void Polygon::draw(int texID, vector<Vector2D> stCoordinates, bool hasAlpha)
{
    glBindTexture(GL_TEXTURE_2D, texID);
    glEnable(GL_TEXTURE_2D);
    
    if(hasAlpha)
    {
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    
    glBegin(GL_POLYGON);
    glNormal3f(normal.x, normal.y, normal.z);
    if(isFrontFacing)
    {
        for(int i = 0; i < verts.size(); i++)
        {
            glTexCoord2f(stCoordinates[i].x, stCoordinates[i].y);
            glVertex3f(verts[i].x, verts[i].y, verts[i].z);
        }
    }
    else
    {
        for(int i = (int)verts.size(); i > 0; i--)
        {
            glTexCoord2f(stCoordinates[i-1].x, stCoordinates[i-1].y);
            glVertex3f(verts[i-1].x, verts[i-1].y, verts[i-1].z);
        }
    }
    glEnd();
    
    if(hasAlpha)
    {
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
    }
    
    glDisable(GL_TEXTURE_2D);
}
