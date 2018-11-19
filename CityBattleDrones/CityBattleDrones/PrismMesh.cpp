//
//  PrismMesh.cpp
//  CityBuilder
//
//  Created by Alessandro Profenna on 2018-11-01.
//  Copyright © 2018 Alessandro Profenna. All rights reserved.
//

#include "PrismMesh.hpp"

PrismMesh::PrismMesh():
    numBaseEdges(8),
    rotationY(0.0),
    initialHeight(1.0), //Initial height is 100meters/unit
    currentHeight(initialHeight),
    scaleFactors(Vector3D(1.0, 1.0, 1.0)),
    position(Vector3D(0.0, initialHeight/2, 0.0))
    {
        build();
    }

PrismMesh::PrismMesh(int numEdges, float height, float rotY, float posX, float posZ, Vector3D scale):
    numBaseEdges(numEdges),
    rotationY(rotY),
    initialHeight(height),
    currentHeight(initialHeight),
    scaleFactors(scale),
    position(Vector3D(posX, initialHeight/2, posZ))
    {
        build();
    }

void PrismMesh::draw()
{
    glPushMatrix();
    //glTranslatef(position.x, 0.0, position.z);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glScalef(scaleFactors.x, scaleFactors.y, scaleFactors.z);
    //glTranslatef(0.0, position.y, 0.0);
    baseBottom.draw();
    baseTop.draw();
    for (auto& quad : quads)
    {
        quad.draw();
    }
    glPopMatrix();
}

void PrismMesh::draw(vector<float> sCoordinates,vector<float> tCoordinates)
{
    glPushMatrix();
    //glTranslatef(position.x, 0.0, position.z);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glScalef(scaleFactors.x, scaleFactors.y, scaleFactors.z);
    //glTranslatef(0.0, position.y, 0.0);
    baseBottom.draw();
    baseTop.draw();
    for (auto& quad : quads)
    {
        quad.draw(sCoordinates, tCoordinates);
    }
    glPopMatrix();
}

void PrismMesh::build()
{
    baseBottom.verts.clear();
    baseTop.verts.clear();
    quads.clear();
    for(int i = 0; i < numBaseEdges; i++)
    {
        float angle = (360.0/numBaseEdges)*i;
        if(numBaseEdges == 4)
        {
            //Better alignment for scaling rectangles/squares
            angle += 45;
        }
        float norm = sqrt(2*pow(initialHeight/2, 2));
        float x = sin(angle * PI / 180.0)*norm;
        float z = cos(angle * PI / 180.0)*norm;
        baseBottom.verts.push_back(Vector3D(x, -(initialHeight/2), z));
        baseTop.verts.push_back(Vector3D(x, (initialHeight/2), z));
    }
    
    for(int i = 0; i < numBaseEdges; i++)
    {
        Polygon newQuad = Polygon();
        newQuad.verts.push_back(baseBottom.verts.at(i));
        newQuad.verts.push_back(baseTop.verts.at(i));
        if(i == 0)
        {
            newQuad.verts.push_back(baseTop.verts.at(numBaseEdges-1));
            newQuad.verts.push_back(baseBottom.verts.at(numBaseEdges-1));
        }
        else{
            newQuad.verts.push_back(baseTop.verts.at(i-1));
            newQuad.verts.push_back(baseBottom.verts.at(i-1));
        }
        newQuad.calculateNormal();
        quads.push_back(newQuad);
    }
    baseBottom.isFrontFacing = false;
    baseBottom.calculateNormal();
    baseTop.calculateNormal();
}


void PrismMesh::changeNumSides(int changeNum)
{
    numBaseEdges += changeNum;
    if (numBaseEdges < 3)
    {
        numBaseEdges = 3;
    }
    build();
}

void PrismMesh::rotateY(float deltaY)
{
    rotationY += deltaY;
}

void PrismMesh::moveAlongGround(float deltaX, float deltaZ)
{
    position.x += deltaX*0.5;
    position.z += deltaZ*0.5;
}

void PrismMesh::changeScaleFactors(Vector3D scaleDeltas)
{
    scaleFactors.x += scaleDeltas.x;
    scaleFactors.y += scaleDeltas.y;
    scaleFactors.z += scaleDeltas.z;
    
    if(scaleFactors.x < minScaleFactor)
    {
        scaleFactors.x = minScaleFactor;
    }
    
    if(scaleFactors.y < minScaleFactor)
    {
        scaleFactors.y = minScaleFactor;
    }
    
    if(scaleFactors.z < minScaleFactor)
    {
        scaleFactors.z = minScaleFactor;
    }
    
    currentHeight = initialHeight * scaleFactors.y;
}




