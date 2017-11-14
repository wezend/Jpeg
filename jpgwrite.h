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
    list<node*> nodeWeightLIST;
    vector<pair<int,string>> jpgTmpDHTtable;
    node *jpgTmpRoot;

    void writeFFDB();
    void writeFFC0();
    void writeFFC4();
    void writeFFDA();

    int tableElementLenth(int originalValue);
    void addInNodeVector(int value);
    void treeValk(node *root);
    void makeHaffmanTree();
    void addInACNodeVector(int zeroCount, int value);
    void addElInNodeLIST(node *tmpNode);
    void makeTree();
    void recTreeValk(node *root, int *lenth);
    void makeTreeRec(int i, int dhtTmp, node* tmpNode, TREE *DHTtree);
};

#endif // JPGWRITE_H
