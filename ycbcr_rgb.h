#ifndef YCBCR_RGB_H
#define YCBCR_RGB_H
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


class YCbCr_RGB
{
public:
    YCbCr_RGB();
    JPG *getJpg() const;
    void setJpg(JPG *value);

    void ConvertYCbCrToRGB();
private:
    JPG *jpg;
};

#endif // YCBCR_RGB_H
