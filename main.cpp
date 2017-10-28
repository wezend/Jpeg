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
#include "jpgread.h"
#include "jpgwrite.h"

using namespace std;

int main(int argc, char *argv[])
{

    JPG jpg("D:\\test.jpg");
    JPGRead jpgRead;
    jpgRead.setJpg(&jpg);

    JPGWrite jpgWrite;
    jpgWrite.setJpg(&jpg);
    jpgWrite.write();

    jpgRead.read();


    return 0;
}
