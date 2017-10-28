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
    f = fopen("D:\\test2.jpg","w+");
    //ofstream fcin;
    //fcin.open("D:\\test1.jpg");
    int o[100];
    o[0]=HEX_TO_DEC("FF");
    o[1]=HEX_TO_DEC("D8");
    int k = 2;
    for(int l = 0;l< 1;l++){
    o[k]=o[0];
    k++;
    o[k]=HEX_TO_DEC("DB");
    k++;
    o[k]=0;
    k++;
    o[k]=jpg->dqtMark.jpgDQTLenght[l];
    k++;
    o[k]=l;
    k++;
    //o[k+z]=jpg->dqtMark.jpgDQTtable[l][i][j];
    int i = 0,j = 0,f = 2,z=0;
    while((i<8)&&(j<8)){
        cout<<z<<" ";
        if (f==0){
            o[k+z]=z;
            cout<<o[k+z]<<" ";
            if(i!=7)
                i+=1;
            else
                j+=1;
            //o[k+z]=jpg->dqtMark.jpgDQTtable[l][i][j];

            z++;
            f=1;
            continue;
        }
        if(f==1){
           o[k+z]=z;
           cout<<o[k+z]<<" ";
            i--;
            j++;
            if(i==0 or j==7)
                {f=2;
            }
            //o[k+z]=jpg->dqtMark.jpgDQTtable[l][i][j];

            z++;
            continue;
        }
        if (f==2){
           o[k+z]=z;
           cout<<o[k+z]<<" ";
            if(j!=7)
                j++;
            else
                i++;
            //o[k+z]=jpg->dqtMark.jpgDQTtable[l][i][j];

            z++;
            f=3;
            continue;
        }
        if(f==3){
            o[k+z]=z;
            cout<<o[k+z]<<" ";
            i++;
            j--;

            if(j==0 or i==7)
                f=0;
            //o[k+z]=jpg->dqtMark.jpgDQTtable[l][i][j];

            z++;
            continue;
        }
    }
    cout<<"      "<<k<<"    "<<jpg->dqtMark.jpgDQTLenght[l];
    k=k+jpg->dqtMark.jpgDQTLenght[l]-3;
    cout<<"      "<<k<<"    ";
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
