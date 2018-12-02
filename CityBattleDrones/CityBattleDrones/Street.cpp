//
//  Street.cpp
//  CityBuilder
//
//  Created by Alessandro Profenna on 2018-11-30.
//  Copyright © 2018 Alessandro Profenna. All rights reserved.
//

#include "Street.hpp"


Street::Street():
    scaleX(1),
    scaleZ(15),
    rotationY(0),
    position(Vector3D(0,0,0))
    {
        build();
    }

void Street::build()
{
    streetPoly = Polygon();
    vector<Vector3D> verts({Vector3D(-1,0,-1), Vector3D(-1,0,1), Vector3D(1,0,1), Vector3D(1,0,-1)});
    streetPoly.verts = verts;
    streetPoly.calculateNormal();
}

void Street::draw()
{
    glPushMatrix();
    glTranslatef(position.x, 0.0, position.z);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glScalef(scaleX, 1.0, scaleZ);
    streetPoly.draw();
    glPopMatrix();
}

void Street::draw(int textID)
{
    float streetLength = scaleX*2;
    float streetWidth = scaleZ*2;
    float ratio = 3; // street texture should be 3 times longer than its width
    
    float numRepeat = streetWidth/(ratio*streetLength);
    vector<Vector2D> stCoordinates = {Vector2D(0,0), Vector2D(0,numRepeat), Vector2D(1,numRepeat), Vector2D(1,0)};
    
    glPushMatrix();
    glTranslatef(position.x, 0.0, position.z);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glScalef(scaleX, 1.0, scaleZ);
    streetPoly.draw(textID, stCoordinates, false);
    glPopMatrix();
}
    

void Street::rotateY(float deltaY)
{
    rotationY += deltaY;
}

void Street::moveAlongGround(float deltaX, float deltaZ)
{
    position.x += deltaX*0.5;
    position.z += deltaZ*0.5;
}

void Street::changeScaleFactors(float deltaX, float deltaZ)
{
    scaleX += deltaX;
    scaleZ += deltaZ;
    
    if(scaleX < 0)
    {
        scaleX = 0;
    }
    
    if(scaleZ < 0)
    {
        scaleZ = 0;
    }
}

string Street::getMetaData()
{
    string md = "+++++\n";
    md += to_string(scaleX) + "\n";
    md += to_string(scaleZ) + "\n";
    md += to_string(rotationY) + "\n";
    md += to_string(position.x) + " " + to_string(position.y) + " " + to_string(position.z) + "\n";
    return md;
}

void Street::processMetaData(string md)
{
    istringstream iss(md);
    
    int i = 0;
    for (string line; getline(iss, line);)
    {
        stringstream s(line);
        if(i == 0) s >> scaleX;
        else if(i == 1) s >> scaleZ;
        else if(i == 2) s >> rotationY;
        else if(i == 3)
        {
            // Vector of string to save tokens
            vector<string> tokens;
            // stringstream class check1
            stringstream check1(line);
            string intermediate;
            // Tokenizing w.r.t. space ' '
            while(getline(check1, intermediate, ' '))
            {
                tokens.push_back(intermediate);
            }
            
            stringstream s1(tokens[0]);
            s1 >> position.x;
            stringstream s2(tokens[1]);
            s2 >> position.y;
            stringstream s3(tokens[2]);
            s3 >> position.z;
        }
        i++;
    }
    build();
}

