//
//  Building.cpp
//  CityBuilder
//
//  Created by Alessandro Profenna on 2018-11-05.
//  Copyright © 2018 Alessandro Profenna. All rights reserved.
//

#include "Building.hpp"

Building::Building():
    PrismMesh(),
    floorHeight(0.08),
    selectedSplineCP(-1),
    selectedBaseCP(-1)
    {
        initializeCpScales();
        build();
    }

Building::Building(int numEdges, float height, float rotY, float posX, float posY, Vector3D scale, float floorHeight):
    PrismMesh(numEdges, height, rotY, posX, posY, scale),
    floorHeight(floorHeight),
    selectedSplineCP(-1),
    selectedBaseCP(-1)
    {
        initializeCpScales();
        build();
    }

void Building::initializeCpScales()
{
    for(int i = 0; i < numControlPoints; i++)
    {
        cpSplineScales.push_back(1.0);
    }
    for(int i = 0; i < numBaseEdges; i++)
    {
        cpBaseScales.push_back(1.0);
    }
}


void Building::build()
{
    int numFloors = getNumFloors();
    float preFloorHeight = initialHeight/numFloors; //Floor height before vertical scaling is applied
    vector< vector<Vector3D> > floors;
    
    //Spline curve function used for determining the extrusion factor for each floor
    verticalSpline = createSpline();
    
    //Creates vertices for the building
    vector<Vector3D> floor1Verts;
    for(int i = 0; i < numBaseEdges; i++)
    {
        float angle = (360.0/numBaseEdges)*i;
        if(numBaseEdges == 4)
        {
            //Better alignment for scaling rectangles/squares
            angle += 45;
        }
        float x = (sin(angle * PI / 180.0)*(initialHeight/2))*cpBaseScales.at(i);
        float z = (cos(angle * PI / 180.0)*(initialHeight/2))*cpBaseScales.at(i);
        floor1Verts.push_back(Vector3D(x, -(initialHeight/2), z));
    }
    
    for(int i = 0; i <= numFloors; i++)
    {
        vector<Vector3D> nextFloorVerts = floor1Verts;
        for(int j = 0; j < numBaseEdges; j++)
        {
            nextFloorVerts.at(j).x *= (float)verticalSpline(i);
            nextFloorVerts.at(j).y = -(initialHeight/2) + (preFloorHeight*i);
            nextFloorVerts.at(j).z *= (float)verticalSpline(i);
        }
        floors.push_back(nextFloorVerts);
    }
    
    //Creates quads for the building
    quads.clear();
    for(int i = 0; i < numFloors; i++)
    {
        for(int j = 0; j < numBaseEdges; j++)
        {
            Polygon newQuad = Polygon();
            newQuad.verts.push_back(floors.at(i).at(j));
            newQuad.verts.push_back(floors.at(i + 1).at(j));
            if(j == 0)
            {
                newQuad.verts.push_back(floors.at(i + 1).at(numBaseEdges-1));
                newQuad.verts.push_back(floors.at(i).at(numBaseEdges-1));
            }
            else{
                newQuad.verts.push_back(floors.at(i + 1).at(j-1));
                newQuad.verts.push_back(floors.at(i).at(j-1));
            }
            newQuad.calculateNormal();
            quads.push_back(newQuad);
        }
    }
    
    //Creates bottom floor and roof polygons
    baseBottom.verts.clear();
    baseTop.verts.clear();
    if(floors.size() > 0)
    {
        baseBottom.verts = floors.at(0);
        baseTop.verts = floors.at(floors.size()-1);
    }
    baseBottom.isFrontFacing = false;
    baseBottom.calculateNormal();
    baseTop.calculateNormal();
}

void Building::changeNumSides(int changeNum)
{
    float diff = abs(changeNum);
    for(int i = 0; i < diff; i++)
    {
        if(changeNum < 0 && numBaseEdges > 3)
        {
            cpBaseScales.pop_back();
        }
        else if(changeNum > 0)
        {
            cpBaseScales.push_back(1.0);
        }
    }
    
    numBaseEdges += changeNum;
    if (numBaseEdges < 3)
    {
        numBaseEdges = 3;
    }
    build();
}

int Building::getNumFloors()
{
    return ceil(currentHeight/floorHeight);
}


tk::spline Building::createSpline()
{
    int numFloors = getNumFloors();
    vector<double> cpIndices;
    double cpIndexInterval = numFloors/(numControlPoints-1);
    for(int i = 0; i < numControlPoints; i++)
    {
        cpIndices.push_back(cpIndexInterval*i);
    }
    tk::spline s;
    s.set_points(cpIndices, cpSplineScales);
    return s;
}

void Building::drawSpline(float yLength)
{
    int numFloors = getNumFloors();
    float splineFloorHeight = yLength/numFloors;
    float lengthHeightRatio = yLength/initialHeight;
    
    //Draws the spline curve using each floor number as inputs
    glBegin(GL_LINE_STRIP);
    for(int i = 0; i <= numFloors; i++){
        float vX = verticalSpline(i)*lengthHeightRatio-lengthHeightRatio;
        float vY = splineFloorHeight*i;
        glVertex3f(vX, vY, 0);
    }
    glEnd();
    
    //Draws the control points along the spline curve
    double cpIndexInterval = numFloors/(numControlPoints-1);
    for(int i = 0; i < cpSplineScales.size(); i++){
        glPushMatrix();
        float cpX = verticalSpline(cpIndexInterval*i)*lengthHeightRatio-lengthHeightRatio;
        float cpY = splineFloorHeight*cpIndexInterval*i;
        glTranslatef(cpX, cpY, 0);
        glutSolidSphere(2.5, 20, 20);
        glPopMatrix();
    }
}

void Building::drawBase()
{
    glPushMatrix();
    Polygon base = baseBottom;
    //glRotatef(rotationY, 0.0, 1.0, 0.0);
    for(int i = 0; i <base.verts.size(); i++)
    {
        base.verts.at(i).x *= scaleFactors.x / verticalSpline(0);
        base.verts.at(i).z *= scaleFactors.z / verticalSpline(0);
        glPushMatrix();
        glTranslatef(base.verts.at(i).x, 0.0, base.verts.at(i).z);
        glutSolidSphere(0.5/numBaseEdges, 20, 20);
        glPopMatrix();
    }
    //glScalef(scaleFactors.x, 1.0, scaleFactors.z);
    glTranslatef(0.0, position.y, 0.0);
    base.draw();
    glPopMatrix();
}

void Building::changeSplineControlPoint(int cpIndex, float newX)
{
    if(cpIndex >= 0 && cpIndex < numControlPoints)
    {
        cpSplineScales.at(cpIndex) = newX;
        build();
    }
}

void Building::checkSplineControlPoint(float wvX, float wvY, float yLength)
{
    int numFloors = getNumFloors();
    float splineFloorHeight = yLength/numFloors;
    float lengthHeightRatio = yLength/initialHeight;
    
    double cpIndexInterval = numFloors/(numControlPoints-1);
    for(int i = 0; i < cpSplineScales.size(); i++)
    {
        float cpX = verticalSpline(cpIndexInterval*i)*lengthHeightRatio-lengthHeightRatio;
        float cpY = splineFloorHeight*cpIndexInterval*i;
        if(abs(cpX - wvX) < 2.5 && abs(cpY - wvY) < 2.5){
            //This is the selected control point
            selectSplineControlPoint(i);
            break;
        }
    }
}

void Building::selectSplineControlPoint(int cpIndex)
{
    selectedSplineCP = cpIndex;
}

void Building::shiftSelectedSplineControlPoint(float wvX, float yLength)
{
    float lengthHeightRatio = yLength/initialHeight;
    changeSplineControlPoint(selectedSplineCP, (wvX + lengthHeightRatio)/lengthHeightRatio);
}

void Building::checkBaseControlPoint(float wvX, float wvY)
{
    for(int i = 0; i < baseBottom.verts.size(); i++)
    {
        float cpXscaled = baseBottom.verts.at(i).x * scaleFactors.x / verticalSpline(0);
        float cpZscaled = baseBottom.verts.at(i).z * scaleFactors.z / verticalSpline(0);
        
        float cpX = cpXscaled;//*sin(rotationY) + cpXscaled*cos(rotationY);
        float cpZ = cpZscaled;//*cos(rotationY) - cpXscaled*sin(rotationY);
        cpZ = -cpZ;
        
        
        float cpRadius = 0.5/numBaseEdges;
        if(abs(cpX - wvX) < cpRadius && abs(cpZ - wvY) < cpRadius){
            //This is the selected control point
            selectBaseControlPoint(i);
        }
    }
}

void Building::selectBaseControlPoint(int cpIndex)
{
    selectedBaseCP = cpIndex;
}

void Building::shiftSelectedBaseControlPoint(float wvX, float wvY)
{
    //If the user changes the number of sides of the building while a control point is selected
    //the selected control point will be reset back to -1
    if(selectedBaseCP >= cpBaseScales.size())
    {
        selectBaseControlPoint(-1);
    }
    //Else, the selected control point of the base can be moved
    else
    {
        //Distance of the mouse from the centre of the base
        float mouseDistance = sqrt(wvX * wvX + wvY * wvY);
        //Distance of control point from the centre of the base, before scaling
        float cpDistance = initialHeight/2;
        float newBaseScale = mouseDistance/cpDistance;
        changeBaseControlPoint(selectedBaseCP, newBaseScale);
    }
}

void Building::changeBaseControlPoint(int cpIndex, float newScale)
{
    if(cpIndex >= 0 && cpIndex < numBaseEdges)
    {
        cpBaseScales.at(cpIndex) = newScale;
        build();
    }
}

void Building::changeScaleFactors(Vector3D scaleDeltas)
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
    position.y = currentHeight/2;
    build();
}

bool Building::checkObjectCollision(Vector3D objectPos)
{
    if(objectPos.y > currentHeight || objectPos.y < 0)
    {
        return false;
    }
    // The floor height of the drone
    // (example: drone could be 4.6 floor heights from the ground)
    float droneFloor = objectPos.y/floorHeight;
    
    // The scale factors of the floor level below and above the drone
    float floorBelowScale = verticalSpline(floor(droneFloor));
    float floorAboveScale = verticalSpline(ceil(droneFloor));
    
    // Linerly interpolated scale at the exact height of the drone
    float lerpScale = floorBelowScale + ((droneFloor - floor(droneFloor))*(floorAboveScale - floorBelowScale));
    
    float collisionRadius = (initialHeight/2)*lerpScale*scaleFactors.x;
    
    //distance between the drone' position and building's position in the XZ plane
    Vector2D distanceVec(position.x - objectPos.x, position.z - objectPos.z);
    float distance = distanceVec.getLength();
    
    if(distance < collisionRadius)
    {
        return true;
    }
    else
    {
        return false;
    }
}


string Building::getMetaData()
{
    string md = "---------\n";
    md += to_string(numBaseEdges) + "\n";
    md += to_string(initialHeight) + "\n";
    md += to_string(currentHeight) + "\n";
    md += to_string(floorHeight) + "\n";
    md += to_string(rotationY) + "\n";
    md += to_string(scaleFactors.x) + " " + to_string(scaleFactors.y) + " " + to_string(scaleFactors.z) + "\n";
    md += to_string(position.x) + " " + to_string(position.y) + " " + to_string(position.z) + "\n";
    for(int i = 0; i < cpSplineScales.size(); i++)
    {
        md += to_string(cpSplineScales.at(i)) + " ";
    }
    md += "\n";
    for(int i = 0; i < cpBaseScales.size(); i++)
    {
        md += to_string(cpBaseScales.at(i)) + " ";
    }
    md += "\n";
    return md;
}

void Building::processMetaData(string md)
{
    istringstream iss(md);
    
    int i = 0;
    for (string line; getline(iss, line);)
    {
        stringstream s(line);
        if(i == 0) s >> numBaseEdges;
        else if(i == 1) s >> initialHeight;
        else if(i == 2) s >> currentHeight;
        else if(i == 3) s >> floorHeight;
        else if(i == 4) s >> rotationY;
        else if(i >= 5 && i <= 8)
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
            
            if(i == 5)
            {
                stringstream s1(tokens[0]);
                s1 >> scaleFactors.x;
                stringstream s2(tokens[1]);
                s2 >> scaleFactors.y;
                stringstream s3(tokens[2]);
                s3 >> scaleFactors.z;
            }
            else if(i == 6)
            {
                stringstream s1(tokens[0]);
                s1 >> position.x;
                stringstream s2(tokens[1]);
                //s2 >> position.y;
                stringstream s3(tokens[2]);
                s3 >> position.z;
            }
            else if(i == 7)
            {
                cpSplineScales.clear();
                for(int j = 0; j < tokens.size(); j++)
                {
                    stringstream s(tokens[j]);
                    float scale;
                    s >> scale;
                    cpSplineScales.push_back(scale);
                }
            }
            else if(i == 8)
            {
                cpBaseScales.clear();
                for(int j = 0; j < tokens.size(); j++)
                {
                    stringstream s(tokens[j]);
                    float scale;
                    s >> scale;
                    cpBaseScales.push_back(scale);
                }
            }
        }
        i++;
    }
    position.y = currentHeight/2;
    
    build();
}



