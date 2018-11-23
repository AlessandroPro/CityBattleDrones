//
//  Building.hpp
//  CityBuilder
//
//  Created by Alessandro Profenna on 2018-11-05.
//  Copyright © 2018 Alessandro Profenna. All rights reserved.
//

#ifndef Building_hpp
#define Building_hpp

#include <stdio.h>
#include "Polygon.hpp"
#include "PrismMesh.hpp"
#include "Spline.cpp"
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#define PI 3.14159265

using namespace std;

class Building : public PrismMesh
{
    float floorHeight;
    const double numControlPoints = 6;
    std::vector<double> cpSplineScales;
    std::vector<double> cpBaseScales;
    tk::spline verticalSpline;
    int selectedSplineCP;
    int selectedBaseCP;
    
public:
    
    Building();
    Building(int numEdges, float height, float rotY, float posX, float posY, Vector3D scale, float floorHeight);
    void initializeCpScales();
    void build();
    void changeNumSides(int changeNum);
    void changeScaleFactors(Vector3D scaleDeltas);
    int getNumFloors();
    tk::spline createSpline();
    void drawSpline(float yLength);
    void drawBase();
    void changeSplineControlPoint(int cpIndex, float deltaX);
    void checkSplineControlPoint(float wvX, float wvY, float yLength);
    void selectSplineControlPoint(int cpIndex);
    void shiftSelectedSplineControlPoint(float wvX, float yLength);
    void checkBaseControlPoint(float wvX, float wvY);
    void selectBaseControlPoint(int cpIndex);
    void shiftSelectedBaseControlPoint(float wvX, float wvY);
    void changeBaseControlPoint(int cpIndex, float newScale);
    string getMetaData();
    void processMetaData(string md);
};

#endif /* Building_hpp */
