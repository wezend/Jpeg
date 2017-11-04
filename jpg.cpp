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
#include "jpg.h"
#include "tree.h"


using namespace std;

JPG::JPG()
{

}

JPG::JPG(const char *pFile)
{
    this->pFile=fopen(pFile,"rb");
}

JPG::~JPG()
{
//    for(int i=0;i<4;i++){
//        delete this->dhtAllMark.jpgDHTtables[i]->jpgDHTtreesRoots;
//        delete this->dhtAllMark.jpgDHTtables[i]->jpgDHTtable;
//    }
//    delete this->dhtAllMark.jpgDHTtables;
//    delete this->sosMark.jpgComponentsTable;
}



