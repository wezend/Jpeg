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

typedef struct
{
    int z=0;
    int f=0;
    int n=0;
    int m=0;
    int x[8][8];
} globlalTable;


class JPGRead
{
public:
    JPGRead();
    void read();

    JPG *getJpg() const;
    void setJpg(JPG *value);
private:
    JPG *jpg;
    int lastMark;
    globlalTable readTmpTable;

    char tmpPicByte;
    int bitCounter=-1;

    void readComment();
    void readDqt();
    void readSOF0();
    void readFFC4();
    void readSOS();
    void readPic();

    node* isOK(int *bitNumber,char *jpgC,int *concreteBit, int index);
    int koefficient(int *bitNumber,char *jpgC,int *concreteBit, int countOfBits);
    void addInTable(int *i, int *j, int index, int *f);
    char getNewByte();

    list<char>::iterator jpgPicIterator;
    int getNewBit();
    node *findNode(node *root);

    int DCtableAlgorithm(node *tmpNode);
    int ACtableAlgorithm(int OriginalLenth);
    void addInACTable(int value);
    void addACInTable(node *originalRoot);
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
