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
#include <iostream>
#include <vector>
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

const int vWidth = 850;     // Viewport width in pixels
const int vHeight = 500;    // Viewport height in pixels
int windowWidth;
int windowHeight;

//Initialize a drone object
Vector3D spawnPoint(0.0, 3.0, 5.0);
// Creates a drone with a scaleFactor of 1.0;
// with 5 arms and 2 propeller blades per arm;
// positioned at spawnPoint
Drone drone(1.0, 6, 2, spawnPoint);
PrismMesh prism;
static Camera camera;          //Camera for the scene
static int currentButton;      //Current mouse button being pressed

//Textures
static std::vector<string> texFiles; //array of texture filenames
static std::vector<RGBpixmap*> pm;  //array of pointers to pixelmaps for each texture file

// Light properties
static GLfloat light_position0[] = { -6.0F, 12.0F, 0.0F, 1.0F };
static GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_ambient[] = { 0.2F, 0.2F, 0.2F, 1.0F };

// Material properties for the ground blocks
static GLfloat block_mat_ambient[] = { 0.3F, 0.2F, 0.05F, 1.0F };
static GLfloat block_mat_specular[] = { 0.4F, 0.2F, 0.4F, 1.0F };
static GLfloat block_mat_diffuse[] = { 0.6F, 0.9F, 0.9F, 0.0F };
static GLfloat block_mat_shininess[] = { 0.8F };

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

int main(int argc, char **argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(vWidth, vHeight);
    glutInitWindowPosition(200, 30);
    glutCreateWindow("Assignment 1 - Drone Interaction");

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
    texFiles.push_back(string("plank01.bmp"));  //2000
    texFiles.push_back(string("steel.bmp"));    //2001
    texFiles.push_back(string("jae2.bmp"));//2002
    texFiles.push_back(string("redMetal2.bmp"));//2003
    pm = TextureUtils::loadTextures(texFiles);
}


// Callback, called whenever GLUT determines that the window should be redisplayed
// or glutPostRedisplay() has been called.
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLdouble)windowWidth / windowHeight, 0.2, 40.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    camera.changeFocus(drone.getPosition());
    camera.setAzimuth(180 + drone.getRotationY());
    camera.update();
    gluLookAt(camera.position.x, camera.position.y, camera.position.z, camera.focus.x, camera.focus.y, camera.focus.z, 0, 1, 0);
    //glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    
    // Draw the full drone and applies its transformations based on its current
    // rotation, position, etc.
    drone.updateDrone();
    drone.draw();
    
    // Set ground block material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, block_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, block_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, block_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, block_mat_shininess);
    
    // Draw ground Block1
    glPushMatrix();
    glTranslatef(1.0, -2.5, 1.0);
    glRotatef(45.0, 0.0, 1.0, 0.0);
    glScalef(3.0, 5.0, 1.0);
    glutSolidCube(1.0);
    glPopMatrix();
    
    // Draw ground Block2
    glPushMatrix();
    glTranslatef(-4.0, -1.0, 4.0);
    glRotatef(30.0, 0.0, 1.0, 0.0);
    glScalef(2.0, 2.0, 3.0);
    glutSolidCube(1.0);
    glPopMatrix();
    
    prism.draw();
    
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





