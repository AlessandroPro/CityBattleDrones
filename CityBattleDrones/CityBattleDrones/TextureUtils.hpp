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

#endif /* TextureUtils_hpp */
