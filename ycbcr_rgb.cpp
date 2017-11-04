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

void YCbCr_RGB::ConvertYCbCrToRGB(){
        JPGALLPICTABLES *tmpP;
        int ccounter=0;
        int tmpYtables[4][8][8];
        int tmpCbTable[8][8];
        int tmpCrTable[8][8];

        //квантизация таблиц
            for(vector<JPGALLPICTABLES*>::iterator j = jpg->picMark.jpgAllPicTables.begin();j!=jpg->picMark.jpgAllPicTables.end();j++){

                tmpP=*j;

                for(int ii=0;ii<4;ii++){
                    for(int i=0;i<8;i++){
                        for(int jj=0;jj<8;jj++){
                            tmpYtables[ii][i][jj]=tmpP->jpgYtable[ii][i][jj] * jpg->dqtMark.jpgDQTtable[0][i][jj];
                            cout<<dec<<tmpYtables[ii][i][jj]<<" ";
                        }
                        cout<<endl;
                    }
                    cout<<endl;
                }


                    for(int i=0;i<8;i++){
                        for(int jj=0;jj<8;jj++){
                            tmpCbTable[i][jj]=tmpP->jpgCb[i][jj] * jpg->dqtMark.jpgDQTtable[1][i][jj];
                            cout<<dec<<tmpCbTable[i][jj]<<" ";
                        }
                    cout<<endl;
                    }

                    cout<<endl;

                    for(int i=0;i<8;i++){
                        for(int jj=0;jj<8;jj++){
                             tmpCrTable[i][jj]=tmpP->jpgCr[i][jj] * jpg->dqtMark.jpgDQTtable[1][i][jj];
                            cout<<dec<<tmpCrTable[i][jj]<<" ";
                        }
                    cout<<endl;
                    }

            }

            cout<<"~~~~~~~~~~"<<endl;

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
                cout <<tmpYtable[i][j]<<" ";
            }
            cout<<endl;
        }


}
