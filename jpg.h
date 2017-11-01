#ifndef JPG_H
#define JPG_H
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
#include <list>

using namespace std;

//FFFE
typedef struct{
    //определяет, есть ли в изображении коментарий. если = 0, значит нет
    //если не 0, то это длина последнего комментария(может быть несколько)
    unsigned int jpgCommentLenght=0;
    string jpgCommentContent[16];
} JPGCOMMENT;

//FFDB
typedef struct
{
    unsigned int jpgDQTLenght[16];
    unsigned int jpgDQTLVT[16];
    unsigned int jpgDQTmaxTableID=0;
    unsigned int jpgDQTtable[16][8][8];

} JPGDQT;

//SOF0
typedef struct
{
    unsigned int jpgCompID=0;
    unsigned int jpgHorizontalDecimation=0;
    unsigned int jpgVerticalDecimation=0;
    unsigned int jpgQuantizationTableID=0;
} JPGCOMPONENT;

typedef struct
{
    unsigned int jpgSOF0Lenght=0;
    unsigned int jpgSOF0Precision=0;
    unsigned int jpgSOF0Height=0;
    unsigned int jpgSOF0Width=0;
    unsigned int jpgSOF0WComponentValue=0;
    JPGCOMPONENT jpgSOF0WComponents[16];

} JPGSOF0;

typedef struct
{
    unsigned int jpgDHTLenght=0;
    unsigned int jpgTableType=0;
    unsigned int jpgTableID=0;
    unsigned int jpgTableSize=0;
    pair<int,string> *jpgDHTtable;
    node *jpgDHTtreesRoots;

} JPGDHT;

typedef struct
{
    JPGDHT  *jpgDHTtables[4];
} JPGDHTALL;

typedef struct
{
    unsigned int jpgSosLenght=0;
    unsigned int jpgSosComponentValue=0;
    pair<int,int> *jpgComponentsTable;

    unsigned int nonameBite1=0;
    unsigned int nonameBite2=0;
    unsigned int nonameBite3=0;

} JPGSOS;

typedef struct
{
    list<char> jpgPicture;

} JPGPICTURE;

class JPG
{
public:
    JPG();
    JPG(const char *pFile);
    FILE * pFile;
    JPGCOMMENT comMark;
    JPGDQT dqtMark;
    JPGSOF0 sof0Mark;
    JPGDHTALL dhtAllMark;
    JPGSOS sosMark;
    JPGPICTURE picMark;
};

#endif // JPG_H
