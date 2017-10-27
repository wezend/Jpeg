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
