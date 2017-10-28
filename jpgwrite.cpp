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
int JPGWrite::HEX_TO_DEC(string st)
{
    int i,s,k,p;
    s=0;
    p=st.length()-1;
    for (i=0; st[i]!='\0'; i++){
switch (toupper(st[i]))
{
    case 'A': k=10; break;
    case 'B': k=11; break;
    case 'C': k=12; break;
    case 'D': k=13; break;
    case 'E': k=14; break;
    case 'F': k=15; break;
    case '1': k=1; break;
    case '2': k=2; break;
    case '3': k=3; break;
    case '4': k=4; break;
    case '5': k=5; break;
    case '6': k=6; break;
    case '7': k=7; break;
    case '8': k=8; break;
    case '9': k=9; break;
    case '0': k=0; break;
}
s=s+k*pow(16,p);
p--;
}
return s;
}

void JPGWrite::write()
{
    f = fopen("D:\\test1.jpg","w+");
    //ofstream fcin;
    //fcin.open("D:\\test1.jpg");
    char o[10000];
    o[0]=HEX_TO_DEC("FF");
    o[1]=HEX_TO_DEC("D8");
    cout<<"LOL"<<jpg->dqtMark.jpgDQTLenght[0];
    //for(int j = 0;j < jpg->dqtMark.)
    o[2]=o[0];
    o[3]=HEX_TO_DEC("DB");
    for(int i = 0;i<67;i++){

    }
    //fcin<<o[3];
    fwrite(&o, sizeof (o), 1, f);
}

JPG *JPGWrite::getJpg() const
{
    return jpg;
}

void JPGWrite::setJpg(JPG *value)
{
    jpg = value;
}
