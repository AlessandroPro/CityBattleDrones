//
//  PrismMesh.cpp
//  CityBuilder
//
//

#include "PrismMesh.hpp"

PrismMesh::PrismMesh():
    numBaseEdges(4),
    rotationY(0.0),
    initialHeight(1.0),
    currentHeight(initialHeight),
    scaleFactors(Vector3D(1.0, 1.0, 1.0)),
    position(Vector3D(0.0, 0.0, 0.0))
    {
        build();
    }

PrismMesh::PrismMesh(int numEdges):
    numBaseEdges(numEdges),
    rotationY(0.0),
    initialHeight(1.0),
    currentHeight(initialHeight),
    scaleFactors(Vector3D(1.0, 1.0, 1.0)),
    position(Vector3D(0.0, 0.0, 0.0))
    {
        build();
    }

PrismMesh::PrismMesh(int numEdges, float height, float rotY, float posX, float posZ, Vector3D scale):
    numBaseEdges(numEdges),
    rotationY(rotY),
    initialHeight(height),
    currentHeight(initialHeight),
    scaleFactors(scale),
    position(Vector3D(posX, 0.0, posZ))
    {
        build();
    }

// Draw the prism without any textures
void PrismMesh::draw()
{
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glScalef(scaleFactors.x, scaleFactors.y, scaleFactors.z);
    baseBottom.draw();
    baseTop.draw();
    for (auto& quad : quads)
    {
        quad.draw();
    }
    glPopMatrix();
}

// Draw the prism with the same texture for all of its sides and a different texture for base top and bottom
// quadTexID - the texture for each prism side quad, which is mapped based on sQuadCoords and tQuadCoords
// baseTexID - the texture for the base top and bottom, which is only applied if baseTex is set to True
void PrismMesh::draw(int quadTexID, vector<Vector2D> stQuadCoords, bool baseTex, int baseTexID)
{
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glScalef(scaleFactors.x, scaleFactors.y, scaleFactors.z);
    
    for (auto& quad : quads)
    {
        quad.draw(quadTexID, stQuadCoords, false);
    }
    
    if(baseTex)
    {
        vector<Vector2D> stBaseCoords;
    
        for(int i = 0; i < numBaseEdges; i++)
        {
            float angle = (360.0/numBaseEdges)*i;
            
            float norm = 0.5;
            float x = sin(angle * PI / 180.0)*norm + 0.5;
            float z = cos(angle * PI / 180.0)*norm + 0.5;
            
            stBaseCoords.push_back(Vector2D(z,x));
        }
        cout << "\n";
        //if the base is a square/rectangle, fit it perfectly to the texture
        if(numBaseEdges == 4)
        {
            stBaseCoords.clear();
            stBaseCoords = {Vector2D(0,0), Vector2D(0,1), Vector2D(1,1), Vector2D(1,0)};
        }
        baseBottom.draw(baseTexID, stBaseCoords, false);
        baseTop.draw(baseTexID, stBaseCoords, false);
    }
    else
    {
        baseBottom.draw();
        baseTop.draw();
    }
    glPopMatrix();
}

// Draw the prism based on one texture, with each face of the prism mapped to a unique location of the texture
// Each prism side quad is mapped differently based on their respective st coords in stSideCoords
// The texture for the top of the prism is mapped based on the st coords in stTopCoords
// The texture for the Tottom of the prism is mapped based on the st coords in stBottomCoords
void PrismMesh::draw(int texID, vector< vector<Vector2D> > stSideCoords, vector<Vector2D> stTopCoords, vector<Vector2D> stBottomCoords)
{
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glScalef(scaleFactors.x, scaleFactors.y, scaleFactors.z);
    
    baseBottom.draw(texID, stBottomCoords, false);
    baseTop.draw(texID, stTopCoords, false);

    for (int i = 0; i < quads.size(); i++)
    {
        quads[i].draw(texID, stSideCoords[i], false);
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

int PrismMesh::getNumSides()
{
    return numBaseEdges;
}




