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


//Клас для записи кода изображения в
//.jpg файл. Реализван метод write, остальные
//инкапсулированы в приватную секцию класса

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
    char o[1000];
    vector<node*> nodeWeightList;

    void writeFFDB();
    void writeFFC0();
    void writeFFC4();
    void writeFFDA();

    int tableElementLenth(int originalValue);
    void addInNodeVector(int value);
    void treeValk(node *root1,node *root2);
};

#endif // JPGWRITE_H
