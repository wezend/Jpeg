#ifndef JPGWRITE_H
#define JPGWRITE_H
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


class JPGWrite
{
public:
    JPGWrite();
    void write();
    JPG *getJpg() const;
    void setJpg(JPG *value);
    int HEX_TO_DEC(string st);

private:
    JPG *jpg;
    FILE *f1;
};

#endif // JPGWRITE_H
