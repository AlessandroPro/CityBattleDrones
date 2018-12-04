/*******************************************************************
           City Battle Drones
********************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <stdlib.h>
#else
#include <GL/glut.h>
#endif
#include "Vector3D.hpp"
#include "PrismMesh.hpp"
#include "Drone.hpp"
#include "DroneAI.hpp"
#include "Camera.hpp"
#include "Building.hpp"
#include "Street.hpp"
#include "Missile.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "Stb_image.cpp"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

int windowWidth = 1200;
int windowHeight = 600;

const int groundLength = 36;          // Default ground length
const int groundWidth = 36;           // Default ground height
const int worldSize = 300;            // Size of the world, used for the skybox
const double tpViewportRatio = 0.7; // Window Width Ratio for the third-person viewport

GLuint program = 0;
GLint texMapLocation;
GLint texModeLocation;
GLint spotlightModeLocation;

//Boundaries of the city viewport
static GLdouble cityViewportX;
static GLdouble cityViewportY;
static GLdouble cityViewportWidth;
static GLdouble cityViewportHeight;

//Boundaries of the first-person viewport
static GLdouble fpViewportX;
static GLdouble fpViewportY;
static GLdouble fpViewportWidth;
static GLdouble fpViewportHeight;

//Initialize a drone object
Vector3D playerSpawnPoint(0.0, 4.0, -3.0);
Vector3D enemySpawnPoint(0.0, 4.0, 4.0);
// Creates a drone with a scaleFactor of 1.0;
// with 6 arms and 2 propeller blades per arm;
// positioned at specified spawnPoint
Drone dronePlayer(0.02, 6, 2, playerSpawnPoint, 20);
DroneAI droneEnemy(0.02, 6, 2, enemySpawnPoint, 20);


static Camera tpCamera;          //third-person camera for the drone
static Camera fpCamera;          //first-person camera for the drone
static int currentButton;      //Current mouse button being pressed
static vector<Building*> buildings;                //array of buildings
static vector<Street*> streets;                //array of streets
static vector<int> buildingTextures;
static vector<int> roofTextures;
static string CityMetaDataFile = "CityMetaData3.txt";
static Polygon ground;
static PrismMesh skybox;
static Polygon dpMapIcon;
static Polygon deMapIcon;

//Textures
static std::vector<char const*> texFiles; //array of texture filenames

// Light properties
static GLfloat light_position0[] = { worldSize*0.5, worldSize*0.1, -worldSize*0.1, 1.0F };
static GLfloat light_diffuse[] = { 1.0, 0.95, 0.7, 1.0 };
static GLfloat light_specular[] = { 1.0, 0.9, 0.7, 1.0 };
static GLfloat light_ambient[] = { 0.95F, 0.8F, 0.6F, 1.0F };

//Material properties for the ground blocks
static GLfloat block_mat_ambient[] = { 0.3F, 0.2F, 0.2F, 1.0F };
static GLfloat block_mat_specular[] = { 0.4F, 0.3F, 0.3F, 1.0F };
static GLfloat block_mat_diffuse[] = { 0.9F, 0.8F, 0.8F, 1.0F };
static GLfloat block_mat_shininess[] = { 0.8F };

// Ground material properties
static GLfloat ground_ambient[] = { 0.6F, 0.5F, 0.5F, 1.0F };
static GLfloat ground_specular[] = { 0.1F, 0.1F, 0.1F, 1.0F };
static GLfloat ground_diffuse[] = { 0.3F, 0.3F, 0.3F, 1.0F };
static GLfloat ground_shininess[] = { 0.1F };

// Street material properties
static GLfloat street_ambient[] = { 0.6F, 0.5F, 0.5F, 1.0F };
static GLfloat street_specular[] = { 0.15F, 0.1F, 0.1F, 1.0F };
static GLfloat street_diffuse[] = { 0.4F, 0.3F, 0.3F, 1.0F };
static GLfloat street_shininess[] = { 0.1F };

// Street Map material properties
static GLfloat streetMap_ambient[] = { 0.4F, 0.4F, 0.4F, 1.0F };
static GLfloat streetMap_specular[] = { 0.15F, 0.1F, 0.1F, 1.0F };
static GLfloat streetMap_diffuse[] = { 0.3F, 0.3F, 0.3F, 1.0F };
static GLfloat streetMap_shininess[] = { 0.1F };

// Drone Player Map Icon material properties
static GLfloat dpMap_ambient[] = { 0.05F, 0.05F, 1.0F, 1.0F };
static GLfloat dpMap_specular[] = { 1.0F, 1.0F, 1.0F, 1.0F };
static GLfloat dpMap_diffuse[] = { 1.0F, 1.0F, 1.0F, 1.0F };
static GLfloat dpMap_shininess[] = { 0.8F };

// Drone Enemy Map Icon material properties
static GLfloat deMap_ambient[] = { 1.0F, 0.05F, 0.05F, 1.0F };
static GLfloat deMap_specular[] = { 1.0F, 1.0F, 1.0F, 1.0F };
static GLfloat deMap_diffuse[] = { 1.0F, 1.0F, 1.0F, 1.0F };
static GLfloat deMap_shininess[] = { 0.8F };

// Skybox texture grid s&t coordinates (5x4 grid lines)
// Example: st12 represents the intersection of vertical line 1 and horizontal line 2
// in a zero-indexed grid
static Vector2D st10 = Vector2D(0.25, 0);
static Vector2D st20 = Vector2D(0.5, 0);

static Vector2D st01 = Vector2D(0, 0.334);
static Vector2D st11 = Vector2D(0.25, 0.334);
static Vector2D st21 = Vector2D(0.5, 0.334);
static Vector2D st31 = Vector2D(0.75, 0.334);
static Vector2D st41 = Vector2D(1, 0.334);

static Vector2D st02 = Vector2D(0, 0.666);
static Vector2D st12 = Vector2D(0.25, 0.666);
static Vector2D st22 = Vector2D(0.5, 0.666);
static Vector2D st32 = Vector2D(0.75, 0.666);
static Vector2D st42 = Vector2D(1, 0.666);

static Vector2D st13 = Vector2D(0.25, 1);
static Vector2D st23 = Vector2D(0.5, 1);

// Skybox texture coordinates
static vector<Vector2D> stSkyTopCoords({st12, st13, st23, st22});
static vector<Vector2D> stSkyBottomCoords({st21, st20, st10, st11});
static vector< vector<Vector2D> > stSkySideCoords({{st11, st12, st22, st21},
                                                {st01, st02, st12, st11},
                                                {st31, st32, st42, st41},
                                                {st21, st22, st32, st31}});


// Prototypes for functions in this module
void initOpenGL(int w, int h);
void display(void);
void drawAssets();
void drawMap();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
void functionKeysUp(int key, int x, int y);
Vector3D ScreenToWorld(int x, int y);
void toggleDronePropellers(int val);
void mouseButtonHandler(int button, int state, int xMouse, int yMouse);
void mouseMotionHandler(int xMouse, int yMouse);
void printControls();
void loadCity(string filename);
void loadTextures(vector<char const*> texFiles);
void handleCollisions();
bool checkOutOfBounds(Vector3D point);

static char* readShaderSource(const char* shaderFile);
static void checkError(GLint status, const char *msg);
static void initShader(const GLchar* vShaderFile, const GLchar* fShaderFile);

int main(int argc, char **argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(200, 30);
    glutCreateWindow("Assignment 3 - City Battle Drones");

    // Initialize GL
    initOpenGL(windowWidth, windowHeight);

    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(functionKeys);
    glutSpecialUpFunc(functionKeysUp);
    glutKeyboardUpFunc(keyboardUp);
    glutMouseFunc(mouseButtonHandler);
    glutMotionFunc(mouseMotionHandler);
    glutIdleFunc(display);

    initShader("Shaders/vShader.glsl", "Shaders/fShader.glsl");
    
    // Start event loop, never returns
    glutMainLoop();

    return 0;
}


// Set up OpenGL. For viewport and projection setup see reshape(). */
void initOpenGL(int w, int h)
{
    // Set up and enable lighting
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Other OpenGL setup
    glEnable(GL_DEPTH_TEST);   // Remove hidded surfaces
    glShadeModel(GL_SMOOTH);   // Use smooth shading, makes boundaries between polygons harder to see 
    glClearColor(0.6F, 0.6F, 0.6F, 0.0F);  // Color and depth for glClear
    glClearDepth(1.0f);
    glEnable(GL_NORMALIZE);    // Renormalize normal vectors 
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   // Nicer perspective
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    //Load textures
    texFiles.push_back("Textures/cityGround2.bmp");      //2000
    
    texFiles.push_back("Textures/steelGradient.bmp");    //2001
    
    texFiles.push_back("Textures/skybox1.bmp");          //2002
    
    texFiles.push_back("Textures/floorTex1.bmp");        //2003
    texFiles.push_back("Textures/floorTex2.bmp");        //2004
    texFiles.push_back("Textures/floorTex3.bmp");        //2005
    texFiles.push_back("Textures/floorTex4.bmp");        //2006
    texFiles.push_back("Textures/floorTex5.bmp");        //2007
    texFiles.push_back("Textures/floorTex6.bmp");        //2008

    texFiles.push_back("Textures/roofTex1.bmp");         //2009
    texFiles.push_back("Textures/roofTex2.bmp");         //2010
    texFiles.push_back("Textures/roofTex3.bmp");         //2011
    texFiles.push_back("Textures/redMetal2.bmp");        //2012
    
    texFiles.push_back("Textures/missileTex1.bmp");      //2013
    texFiles.push_back("Textures/smoke1.png");           //2014
    texFiles.push_back("Textures/blast.bmp");            //2015
    
    texFiles.push_back("Textures/street.bmp");           //2016
    
    loadTextures(texFiles);
    
    skybox.changeScaleFactors(Vector3D(worldSize, worldSize, worldSize));
    
    ground.verts.push_back(Vector3D(-groundLength/2, -0.1, -groundWidth/2));
    ground.verts.push_back(Vector3D(-groundLength/2, -0.1, groundWidth/2));
    ground.verts.push_back(Vector3D(groundLength/2, -0.1, groundWidth/2));
    ground.verts.push_back(Vector3D(groundLength/2, -0.1, -groundWidth/2));
    ground.calculateNormal();
    
    fpCamera.setElevation(5);
    
    vector<Vector3D> verts({Vector3D(-0.5,0,-0.75),Vector3D(0,0,0.75),Vector3D(0.5,0,-0.75)});
    dpMapIcon.verts = verts;
    deMapIcon.verts = verts;
    dpMapIcon.calculateNormal();
    deMapIcon.calculateNormal();
    
    loadCity(CityMetaDataFile);
}


// Callback, called whenever GLUT determines that the window should be redisplayed
// or glutPostRedisplay() has been called.
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    for(int j = 0; j < dronePlayer.missiles.size(); j++)
    {
        dronePlayer.missiles[j].setTargetPos(droneEnemy.getPosition());
    }
    
    for(int j = 0; j < droneEnemy.missiles.size(); j++)
    {
        droneEnemy.missiles[j].setTargetPos(dronePlayer.getPosition());
    }
    
    droneEnemy.makeDecisions(dronePlayer.getPosition());
    
    handleCollisions();
    
    dronePlayer.updateDrone();
    droneEnemy.updateDrone();
    
    Vector3D newFocus = dronePlayer.getPosition();
    
    tpCamera.changeFocus(newFocus);
    tpCamera.setAzimuth(180 + dronePlayer.getRotationY());
    tpCamera.update();
    
    fpCamera.changeFocus(Vector3D(newFocus.x, newFocus.y - 0.02, newFocus.z));
    fpCamera.setAzimuth(dronePlayer.getRotationY());
    fpCamera.update();
    
    glViewport(0, 0, (GLsizei)windowWidth*tpViewportRatio, (GLsizei)windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLdouble)(windowWidth*tpViewportRatio / windowHeight), 0.01, 400.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(tpCamera.position.x, tpCamera.position.y, tpCamera.position.z, tpCamera.focus.x, tpCamera.focus.y, tpCamera.focus.z, 0, 1, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    
    glUniform1i(texMapLocation, 0);
    glUniform1i(texModeLocation, 0);
    drawAssets();
    
    glViewport(fpViewportX, fpViewportY, (GLsizei)fpViewportWidth, (GLsizei)fpViewportHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLdouble)(fpViewportWidth/fpViewportHeight), 0.005, 400.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    float invertedY = fpCamera.focus.y - (fpCamera.position.y - fpCamera.focus.y);
    gluLookAt(fpCamera.focus.x, fpCamera.focus.y, fpCamera.focus.z, fpCamera.position.x, invertedY, fpCamera.position.z, 0, 1, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    
    glUniform1i(spotlightModeLocation, 1);
    drawAssets();
    glUniform1i(spotlightModeLocation, 0);
    
    glViewport(cityViewportX, cityViewportY, (GLsizei)cityViewportWidth, (GLsizei)cityViewportHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(20.0, (GLdouble)(cityViewportWidth/cityViewportWidth), 0.01, 400.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.1, 101, 0, 0, 0, 0, 0, 1, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    
    glUniform1i(texModeLocation, 2);
    drawMap();

    
    glutSwapBuffers();   // Double buffering, swap buffers
}

void drawAssets()
{
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    vector<Vector2D> stCoordinates({Vector2D(0,0), Vector2D(0,1), Vector2D(1,1), Vector2D(1,0)});
    
    
    // Set material properties of the streets
    glMaterialfv(GL_FRONT, GL_AMBIENT, street_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, street_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, street_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, street_shininess);
    
    for(int i = 0; i < streets.size(); i++)
    {
        glPushMatrix();
        glTranslatef(0, 0.001*i, 0);
        streets[i]->draw(2016);
        glPopMatrix();
    }
    
    //skybox
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glDisable(GL_CULL_FACE);
    glUniform1i(texModeLocation, 1);
    glPushMatrix();
    glTranslatef(0, 0, 0);
    skybox.draw(2002, stSkySideCoords, stSkyTopCoords, stSkyBottomCoords);
    glPopMatrix();
    glUniform1i(texModeLocation, 0);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    // Set ground block material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, block_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, block_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, block_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, block_mat_shininess);
    
    for(int i = 0; i < buildings.size(); i++)
    {
        buildings[i]->draw(2003 + buildingTextures[i], stCoordinates, true, 2009 + roofTextures[i]);
    }
    
    // Set material properties of the ground
    glMaterialfv(GL_FRONT, GL_AMBIENT, ground_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ground_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ground_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, ground_shininess);
    
    //Draw ground quad
    ground.draw(2000, stCoordinates, false);
    
    droneEnemy.draw();
    dronePlayer.draw();
}

void drawMap()
{
    
    // Set material properties of the streets
    glMaterialfv(GL_FRONT, GL_AMBIENT, streetMap_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, streetMap_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, streetMap_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, streetMap_shininess);
    
    for(int i = 0; i < streets.size(); i++)
    {
        streets[i]->draw();
    }
    
    // Set ground block material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, block_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, block_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, block_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, block_mat_shininess);
    
    for(int i = 0; i < buildings.size(); i++)
    {
        buildings[i]->draw();
    }
    
    // Set material properties of the ground
    glMaterialfv(GL_FRONT, GL_AMBIENT, ground_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ground_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ground_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, ground_shininess);
    
    //Draw ground quad
    ground.draw();
    
    // Set material properties of the the drone player's map icon
    glMaterialfv(GL_FRONT, GL_AMBIENT, dpMap_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, dpMap_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, dpMap_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, dpMap_shininess);
    
    glPushMatrix();
    glTranslatef(dronePlayer.getPosition().x, 10, dronePlayer.getPosition().z);
    glRotatef(dronePlayer.getRotationY(), 0, 1, 0);
    dpMapIcon.draw();
    glPopMatrix();
    
    // Set material properties of the the drone player's map icon
    glMaterialfv(GL_FRONT, GL_AMBIENT, deMap_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, deMap_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, deMap_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, deMap_shininess);
    
    glPushMatrix();
    glTranslatef(droneEnemy.getPosition().x, 10, droneEnemy.getPosition().z);
    glRotatef(droneEnemy.getRotationY(), 0, 1, 0);
    deMapIcon.draw();
    glPopMatrix();
}


// Callback, called at initialization and whenever user resizes the window.
void reshape(int w, int h)
{
    windowWidth = w;
    windowHeight = h;
    float padding = 5;
    
    //Boundaries of the city viewport
    cityViewportX  = windowWidth*tpViewportRatio + padding;
    cityViewportY =  0;
    cityViewportWidth = windowWidth-cityViewportX;
    cityViewportHeight = windowHeight*0.5 - padding/2.0;
    
    
    //Boundaries of the first-person viewport
    fpViewportX  = windowWidth*tpViewportRatio + padding;
    fpViewportY =  cityViewportHeight + padding;
    fpViewportWidth = windowWidth-fpViewportX;
    fpViewportHeight = windowHeight*0.5 - padding/2.0;
}

// Callback, handles input from the keyboard, non-arrow keys
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
            exit(0);
            break;
        case 'f':
            glutFullScreen();
            break;
        case 'w':
            dronePlayer.setAction(2, true);
            break;
        case 's':
            dronePlayer.setAction(3, true);
            break;
        case 'a':
            dronePlayer.setAction(7, true);
            break;
        case 'd':
            dronePlayer.setAction(6, true);
            break;
        case 'h':
            printControls();
            break;
        case 'y':
            droneEnemy.active = !droneEnemy.active;
            break;
        case ' ':
            dronePlayer.launchMissile();
            break;
        case 'c':
            droneEnemy.launchMissile();
            break;
        case 'n':
        {
            tpCamera.setZoomChangeRate(0.01);
            tpCamera.controlActions[2] = true;
            break;
        }
        case 'm':
        {
            tpCamera.setZoomChangeRate(-0.01);
            tpCamera.controlActions[2] = true;
            break;
        }
        case 'k':
        {
            fpCamera.setElevationChangeRate(1);
            fpCamera.controlActions[1] = true;
            break;
        }
        case 'i':
        {
            fpCamera.setElevationChangeRate(-1);
            fpCamera.controlActions[1] = true;
            break;
        }
    }
    glutPostRedisplay();   // Trigger a window redisplay
}

void keyboardUp(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'k':
            fpCamera.controlActions[1] = false;
            break;
        case 'i':
            fpCamera.controlActions[1] = false;
            break;
        case 'w':
            dronePlayer.setAction(2, false);
            break;
        case 's':
            dronePlayer.setAction(3, false);
            break;
        case 'a':
            dronePlayer.setAction(7, false);
            break;
        case 'd':
            dronePlayer.setAction(6, false);
            break;
        case 'n':
            tpCamera.controlActions[2] = false;
            break;
        case 'm':
            tpCamera.controlActions[2] = false;
            break;
    }
    glutPostRedisplay();
}

// Callback, handles input from the keyboard, function and arrow keys
void functionKeys(int key, int x, int y)
{
    if (key == GLUT_KEY_DOWN)
    {
        dronePlayer.setAction(1, true);
    }
    else if (key == GLUT_KEY_UP){
        dronePlayer.setAction(0, true);
    }
    else if (key == GLUT_KEY_LEFT){
        dronePlayer.setAction(4, true);
    }
    else if (key == GLUT_KEY_RIGHT){
        dronePlayer.setAction(5, true);
    }
    glutPostRedisplay();   // Trigger a window redisplay
}

void functionKeysUp(int key, int x, int y)
{
    if (key == GLUT_KEY_DOWN)
    {
        dronePlayer.setAction(1, false);
    }
    else if (key == GLUT_KEY_UP){
        dronePlayer.setAction(0, false);
    }
    else if (key == GLUT_KEY_LEFT){
        dronePlayer.setAction(4, false);
    }
    else if (key == GLUT_KEY_RIGHT){
        dronePlayer.setAction(5, false);
    }
    glutPostRedisplay();   // Trigger a window redisplay
}


Vector3D ScreenToWorld(int x, int y)
{
    return Vector3D(0, 0, 0);
}

void mouseButtonHandler(int button, int state, int xMouse, int yMouse)
{
    currentButton = button;
    
    if (button == GLUT_LEFT_BUTTON)
    {
        switch (state)
        {
            case GLUT_DOWN:
            {
                if(xMouse < windowWidth && xMouse > 0 && yMouse <= windowHeight && yMouse > 0)
                {
                    tpCamera.clickX = xMouse;
                    tpCamera.clickY = yMouse;
                    tpCamera.clickAndDrag = true;
                }
                break;
            }
            case GLUT_UP:
                tpCamera.clickAndDrag = false;
        }
    }
    
    /* Schedule a call to display() */
    glutPostRedisplay();
}

void mouseMotionHandler(int xMouse, int yMouse)
{
    if (currentButton == GLUT_LEFT_BUTTON)
    {
        if(tpCamera.clickAndDrag)
        {
            tpCamera.move(xMouse, yMouse);
        }
    }
    
    /* Schedule a call to display() */
    glutPostRedisplay();
}

// Prints the dronePlayer controls to the standard output
void printControls()
{
    string controls = "\nHere are the controls for the dronePlayer:\n\n";
    controls += "Move drone player forward:         w key\n";
    controls += "Move drone player backward:        s key\n";
    controls += "Move drone player left:            a key\n";
    controls += "Move drone player right:           d key\n\n";
    controls += "Rotate drone player left:          left arrow key\n";
    controls += "Rotate drone player right:         right arrow key\n";
    controls += "Move drone player up:              up arrow key\n";
    controls += "Move drone player down:            down arrow key\n\n";
    controls += "Change 3rd-person camera angle:    click and drag up/down on first-person view port\n";
    controls += "Zoom 3rd-person camera in:         n key\n";
    controls += "Zoom 3rd-person camera out:        m key\n\n";
    controls += "Activate/Deactivate Enemy AI:      y key\n\n";
    controls += "Launch missile:                    spacebar\n\n";
    controls += "Tilt 1st-person camera down:       k key\n";
    controls += "Tilt 1st-person camera up:         i key\n\n";
    controls += "Print controls:                    h key\n";
    controls += "Fullscreen:                        f key\n";
    controls += "Quit the program:                  ESC\n";
    std::cout << controls;
};


void loadCity(string filename)
{
    string line;
    string metaData;
    vector<Building*> loadedBuildings;
    vector<Street*> loadedStreets;
    ifstream myfile (filename);
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            if(!line.compare("---------"))
            {
                if(loadedBuildings.size() > 0)
                {
                    loadedBuildings.at(loadedBuildings.size() - 1)->processMetaData(metaData);
                }
                Building* bd = new Building();
                loadedBuildings.push_back(bd);
                metaData = "";
            }
            else if(!line.compare("END_BUILDING_LIST"))
            {
                if(loadedBuildings.size() > 0)
                {
                    loadedBuildings.at(loadedBuildings.size() - 1)->processMetaData(metaData);
                }
            }
            else if(!line.compare("+++++"))
            {
                if(loadedStreets.size() > 0)
                {
                    loadedStreets.at(loadedStreets.size() - 1)->processMetaData(metaData);
                }
                Street* bd = new Street();
                loadedStreets.push_back(bd);
                metaData = "";
            }
            else if(!line.compare("END_STREET_LIST"))
            {
                if(loadedStreets.size() > 0)
                {
                    loadedStreets.at(loadedStreets.size() - 1)->processMetaData(metaData);
                }
            }
            else
            {
                metaData += line + "\n";
            }
        }
        
        for (auto& bld : loadedBuildings)
        {
            buildings.push_back(bld);
            int randIndex = rand() % 6;
            buildingTextures.push_back(randIndex);
            randIndex = rand() % 3;
            roofTextures.push_back(randIndex);
        }
        for (auto& strt : loadedStreets)
        {
            streets.push_back(strt);
        }
        
        
        glutPostRedisplay();
        myfile.close();
    }
    else cout << "Unable to open file";
}

void loadTextures(vector<char const*> texFiles)
{
    for(int i = 0; i < texFiles.size(); i++)
    {
        int x,y,n;
        char const* filename = texFiles[i];
        unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
    
        glBindTexture(GL_TEXTURE_2D, 2000 + i);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        
        // ... process data if not NULL ...
        // ... x = width, y = height, n = # 8-bit components per pixel ...
        // ... replace '0' with '1'..'4' to force that many components per pixel
        // ... but 'n' will always be the number that it would have been if you said 0
        if (n == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else if (n == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        stbi_image_free(data);
    }
}

void handleCollisions()
{
    // Check for drone/missile collisions with the buildings
    for(int i = 0; i < buildings.size(); i++)
    {
        // Check if the dronePlayer collided with a building
        bool collideBld = buildings[i]->checkObjectCollision(dronePlayer.getPosition());
        if(collideBld && !dronePlayer.isDestroyed)
        {
            dronePlayer.destroy();
        }
        
        // Check if the droneEnemy collided with a building
        collideBld = buildings[i]->checkObjectCollision(droneEnemy.getPosition());
        if(collideBld && !droneEnemy.isDestroyed)
        {
            droneEnemy.destroy();
        }
        
        // Check if one of the dronePlayer's missiles collided with a building
        for(int j = 0; j < dronePlayer.missiles.size(); j++)
        {
            collideBld = buildings[i]->checkObjectCollision(dronePlayer.missiles[j].getPosition());
            if(collideBld && !dronePlayer.missiles[j].isDestroyed)
            {
                dronePlayer.missiles[j].blowUp();
            }
        }
        
        // Check if one of the droneEnemy's missiles collided with a building
        for(int j = 0; j < droneEnemy.missiles.size(); j++)
        {
            collideBld = buildings[i]->checkObjectCollision(droneEnemy.missiles[j].getPosition());
            if(collideBld && !droneEnemy.missiles[j].isDestroyed)
            {
                droneEnemy.missiles[j].blowUp();
            }
        }
        
    }
    
    // Check if one of the dronePlayer's missiles collided with the droneEnemy or world boundary
    for(int j = 0; j < dronePlayer.missiles.size(); j++)
    {
        if(!dronePlayer.missiles[j].isDestroyed && !droneEnemy.isDestroyed)
        {
            if(checkOutOfBounds(dronePlayer.missiles[j].getPosition()))
            {
                dronePlayer.missiles[j].blowUp();
                continue;
            }
            if(!droneEnemy.isDestroyed)
            {
                Vector3D pmPos = dronePlayer.missiles[j].getPosition();
                Vector3D ePos = droneEnemy.getPosition();
                float distanceToEnemy = Vector3D::subtract(pmPos, ePos).getLength();
                if(distanceToEnemy < 0.1)
                {
                    droneEnemy.destroy();
                    dronePlayer.missiles[j].blowUp();
                }
            }
        }
    }
    
    // Check if one of the droneEnemy's missiles collided with the dronePlayer or world boundary
    for(int j = 0; j < droneEnemy.missiles.size(); j++)
    {
        if(!droneEnemy.missiles[j].isDestroyed)
        {
            if(checkOutOfBounds(droneEnemy.missiles[j].getPosition()))
            {
                droneEnemy.missiles[j].blowUp();
                continue;
            }
            if(!dronePlayer.isDestroyed)
            {
                Vector3D emPos = droneEnemy.missiles[j].getPosition();
                Vector3D pPos = dronePlayer.getPosition();
                float distanceToPlayer = Vector3D::subtract(emPos, pPos).getLength();
                if(distanceToPlayer < 0.1)
                {
                    dronePlayer.destroy();
                    droneEnemy.missiles[j].blowUp();
                }
            }
        }
    }
    
    // Check if the dronePlayer and droneEnemy collide with each other
    Vector3D ePos = droneEnemy.getPosition();
    Vector3D pPos = dronePlayer.getPosition();
    float distanceBetweenDrones = Vector3D::subtract(ePos, pPos).getLength();
    if(distanceBetweenDrones < 0.1 && !dronePlayer.isDestroyed)
    {
        droneEnemy.destroy();
        dronePlayer.destroy();
    }
    
    // Check if the dronePlayer and droneEnemy collide with the ground
    if(dronePlayer.getPosition().y < 0)
    {
        dronePlayer.destroy();
    }
    if(droneEnemy.getPosition().y < 0)
    {
        droneEnemy.destroy();
    }
}

// Returns true if the given point is out of bounds (not within the ground plane area or above ground)
bool checkOutOfBounds(Vector3D point)
{
    float halfLength = groundLength/2;
    float halfWidth = groundWidth/2;
    if(point.x > halfLength || point.x < -halfLength ||
       point.z > halfWidth || point.z < -halfWidth || point.y < 0)
    {
        return true;
    }
    return false;
}


/* shader reader */
/* creates null terminated string from file */

static char* readShaderSource(const char* shaderFile)
{
    struct stat statBuf;
    FILE* fp = fopen(shaderFile, "r");
    char* buf;
    
    stat(shaderFile, &statBuf);
    buf = (char*) malloc((statBuf.st_size + 1) * sizeof(char));
    fread(buf, 1, statBuf.st_size, fp);
    buf[statBuf.st_size] = '\0';
    fclose(fp);
    return buf;
}

/* error printing function */

static void checkError(GLint status, const char *msg)
{
    if (!status)
    {
        printf("%s\n", msg);
        exit(EXIT_FAILURE);
    }
}

/* GLSL initialization */

static void initShader(const GLchar* vShaderFile, const GLchar* fShaderFile)
{
    GLint status = glGetError()==GL_NO_ERROR;
    GLchar *vSource, *fSource;
    GLuint vShader, fShader;
    GLuint texMapLocation;
    
    /* read shader files */
    
    vSource = readShaderSource(vShaderFile);
    checkError(status, "Failed to read vertex shader");
    
    fSource = readShaderSource(fShaderFile);
    checkError(status, "Failed to read fragment shader");
    
    /* create program and shader objects */
    
    vShader = glCreateShader(GL_VERTEX_SHADER);
    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    program = glCreateProgram();
    
    /* attach shaders to the program object */
    
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    
    /* read shaders */
    
    glShaderSource(vShader, 1, (const GLchar**) &vSource, NULL);
    glShaderSource(fShader, 1, (const GLchar**) &fSource, NULL);
    
    /* compile shaders */
    
    glCompileShader(vShader);
    glCompileShader(fShader);
    
    /* error check */
    
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &status);
    checkError(status, "Failed to compile the vertex shader.");
    
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &status);
    checkError(status, "Failed to compile the fragment shader.");
    
    /* link */
    
    glLinkProgram(program);
    glGetShaderiv(program, GL_LINK_STATUS, &status);
    //checkError(status, "Failed to link the shader program object.");
    
    /* use program object */
    
    glUseProgram(program);
    
    /* set up uniform parameter */
    
    texMapLocation = glGetUniformLocation(program, "texMap");
    texModeLocation = glGetUniformLocation(program, "texMode");
    spotlightModeLocation = glGetUniformLocation(program, "spotlightMode");
}



