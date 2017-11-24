#include "ycbcr_rgb.h"
#include "jpgread.h"
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
#include <windef.h>
#include"tree.h"
#include "jpgread.h"
#include "ycbcr_rgb.h"
#include "ycbcr_rgb.h"
#include <iomanip>

YCbCr_RGB::YCbCr_RGB()
{

}

JPG *YCbCr_RGB::getJpg() const
{
    return jpg;
}

void YCbCr_RGB::setJpg(JPG *value)
{
    jpg = value;
}
int YCbCr_RGB::obr_cos_sin(int x,int y, int z){
    int s = 0, cu, cv, b[4][8][8];
    for(int u = 0;u < 7;u++){
        for(int v = 0;v< 7;v++){
            if(u == 0){
                cu = 1/sqrt(2);
            }
            else
                cu = 1;
            if(v == 0)
                cv = 1/sqrt(2);
            else
                cv = 1;
            s =s + cv * cu * cos((2 * x + 1) * u * 3.14 / 16) * cos((2 * y + 1) * v * 3.14/16) * b[z][x][y];
        }

}
    return s*0.25;
}
int YCbCr_RGB::cos_sin(int x,int y, int z){
    int f = 0, cu, cv;
    for(int u = 0;u < 7;u++){
        for(int v = 0;v< 7;v++){
            if(u == 0){
                cu = 1/2;
            }
            else
                cu = 1;
            if(v == 0)
                cv = 1/2;
            else
                cv = 1;
            f =f +cos((2*x+1)*u*3.14/16)*cos((2*y+1)*v*3.14/16)*tmpYtables[z][x][y];// вместо таблицы тм это должна быть
        }

}
    return f*0.25*cu*cv;
}
void YCbCr_RGB::ConvertYCbCrToRGB(){
        JPGALLPICTABLES *tmpP;
        int ccounter=0;

        //квантизация таблиц
            for(vector<JPGALLPICTABLES*>::iterator j = jpg->picMark.jpgAllPicTables.begin();j!=jpg->picMark.jpgAllPicTables.end();j++){

                tmpP=*j;

                for(int ii=0;ii<4;ii++){
                    for(int i=0;i<8;i++){
                        for(int jj=0;jj<8;jj++){
                            tmpYtables[ii][i][jj]=tmpP->jpgYtable[ii][i][jj] * jpg->dqtMark.jpgDQTtable[0][i][jj];
//                            cout<<dec<<tmpYtables[ii][i][jj]<<" ";
                        }
//                        cout<<endl;
                    }
//                    cout<<endl;
                }


                    for(int i=0;i<8;i++){
                        for(int jj=0;jj<8;jj++){
                            tmpCbTable[i][jj]=tmpP->jpgCb[i][jj] * jpg->dqtMark.jpgDQTtable[1][i][jj];
//                            cout<<dec<<tmpCbTable[i][jj]<<" ";
                        }
//                    cout<<endl;
                    }

//                    cout<<endl;

                    for(int i=0;i<8;i++){
                        for(int jj=0;jj<8;jj++){
                             tmpCrTable[i][jj]=tmpP->jpgCr[i][jj] * jpg->dqtMark.jpgDQTtable[1][i][jj];
//                            cout<<dec<<tmpCrTable[i][jj]<<" ";
                        }
//                    cout<<endl;
                    }

            }

//            cout<<"~~~~~~~~~~"<<endl;

            double tmpYtable[8][8];
            double ci,cj;
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                if(i==0){
                    ci=1/sqrt(2);
                }else{
                    ci=1;
                }
                if(j==0){
                    cj=1/sqrt(2);
                }else{
                    cj=1;
                }

                tmpYtable[i][j]=0.25*(ci*cj*tmpYtables[0][i][j]*cos(((2.0*i+1.0)*i*M_PI)/16.0)*cos(((2.0*j+1.0)*j*M_PI)/16.0));
//                cout <<tmpYtable[i][j]<<" ";
            }
//            cout<<endl;
        }


}
