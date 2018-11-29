//
//  Missile.cpp
//  CityBattleDrones
//
//  Created by Alessandro Profenna on 2018-11-29.
//  Copyright © 2018 Alessandro Profenna. All rights reserved.
//

#include "Missile.hpp"

Missile::Missile():
    scaleFactor(1.0),
    speed(10),
    length(5),
    bodyTexture(0),
    headTexture(0),
    position(Vector3D(0,0,0)),
    forward(Vector3D(1,0,0)),
    rotation(Vector3D(0,0,0)),
    targetPos(Vector3D(10, 10, 10)),
    body(PrismMesh(20))
    {}

Missile::Missile(float scaleFactor, float speed, Vector3D position, Vector3D forward, Vector3D targetPos, int bodyTex, int headTex):
    scaleFactor(scaleFactor),
    speed(speed),
    length(5),
    bodyTexture(bodyTex),
    headTexture(headTex),
    position(position),
    forward(forward),
    rotation(Vector3D(0,0,0)),
    targetPos(targetPos),
    body(PrismMesh(20))
    {}

void Missile::update()
{
    changeForward(targetPos, 0.03);
    moveForward(speed);
}

void Missile::draw()
{
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    
    // Align with forward
    Vector3D defaultOrientation = Vector3D(0,0,1);
    defaultOrientation.normalize();
    forward.normalize();
    float aDotB = Vector3D::DotProduct(defaultOrientation, forward);
    float alignAngle = RADTODEG * acosf(aDotB);
    Vector3D cross = Vector3D::crossProduct(defaultOrientation, forward);
    cout << alignAngle << "\n";
    glRotatef(alignAngle, cross.x, cross.y, cross.z);
    
    glScalef(scaleFactor/2, scaleFactor/2, scaleFactor);
    glScalef(1, 1, length);
    glRotatef(90, 1, 0, 0);
    body.draw();
    for(int i = 0; i < body.getNumSides(); i++)
    {
        // Triangle polygon to make pyramid head for the missle
        vector<Vector3D> triVerts({});
        triVerts.push_back(body.baseTop.verts[i]);
        if(i == body.getNumSides() - 1)
        {
           triVerts.push_back(body.baseTop.verts[0]);
        }
        else
        {
           triVerts.push_back(body.baseTop.verts[i + 1]);
        }
        triVerts.push_back(Vector3D(0, 0.8, 0));
        Polygon triangle;
        triangle.verts = triVerts;
        triangle.calculateNormal();
        triangle.draw();
    }
    glPopMatrix();
}

void Missile::moveForward(float speed)
{
    Vector3D velocity = Vector3D::scalarMul(forward, speed);
    position = Vector3D::add(position, velocity);
}

void Missile::alignRotation(Vector3D direction)
{
    float rotY = atan(direction.x/direction.z);
}

void Missile::changeForward(Vector3D targetPos, float changeDelta)
{
    Vector3D targetDir = Vector3D::subtract(targetPos, position);
    Vector3D changeDir = Vector3D::subtract(targetDir, forward);
    changeDir.normalize();
    changeDir = Vector3D::scalarMul(changeDir, changeDelta);
    
    forward = Vector3D::add(forward, changeDir);
    forward.normalize();
}
