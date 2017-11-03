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

    JPG jpg("D:\\test4.jpg");
    JPGRead jpgRead;
    jpgRead.setJpg(&jpg);

    JPGWrite jpgWrite;
    jpgWrite.setJpg(&jpg);


    jpgRead.read();
    jpgWrite.write();


//    cout<<"~~~~"<<endl;
//    JPG pic1("D:\\test19.jpg");
//    JPG pic2("D:\\test1.jpg");

//    int r1=0;

//    while(true){
//        r1=read_u8(pic1.pFile);
//        if(r1=255){
//            r1=read_u8(pic1.pFile);
//            if(r1==219)
//                break;
//        }
//    }

//    int r2=0;
//    while(true){
//        r2=read_u8(pic2.pFile);
//        if(r2=255){
//            r2=read_u8(pic2.pFile);
//            if(r2==219)
//                break;
//        }
//    }
//    int h=0;
//    while (r1==r2) {
//        r1=read_u8(pic1.pFile);
//        r2=read_u8(pic2.pFile);
//        h++;

//    }

//    cout<<r1<<" "<<r2<<" "<<h;


    return 0;
}
