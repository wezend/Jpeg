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
#include<algorithm>


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


    o[0]=HEX_TO_DEC("FF");
    o[1]=HEX_TO_DEC("D8");
    fwrite(&o, 2, 1, f1);

    writeFFDB();

    writeFFC0();

    writeFFC4();

    writeFFDA();


//    vector<node*> nodeWeightList;

//    JPGALLPICTABLES *tmpP;
//    for(vector<JPGALLPICTABLES*>::iterator j = jpg->picMark.jpgAllPicTables.begin();j!=jpg->picMark.jpgAllPicTables.end();j++){
//        tmpP=*j;
//        for(int ii=3;ii>0;ii--){
//             tmpP->jpgYtable[ii][0][0]=tmpP->jpgYtable[ii][0][0] - tmpP->jpgYtable[ii-1][0][0];
//        }

//        for(int i=0;i<4;i++){
//            bool newTrigger=0;
//             node* y=*f;
//            for(vector<node*>::iterator f=nodeWeightList.begin();f!=nodeWeightList.end();f++){
//                y=*f;
//                if(tmpP->jpgYtable[i][0][0]==y->weight)


//            }

//        }


//    }




         char b=HEX_TO_DEC("FF");
        for(list<char>::iterator j = jpg->picMark.jpgPicture.begin();j!=jpg->picMark.jpgPicture.end();j++){
           o[0]=*j;
           if(b==*j){
               o[1]=0;
               fwrite(&o,2,1,f1);
           }
           else
           fwrite(&o, 1,1,f1);
        }

        o[0]=HEX_TO_DEC("FF");
        o[1]=HEX_TO_DEC("D9");
        fwrite(&o, 2,1,f1);





}

JPG *JPGWrite::getJpg() const
{
    return jpg;
}

void JPGWrite::setJpg(JPG *value)
{
    jpg = value;
}

void JPGWrite::writeFFDB(){
    for(int l = 0;l<jpg->dqtMark.jpgDQTmaxTableID;l++){
    o[0]=HEX_TO_DEC("FF");
    o[1]=HEX_TO_DEC("DB");

    o[2]=jpg->dqtMark.jpgDQTLenght[l] >> 8;
    o[3]=jpg->dqtMark.jpgDQTLenght[l] & 255;

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
}

void JPGWrite::writeFFC0(){

    o[0]=HEX_TO_DEC("FF");
    o[1]=HEX_TO_DEC("C0");

    o[2]=jpg->sof0Mark.jpgSOF0Lenght>>8;
    o[3]=jpg->sof0Mark.jpgSOF0Lenght & 255;


    o[4]=jpg->sof0Mark.jpgSOF0Precision;
    fwrite(&o, 5, 1, f1);

       o[0]=jpg->sof0Mark.jpgSOF0Height>>8;
       o[1]=jpg->sof0Mark.jpgSOF0Height&255;
       fwrite(&o, 2, 1, f1);

       o[0]=jpg->sof0Mark.jpgSOF0Width>>8;
       o[1]=jpg->sof0Mark.jpgSOF0Width&255;
       fwrite(&o, 2, 1, f1);

    o[0]=jpg->sof0Mark.jpgSOF0WComponentValue;
    fwrite(&o, 1, 1, f1);
    for(int i = 0; i < jpg->sof0Mark.jpgSOF0WComponentValue;i++){
        o[0]=jpg->sof0Mark.jpgSOF0WComponents[i].jpgCompID;
        o[1]=jpg->sof0Mark.jpgSOF0WComponents[i].jpgHorizontalDecimation<<4 | (jpg->sof0Mark.jpgSOF0WComponents[i].jpgVerticalDecimation & 15);
        o[2]=jpg->sof0Mark.jpgSOF0WComponents[i].jpgQuantizationTableID;
        fwrite(&o, 3, 1, f1);
    }

}

void JPGWrite::writeFFC4(){
    string x;
    int f,z;

    for(int i = 0;i<4;i++){
    o[0]=HEX_TO_DEC("FF");
    o[1]=HEX_TO_DEC("C4");
    fwrite(&o, 2, 1, f1);

    o[0]=jpg->dhtAllMark.jpgDHTtables[i]->jpgDHTLenght>>8;
    o[1]=jpg->dhtAllMark.jpgDHTtables[i]->jpgDHTLenght & 255;

    fwrite(&o, 2, 1, f1);


    o[0]= jpg->dhtAllMark.jpgDHTtables[i]->jpgTableType<<4 | (jpg->dhtAllMark.jpgDHTtables[i]->jpgTableID & 15);

    fwrite(&o, 1, 1, f1);
    for(int k = 0;k<16;k++){
        o[k]=0;
    }
    x=jpg->dhtAllMark.jpgDHTtables[i]->jpgDHTtable[0].second;
    z = 1, f = 0;

    for(int j=0;j<jpg->dhtAllMark.jpgDHTtables[i]->jpgTableSize;j++){
        x=jpg->dhtAllMark.jpgDHTtables[i]->jpgDHTtable[j].second;
        o[x.size()-1]+=1;
    }

    fwrite(&o, 16, 1, f1);


    for(int j=0;j<jpg->dhtAllMark.jpgDHTtables[i]->jpgTableSize;j++){
        o[j]=jpg->dhtAllMark.jpgDHTtables[i]->jpgDHTtable[j].first;
    }
    fwrite(&o,jpg->dhtAllMark.jpgDHTtables[i]->jpgTableSize,1,f1);

}
}

void JPGWrite::writeFFDA(){
    o[0]=HEX_TO_DEC("FF");
    o[1]=HEX_TO_DEC("DA");

    o[2]=jpg->sosMark.jpgSosLenght>>8;
    o[3]=jpg->sosMark.jpgSosLenght & 255;

    o[4]=jpg->sosMark.jpgSosComponentValue;
    fwrite(&o, 5,1,f1);

    int fir=0,sec=0;

    for(int i = 0;i<jpg->sosMark.jpgSosComponentValue;i++){
        o[0]=i+1;
        fir=jpg->sosMark.jpgComponentsTable[i].first;
        sec=jpg->sosMark.jpgComponentsTable[i].second;
        o[1]=fir << 4 | (sec & 15);

        fwrite(&o, 2,1,f1);
    }
        o[0]=jpg->sosMark.nonameBite1;
        o[1]=jpg->sosMark.nonameBite2;
        o[2]=jpg->sosMark.nonameBite3;
        fwrite(&o, 3,1,f1);
}

