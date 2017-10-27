#ifndef JPGREAD_H
#define JPGREAD_H
#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cstdio>
#include <tree.h>
#include <utility>
#include <fstream>
#include <string.h>
#include <math.h>


using namespace std;

#include "jpg.h"

static unsigned short read_u8(FILE *fp);
static unsigned short read_u16(FILE *fp);
static unsigned int   read_u32(FILE *fp);
static int            read_s32(FILE *fp);


class JPGRead
{
public:
    JPGRead();
    void change();
    void read();



    JPG *getJpg() const;
    void setJpg(JPG *value);
private:
    JPG *jpg;
    int lastMark;
    void readComment();
    void readDqt();
    void readSOF0();
    void readFFC4();
    void readSOS();


};

void makeTreeRec(int i, int dhtTmp, node* tmpNode, TREE *DHTtree);

static unsigned short read_u8(FILE *fp)
{
    unsigned char b0;

    b0 = getc(fp);

    return b0;
}

static unsigned short read_u16(FILE *fp)
{
    unsigned char b0, b1;

    b1 = getc(fp);
    b0 = getc(fp);


    return ( (b1 << 8)|b0);
}

static unsigned int read_u32(FILE *fp)
{
    unsigned char b0, b1, b2, b3;

    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);

    return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

static int read_s32(FILE *fp)
{
    unsigned char b0, b1, b2, b3;

    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);

    return ((int)(((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}



#endif // JPGREAD_H
