/*******************************************************************
           City Battle Drones
********************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <stdlib.h>
#else
#include <GL/glut.h>
#endif
#include "Vector3D.hpp"
#include "PrismMesh.hpp"
#include "TextureUtils.hpp"
#include "Drone.hpp"
#include "Camera.hpp"
#include "Building.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

const int vWidth = 850;     // Viewport width in pixels
const int vHeight = 500;    // Viewport height in pixels
int windowWidth;
int windowHeight;

const int groundLength = 36;         // Default ground length 100 meters/unit
const int groundWidth = 36;          // Default ground height 100 meters/unit
const int worldSize = 60;            // Size of the world, used for the skybox

//Initialize a drone object
Vector3D spawnPoint(0.0, 3.0, 5.0);
// Creates a drone with a scaleFactor of 1.0;
// with 5 arms and 2 propeller blades per arm;
// positioned at spawnPoint
Drone drone(0.002, 6, 2, spawnPoint);
PrismMesh prism;
static Camera camera;          //Camera for the scene
static int currentButton;      //Current mouse button being pressed
static vector<Building*> buildings;                //array of buildings
static string CityMetaDataFile = "CityMetaData.txt";
static Polygon ground;
static PrismMesh skybox;

//Textures
static std::vector<string> texFiles; //array of texture filenames
static std::vector<RGBpixmap*> pm;  //array of pointers to pixelmaps for each texture file

// Light properties
static GLfloat light_position0[] = { worldSize*0.5, worldSize*0.1, -worldSize*0.1, 1.0F };
static GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_ambient[] = { 0.9F, 0.9F, 0.9F, 1.0F };

// Material properties for the ground blocks
static GLfloat block_mat_ambient[] = { 0.3F, 0.2F, 0.05F, 1.0F };
static GLfloat block_mat_specular[] = { 0.4F, 0.2F, 0.4F, 1.0F };
static GLfloat block_mat_diffuse[] = { 0.6F, 0.9F, 0.9F, 0.0F };
static GLfloat block_mat_shininess[] = { 0.8F };

// Ground material properties
static GLfloat ground_ambient[] = { 0.3F, 0.2F, 0.1F, 1.0F };
static GLfloat ground_specular[] = { 0.1F, 0.1F, 0.1F, 0.1F };
static GLfloat ground_diffuse[] = { 0.3F, 0.3F, 0.4F, 1.0F };
static GLfloat ground_shininess[] = { 0.1F };

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
static vector<Vector2D> stSkyTopCoords = {st12, st13, st23, st22};
static vector<Vector2D> stSkyBottomCoords = {st21, st20, st10, st11};
static vector<vector<Vector2D>> stSkySideCoords = {{st11, st12, st22, st21},
                                                   {st01, st02, st12, st11},
                                                   {st31, st32, st42, st41},
                                                   {st21, st22, st32, st31}};


// Prototypes for functions in this module
void initOpenGL(int w, int h);
void display(void);
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

int main(int argc, char **argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(vWidth, vHeight);
    glutInitWindowPosition(200, 30);
    glutCreateWindow("Assignment 3 - City Battle Drones");

    // Initialize GL
    initOpenGL(vWidth, vHeight);

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
    
    
    //Load textures
    texFiles.push_back(string("plank01.bmp"));      //2000
    texFiles.push_back(string("steel.bmp"));        //2001
    texFiles.push_back(string("jae2.bmp"));         //2002
    texFiles.push_back(string("redMetal2.bmp"));    //2003
    texFiles.push_back(string("floor1.bmp"));       //2004
    texFiles.push_back(string("skybox1.bmp"));      //2005
    texFiles.push_back(string("cityGround1.bmp"));  //2006
    
    pm = TextureUtils::loadTextures(texFiles);
    
    skybox.changeScaleFactors(Vector3D(worldSize, worldSize, worldSize));
    
    ground.verts.push_back(Vector3D(groundLength/2, 0.0, -groundWidth/2));
    ground.verts.push_back(Vector3D(groundLength/2, 0.0, groundWidth/2));
    ground.verts.push_back(Vector3D(-groundLength/2, 0.0, groundWidth/2));
    ground.verts.push_back(Vector3D(-groundLength/2, 0.0, -groundWidth/2));
    ground.calculateNormal();
}


// Callback, called whenever GLUT determines that the window should be redisplayed
// or glutPostRedisplay() has been called.
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    vector<Vector2D> stCoordinates = {Vector2D(0,0), Vector2D(0,1), Vector2D(1,1), Vector2D(1,0)};
    
    glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLdouble)windowWidth / windowHeight, 0.0005, 400.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    drone.updateDrone();
    camera.changeFocus(drone.getPosition());
    camera.setAzimuth(180 + drone.getRotationY());
    camera.update();
    
    gluLookAt(camera.position.x, camera.position.y, camera.position.z, camera.focus.x, camera.focus.y, camera.focus.z, 0, 1, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    
    drone.draw();
    
    // Set material properties of the ground
    glMaterialfv(GL_FRONT, GL_AMBIENT, ground_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ground_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ground_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, ground_shininess);
    
    //Draw ground quad
    ground.draw(2006, stCoordinates);
    
    // Set ground block material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, block_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, block_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, block_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, block_mat_shininess);
    
    
    for(int i = 0; i < buildings.size(); i++)
    {
        buildings[i]->draw(2004, stCoordinates, false, 0);
    }
    
    //skybox
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    skybox.draw(2005, stSkySideCoords, stSkyTopCoords, stSkyBottomCoords);


    
//    
//    // Draw ground Block1
//    glPushMatrix();
//    glTranslatef(1.0, -2.5, 1.0);
//    glRotatef(45.0, 0.0, 1.0, 0.0);
//    glScalef(3.0, 5.0, 1.0);
//    glutSolidCube(1.0);
//    glPopMatrix();
//    
//    // Draw ground Block2
//    glPushMatrix();
//    glTranslatef(-4.0, -1.0, 4.0);
//    glRotatef(30.0, 0.0, 1.0, 0.0);
//    glScalef(2.0, 2.0, 3.0);
//    glutSolidCube(1.0);
//    glPopMatrix();
//    
//    prism.draw();
    
    glutSwapBuffers();   // Double buffering, swap buffers
}

// Callback, called at initialization and whenever user resizes the window.
void reshape(int w, int h)
{
    windowWidth = w;
    windowHeight = h;

    // Set up the camera at position (0, 6, 22) looking at the origin, up along positive y axis
    //gluLookAt(0.0, 6.0, 22.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// Callback, handles input from the keyboard, non-arrow keys
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'p':
            drone.propsSpinning = !drone.propsSpinning;
            glutTimerFunc(5.0, toggleDronePropellers, 0);
            break;
        case 'w':
            drone.setAction(2, true);
            break;
        case 's':
            drone.setAction(3, true);
            break;
        case 'a':
            drone.setAction(7, true);
            break;
        case 'd':
            drone.setAction(6, true);
            break;
        case 'h':
            printControls();
            break;
        case 'i':
        {
            camera.setZoomChangeRate(0.01);
            camera.controlActions[2] = true;
            break;
        }
        case 'o':
        {
            camera.setZoomChangeRate(-0.01);
            camera.controlActions[2] = true;
            break;
        }
    }
    glutPostRedisplay();   // Trigger a window redisplay
}

void keyboardUp(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'w':
            drone.setAction(2, false);
            break;
        case 's':
            drone.setAction(3, false);
            break;
        case 'a':
            drone.setAction(7, false);
            break;
        case 'd':
            drone.setAction(6, false);
            break;
        case 'v':
        {
            loadCity(CityMetaDataFile);
            break;
        }
        case 'i':
            camera.controlActions[2] = false;
            break;
        case 'o':
            camera.controlActions[2] = false;
            break;
    }
    glutPostRedisplay();
}

// Callback, handles input from the keyboard, function and arrow keys
void functionKeys(int key, int x, int y)
{
    if (key == GLUT_KEY_DOWN)
    {
        //drone.changeElevation(-0.5);
        drone.setAction(1, true);
    }
    else if (key == GLUT_KEY_UP){
        //drone.changeElevation(0.5);
        drone.setAction(0, true);
    }
    else if (key == GLUT_KEY_LEFT){
        //drone.changeDirection(5.0);
        drone.setAction(4, true);
    }
    else if (key == GLUT_KEY_RIGHT){
        //drone.changeDirection(-5.0);
        drone.setAction(5, true);
    }
    glutPostRedisplay();   // Trigger a window redisplay
}

void functionKeysUp(int key, int x, int y)
{
    if (key == GLUT_KEY_DOWN)
    {
        //drone.changeElevation(-0.5);
        drone.setAction(1, false);
    }
    else if (key == GLUT_KEY_UP){
        //drone.changeElevation(0.5);
        drone.setAction(0, false);
    }
    else if (key == GLUT_KEY_LEFT){
        //drone.changeDirection(5.0);
        drone.setAction(4, false);
    }
    else if (key == GLUT_KEY_RIGHT){
        //drone.changeDirection(-5.0);
        drone.setAction(5, false);
    }
    glutPostRedisplay();   // Trigger a window redisplay
}


Vector3D ScreenToWorld(int x, int y)
{
    // you will need to finish this if you use the mouse
    return Vector3D(0, 0, 0);
}

// Spins the drone's propellers if the drone is on
void toggleDronePropellers(int val)
{
    // Recursive loop to keep the propellers spinning
    if(drone.propsSpinning){
        drone.spinPropellers();
        glutTimerFunc(4.0, toggleDronePropellers, 0);
    }
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
                if(xMouse < vWidth && xMouse > 0 && yMouse <= vHeight && yMouse > 0)
                {
                    camera.clickX = xMouse;
                    camera.clickY = yMouse;
                    camera.clickAndDrag = true;
                }
                break;
            }
            case GLUT_UP:
                camera.clickAndDrag = false;
        }
    }
    
    /* Schedule a call to display() */
    glutPostRedisplay();
}

void mouseMotionHandler(int xMouse, int yMouse)
{
    if (currentButton == GLUT_LEFT_BUTTON)
    {
        if(camera.clickAndDrag)
        {
            camera.move(xMouse, yMouse);
        }
    }
    
    /* Schedule a call to display() */
    glutPostRedisplay();
}

// Prints the drone controls to the standard output
void printControls()
{
    string controls = "\nHere are the controls for the drone:\n\n";
    controls += "Move Up:                   Up Arrow Key\n";
    controls += "Move Down:                 Down Arrow Key\n";
    controls += "Rotate ClockWise:          Right Arrow Key\n";
    controls += "Rotate CounterClockWise:   Left Arrow Key\n";
    controls += "Move Forward:              f Key\n";
    controls += "Move Backward:             b Key\n";
    controls += "Start Spinning Propellers: s Key\n";
    controls += "Stop Spinning Propellers:  s Key\n";
    controls += "Print Controls:            h Key\n";
    std::cout << controls;
};

void loadCity(string filename)
{
    string line;
    string metaData;
    vector<Building*> loadedBuildings;
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
            else if(!line.compare("END_LIST"))
            {
                if(loadedBuildings.size() > 0)
                {
                    loadedBuildings.at(loadedBuildings.size() - 1)->processMetaData(metaData);
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
        }
        glutPostRedisplay();
        myfile.close();
    }
    else cout << "Unable to open file";
}





