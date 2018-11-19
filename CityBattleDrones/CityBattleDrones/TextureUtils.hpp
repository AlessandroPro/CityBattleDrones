//
//  TextureUtils.hpp
//  CityBattleDrones
//
//  Created by Alessandro Profenna on 2018-11-17.
//  Copyright © 2018 Alessandro Profenna. All rights reserved.
//

#ifndef TextureUtils_hpp
#define TextureUtils_hpp

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <stdlib.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;
typedef unsigned short ushort;
typedef unsigned long  ulong;


typedef struct RGB
{
    byte r,g,b;
} RGB;

typedef struct RGBpixmap
{
    int nRows, nCols;
    RGB *pixel;
} RGBpixmap;

class TextureUtils
{
    static void fskip(FILE *fp, int num_bytes);
    static ushort getShort(FILE *fp);
    static ulong getLong(FILE *fp);
    
    public:
    
    static void readBMPFile(RGBpixmap *pm, string file);
    static void setTexture(RGBpixmap *p, GLuint textureID);
    static vector<RGBpixmap*> loadTextures(vector<string> texFiles);
};


/*
void myInit(void)
{
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    
    makeCheckerboard(&pix[0]);         // make texture for side 0 procedurally
    setTexture(&pix[0],2000);          // assign a unique identifier
    readBMPFile(&pix[1],"clover01.bmp");  // read texture for side 1 from image
    setTexture(&pix[1],2001);
    readBMPFile(&pix[2],"plank01.bmp");  // read texture for side 2 from image
    setTexture(&pix[2],2002);           // assign a unique identifier
    readBMPFile(&pix[3],"professor.bmp");  // read texture for side 3 from image
    setTexture(&pix[3],2003);
    makeCheckerboard(&pix[4]);         // make texture for side 4 procedurally
    setTexture(&pix[4],2004);           // assign a unique identifier
    readBMPFile(&pix[5],"floor1.bmp");  // read texture for side 5 from image
    setTexture(&pix[5],2005);
    
    glViewport(0, 0, 640, 480);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 640.0/480.0, 1.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(0.0, 0.0, -4.0); // move camera back from default position
    
}


void display1(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set up texture mapping assuming no lighting/shading
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glPushMatrix();
    
    glBindTexture(GL_TEXTURE_2D, 2000); // top face of cube
    glBegin(GL_QUADS);
    glTexCoord2f( 0.0, 0.0);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f( 0.0, 1.0);
    glVertex3f(-1.0f, 1.0f,  1.0f);
    glTexCoord2f( 1.0, 1.0);
    glVertex3f( 1.0f, 1.0f,  1.0f);
    glTexCoord2f( 1.0, 0.0);
    glVertex3f( 1.0f, 1.0f, -1.0f);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 2001); // right face of cube
    glBegin(GL_QUADS);
    glTexCoord2f( 0.0, 0.0);
    glVertex3f( 1.0f, 1.0f, -1.0f);
    glTexCoord2f( 0.0, 1.0);
    glVertex3f( 1.0f, 1.0f,  1.0f);
    glTexCoord2f( 1.0, 1.0);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f( 1.0, 0.0);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 2002); // left face of cube
    glBegin(GL_QUADS);
    glTexCoord2f( 0.0, 0.0);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f( 0.0, 1.0);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f( 1.0, 1.0);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f( 1.0, 0.0);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 2003); // bottom face of cube
    glBegin(GL_QUADS);
    glTexCoord2f( 0.0, 0.0);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f( 0.0, 1.0);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f( 1.0, 1.0);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f( 1.0, 0.0);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 2004); // back face of cube
    glBegin(GL_QUADS);
    glTexCoord2f( 0.0, 0.0);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f( 0.0, 1.0);
    glVertex3f( -1.0f, 1.0f,  -1.0f);
    glTexCoord2f( 1.0, 1.0);
    glVertex3f( 1.0f, 1.0f,  -1.0f);
    glTexCoord2f( 1.0, 0.0);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 2005); // front face of cube
    glBegin(GL_QUADS);
    glTexCoord2f( 0.0, 0.0);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f( 0.0, 5.0);
    glVertex3f(-1.0f, 1.0f,  1.0f);
    glTexCoord2f( 2.0, 5.0);
    glVertex3f( 1.0f, 1.0f,  1.0f);
    glTexCoord2f( 2.0, 0.0);
    glVertex3f( 1.0f, -1.0f, 1.0f);
    glEnd();
    
    glFlush();
    glPopMatrix();
    glutSwapBuffers();
}
*/

#endif /* TextureUtils_hpp */
