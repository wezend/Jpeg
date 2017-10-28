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
    f1 = fopen("D:\\test1.jpg","wb");
    char o[1000];
    o[0]=HEX_TO_DEC("FF");
    o[1]=HEX_TO_DEC("D8");
    fwrite(&o, 2, 1, f1);
    for(int l = 0;l<jpg->dqtMark.jpgDQTmaxTableID;l++){
    o[0]=HEX_TO_DEC("FF");
    o[1]=HEX_TO_DEC("DB");
    o[2]=0;
    o[3]=jpg->dqtMark.jpgDQTLenght[l];
    o[4]=l;
    fwrite(&o,5, 1, f1);
    int i = 0,j = 0,f = 2,z=0;
    while((i<8)&&(j<8)){
        if (f==0){
            o[z]=jpg->dqtMark.jpgDQTtable[l][i][j];
            if(i!=7)
                i+=1;
            else
                j+=1;

            z++;
            f=1;
            continue;
        }
        if(f==1){
            o[z]=jpg->dqtMark.jpgDQTtable[l][i][j];
            i--;
            j++;
            if(i==0 or j==7)
                {f=2;
            }
            z++;
            continue;
        }
        if (f==2){
            o[z]=jpg->dqtMark.jpgDQTtable[l][i][j];
            if(j!=7)
                j++;
            else
                i++;
            z++;
            f=3;
            continue;
        }
        if(f==3){
            o[z]=jpg->dqtMark.jpgDQTtable[l][i][j];
            i++;
            j--;

            if(j==0 or i==7)
                f=0;
            z++;
            continue;
        }
    }
    fwrite(&o, 64, 1, f1);
    }
    o[0]=HEX_TO_DEC("FF");
    o[1]=HEX_TO_DEC("C0");
    o[2]=0;
    o[3]=jpg->sof0Mark.jpgSOF0Lenght;
    o[4]=jpg->sof0Mark.jpgSOF0Precision;
    fwrite(&o, 5, 1, f1);
    if(jpg->sof0Mark.jpgSOF0Height> 255){
       o[0]=jpg->sof0Mark.jpgSOF0Height;
       fwrite(&o, 1, 2, f1);
    }
    else{
        o[0]=0;
        o[1]=jpg->sof0Mark.jpgSOF0Height;
        fwrite(&o, 2, 1, f1);
    }
    if(jpg->sof0Mark.jpgSOF0Height> 255){
       o[0]=jpg->sof0Mark.jpgSOF0Width;
       fwrite(&o, 1, 2, f1);
    }
    else{
        o[0]=0;
        o[1]=jpg->sof0Mark.jpgSOF0Height;
        fwrite(&o, 2, 1, f1);
    }
    o[0]=jpg->sof0Mark.jpgSOF0WComponentValue;
    fwrite(&o, 1, 1, f1);
    for(int i = 0; i < jpg->sof0Mark.jpgSOF0WComponentValue;i++){
        o[i]=jpg->sof0Mark.jpgSOF0WComponents[i].jpgCompID;
        o[i]=jpg->sof0Mark.jpgSOF0WComponents[i].jpgHorizontalDecimation*10+jpg->sof0Mark.jpgSOF0WComponents[i].jpgVerticalDecimation;
        o[i]=jpg->sof0Mark.jpgSOF0WComponents[i].jpgQuantizationTableID;
        fwrite(&o, 3, 1, f1);
    }
    o[0]=HEX_TO_DEC("FF");
    o[1]=HEX_TO_DEC("C4");
    fwrite(&o, 2, 1, f1);
    for(int i = 0;i<4;i++){
    o[0]=jpg->dhtAllMark.jpgDHTtables[i]->jpgDHTLenght;
    o[1]=jpg->dhtAllMark.jpgDHTtables[i]->jpgTableType*10+jpg->dhtAllMark.jpgDHTtables[i]->jpgTableID;
    //o[2]=jpg->dhtAllMark.jpgDHTtables[i]->
    fwrite(&o, 5, 1, f1);
}

}

JPG *JPGWrite::getJpg() const
{
    return jpg;
}

void JPGWrite::setJpg(JPG *value)
{
    jpg = value;
}
