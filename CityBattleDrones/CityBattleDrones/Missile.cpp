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
    body(PrismMesh(20)),
    isDestroyed(true),
    timeDestroyed(-1)
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
    body(PrismMesh(20)),
    isDestroyed(true),
    timeDestroyed(-1)
    {}

void Missile::update()
{
    if(!isDestroyed)
    {
        changeForward(targetPos, 0.04);
        moveForward(speed);
    }
}

void Missile::draw()
{
    vector<Vector2D> stCoordinates = {Vector2D(1,0), Vector2D(1,1), Vector2D(0,1), Vector2D(0,0)};
    if(!isDestroyed)
    {
        vector<Vector2D> stBodyCoords = {Vector2D(0,0), Vector2D(0,1), Vector2D(1,1), Vector2D(1,0)};
        vector<Vector2D> stHeadTriangle = {Vector2D(0,0), Vector2D(0,1), Vector2D(0.5, 1)};
        
        glPushMatrix();
        glTranslatef(position.x, position.y, position.z);
        
        // Align with forward
        alignRotation(forward);
        
        glScalef(scaleFactor/2, scaleFactor/2, scaleFactor);
        glScalef(1, 1, length);
        glRotatef(90, 1, 0, 0);
        body.draw(bodyTexture, stBodyCoords, true, bodyTexture);
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
            triangle.draw(headTexture, stHeadTriangle, false);
        }
        glPopMatrix();
    }
    else
    {
        float timeSinceDestroyed = glutGet(GLUT_ELAPSED_TIME) - timeDestroyed;
        
        if(timeSinceDestroyed < 2000)
        {
            // Animates a smoke texture to represent the destruction of the missile
            glPushMatrix();
            vector<Vector3D> verts({Vector3D(1,-1,0), Vector3D(1,1,0), Vector3D(-1,1,0), Vector3D(-1,-1,0)});
            Polygon smoke;
            smoke.verts = verts;
            float animPercent = timeSinceDestroyed/1000;
            glTranslatef(position.x, position.y, position.z);
            glScalef(scaleFactor, scaleFactor, scaleFactor);
            glColor4f(1.0, 1.0, 1.0, 1 - animPercent);
            glScalef(0.3 + animPercent*4, 0.3 + animPercent*5, 0.3 + animPercent*4);
            glScalef(3, 2, 3);
            smoke.draw(2015, stCoordinates, true);
            glPopMatrix();
        }
    }
}

void Missile::moveForward(float speed)
{
    Vector3D velocity = Vector3D::scalarMul(forward, speed);
    position = Vector3D::add(position, velocity);
}

void Missile::alignRotation(Vector3D direction)
{
    Vector3D defaultOrientation = Vector3D(0,0,1);
    defaultOrientation.normalize();
    forward.normalize();
    float aDotB = Vector3D::DotProduct(defaultOrientation, forward);
    float alignAngle = RADTODEG * acosf(aDotB);
    Vector3D cross = Vector3D::crossProduct(defaultOrientation, forward);
    glRotatef(alignAngle, cross.x, cross.y, cross.z);
}

void Missile::changeForward(Vector3D targetPos, float changeDelta)
{
    float distance = Vector3D::subtract(targetPos, position).getLength();
    
    if(distance < 3)
    {
        Vector3D targetDir = Vector3D::subtract(targetPos, position);
        Vector3D changeDir = Vector3D::subtract(targetDir, forward);
        changeDir.normalize();
        changeDir = Vector3D::scalarMul(changeDir, changeDelta);
        
        forward = Vector3D::add(forward, changeDir);
        forward.normalize();
    }
}

Vector3D Missile::getPosition()
{
    return position;
}

void Missile::setPosition(Vector3D newPos)
{
    position = newPos;
}

void Missile::setForward(Vector3D newForward)
{
    forward = newForward;
}

void Missile::blowUp()
{
    isDestroyed = true;
    timeDestroyed = glutGet(GLUT_ELAPSED_TIME);
}

void Missile::setTargetPos(Vector3D newTargetPos)
{
    targetPos = newTargetPos;
}

