//
//  TextureUtils.cpp
//  CityBattleDrones
//
//  Created by Alessandro Profenna on 2018-11-17.
//  Copyright © 2018 Alessandro Profenna. All rights reserved.
//

#include "TextureUtils.hpp"



/**************************************************************************
 *  fskip                                                                 *
 *     Skips bytes in a file.                                             *
 **************************************************************************/

void TextureUtils::fskip(FILE *fp, int num_bytes)
{
    int i;
    for (i=0; i<num_bytes; i++)
        fgetc(fp);
}


/**************************************************************************
 *                                                                        *
 *    Loads a bitmap file into memory.                                    *
 **************************************************************************/

ushort TextureUtils::getShort(FILE *fp) //helper function
{ //BMP format uses little-endian integer types
    // get a 2-byte integer stored in little-endian form
    char ic;
    ushort ip;
    ic = fgetc(fp); ip = ic;  //first byte is little one
    ic = fgetc(fp);  ip |= ((ushort)ic << 8); // or in high order byte
    return ip;
}


ulong TextureUtils::getLong(FILE *fp) //helper function
{  //BMP format uses little-endian integer types
    // get a 4-byte integer stored in little-endian form
    ulong ip = 0;
    char ic = 0;
    unsigned char uc = ic;
    ic = fgetc(fp); uc = ic; ip = uc;
    ic = fgetc(fp); uc = ic; ip |=((ulong)uc << 8);
    ic = fgetc(fp); uc = ic; ip |=((ulong)uc << 16);
    ic = fgetc(fp); uc = ic; ip |=((ulong)uc << 24);
    return ip;
}


void TextureUtils::readBMPFile(RGBpixmap *pm, string file)
{
    FILE *fp;
    long index;
    int k,row,col,numPadBytes, nBytesInRow;
    char ch1,ch2;
    ulong fileSize;
    ushort reserved1;    // always 0
    ushort reserved2;     // always 0
    ulong offBits; // offset to image - unreliable
    ulong headerSize;     // always 40
    ulong numCols; // number of columns in image
    ulong numRows; // number of rows in image
    ushort planes;      // always 1
    ushort bitsPerPixel;    //8 or 24; allow 24 here
    ulong compression;      // must be 0 for uncompressed
    ulong imageSize;       // total bytes in image
    ulong xPels;    // always 0
    ulong yPels;    // always 0
    ulong numLUTentries;    // 256 for 8 bit, otherwise 0
    ulong impColors;       // always 0
    long count;
    char dum;
    
    /* open the file */
    const char *fileC = file.c_str();
    if ((fp = fopen(fileC,"rb")) == NULL)
    {
        printf("Error opening file %s.\n",fileC);
        exit(1);
    }
    
    /* check to see if it is a valid bitmap file */
    if (fgetc(fp)!='B' || fgetc(fp)!='M')
    {
        fclose(fp);
        printf("%s is not a bitmap file.\n",fileC);
        exit(1);
    }
    
    fileSize =      getLong(fp);
    reserved1 =    getShort(fp);    // always 0
    reserved2=     getShort(fp);     // always 0
    offBits =        getLong(fp); // offset to image - unreliable
    headerSize =     getLong(fp);     // always 40
    numCols =        getLong(fp); // number of columns in image
    numRows =         getLong(fp); // number of rows in image
    planes=        getShort(fp);      // always 1
    bitsPerPixel=   getShort(fp);    //8 or 24; allow 24 here
    compression =    getLong(fp);      // must be 0 for uncompressed
    imageSize =     getLong(fp);       // total bytes in image
    xPels =        getLong(fp);    // always 0
    yPels =        getLong(fp);    // always 0
    numLUTentries = getLong(fp);    // 256 for 8 bit, otherwise 0
    impColors =     getLong(fp);       // always 0
    
    if(bitsPerPixel != 24)
    { // error - must be a 24 bit uncompressed image
        printf("Error bitsperpixel not 24\n");
        exit(1);
    }
    //add bytes at end of each row so total # is a multiple of 4
    // round up 3*numCols to next mult. of 4
    nBytesInRow = ((3 * numCols + 3)/4) * 4;
    numPadBytes = nBytesInRow - 3 * numCols; // need this many
    pm->nRows = numRows; // set class's data members
    pm->nCols = numCols;
    pm->pixel = (RGB *) malloc(3 * numRows * numCols);//make space for array
    if(!pm->pixel) return; // out of memory!
    count = 0;
    dum;
    for(row = 0; row < numRows; row++) // read pixel values
    {
        for(col = 0; col < numCols; col++)
        {
            int r,g,b;
            b = fgetc(fp); g = fgetc(fp); r = fgetc(fp); //read bytes
            pm->pixel[count].r = r; //place them in colors
            pm->pixel[count].g = g;
            pm->pixel[count++].b = b;
        }
        fskip(fp,numPadBytes);
    }
    fclose(fp);
}


void TextureUtils::setTexture(RGBpixmap *p, GLuint textureID)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, p->nCols, p->nRows, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, p->pixel);
}

vector<RGBpixmap*> TextureUtils::loadTextures(vector<string> texFiles)
{
    vector<RGBpixmap*> pm = {};
    for(int i = 0; i < texFiles.size(); i++)
    {
        pm.push_back(new RGBpixmap());
        readBMPFile(pm.at(i),texFiles.at(i));
        setTexture(pm.at(i),2000 + i);
    }
    return pm;
}


