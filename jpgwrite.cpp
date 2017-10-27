#include "jpgwrite.h"
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


JPGWrite::JPGWrite()
{

}

void JPGWrite::change()
{
    jpg->comMark.jpgCommentLenght=8;
    cout<<"JPGWrite::change()"<<endl;

}

JPG *JPGWrite::getJpg() const
{
    return jpg;
}

void JPGWrite::setJpg(JPG *value)
{
    jpg = value;
}
