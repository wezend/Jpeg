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
#include"tree.h"
#include "jpgread.h"


using namespace std;

//please, don.t use it;
string dhtCode="";

JPGRead::JPGRead()
{

}

void JPGRead::change()
{
    jpg->comMark.jpgCommentLenght=4;
    cout<<"JPGRead::change()"<<endl;
}

void JPGRead::read()
{
    //тест на формат файла jpeg
    lastMark=read_u16(jpg->pFile);
    if (lastMark!=65496)
        {
            cout<<"Error! It is not JPEG!"<<endl;
            return;
        }

    //тест на наличие комментария
    lastMark=read_u16(jpg->pFile);
    if (lastMark==65504)
        {
        readComment();
    }

   //тест на наличие таблиц квантования;
    if (lastMark!=65499)
        {
        cout<<"Error! It is not DQT tables!"<<endl;
    }else{
        readDqt();
    }

    //тест на наличие SOF0
    if (lastMark!=65472)
        {
            cout<<"Error! It is not SOF0 marker!"<<endl;
    }else {
        readSOF0();
    }

    //тест на наличие FFC4
    lastMark=read_u16(jpg->pFile);
    if (lastMark!=65476)
        {
            cout<<"Error! It is not FFC4 marker!"<<endl;
    }else {
        readFFC4();
    }

    cout<<"to doooo"<<endl;

}

JPG *JPGRead::getJpg() const
{
    return jpg;
}

void JPGRead::setJpg(JPG *value)
{
    jpg = value;
}

void JPGRead::readComment()
{
    int t=0;
    string sTmp;

    do{

    int s=0;
    jpg->comMark.jpgCommentLenght=read_u16(jpg->pFile);
    for(int i=0;i<jpg->comMark.jpgCommentLenght-2;i++)
    {
        s=read_u8(jpg->pFile);
        sTmp+=s;
    }
    jpg->comMark.jpgCommentContent[t]=sTmp;
    t++;
    lastMark=read_u16(jpg->pFile);
    }while(lastMark==65504+t);
}

void JPGRead::readDqt()
{

    int t=0;
    do{
     jpg->dqtMark.jpgDQTLenght[t]=read_u16(jpg->pFile);


    int LVS=read_u8(jpg->pFile);
    if((LVS>>4)==1)
         jpg->dqtMark.jpgDQTLVT[t]=2;
    else
         jpg->dqtMark.jpgDQTLVT[t]=1;


     int z=2;
     int f=2;
     int n,m;
     n=m=0;
     jpg->dqtMark.jpgDQTtable[t][n][m]=read_u8(jpg->pFile);

     do{
         if (f==0){
             if(n!=7)
                 n++;
             else
                 m++;
             f=1;

             if(jpg->dqtMark.jpgDQTLVT[t]==1)
                jpg->dqtMark.jpgDQTtable[t][n][m]=read_u8(jpg->pFile);
             else {
                 jpg->dqtMark.jpgDQTtable[t][n][m]=read_u16(jpg->pFile);
             }
             z++;
             continue;
         }
         if(f==1){
             n--;
             m++;
             if(n==0 or m==7)
                 {f=2;}

             if(jpg->dqtMark.jpgDQTLVT[t]==1)
                jpg->dqtMark.jpgDQTtable[t][n][m]=read_u8(jpg->pFile);
             else {
                 jpg->dqtMark.jpgDQTtable[t][n][m]=read_u16(jpg->pFile);
             }
             z++;
             continue;
         }

         if (f==2){
             if(m!=7)
                 m++;
             else
                 n++;

             if(jpg->dqtMark.jpgDQTLVT[t]==1)
                jpg->dqtMark.jpgDQTtable[t][n][m]=read_u8(jpg->pFile);
             else {
                 jpg->dqtMark.jpgDQTtable[t][n][m]=read_u16(jpg->pFile);
             }

             z++;
             f=3;
             continue;
         }
         if(f==3){
             n++;
             m--;
             if(m==0 or n==7)
                 f=0;

             if(jpg->dqtMark.jpgDQTLVT[t]==1)
                jpg->dqtMark.jpgDQTtable[t][n][m]=read_u8(jpg->pFile);
             else {
                 jpg->dqtMark.jpgDQTtable[t][n][m]=read_u16(jpg->pFile);
             }

             z++;
             continue;
         }
     }while(z!=65);

     t++;
     lastMark=read_u16(jpg->pFile);
    }while(lastMark==65499);
    jpg->dqtMark.jpgDQTmaxTableID=t;

    //вывод таблиц квантования
    //    for(int r=0;r<jpg->dqtMark.jpgDQTmaxTableID;r++){
    //        for(int i=0; i<8;i++){
    //            for(int j=0;j<8;j++)
    //                cout<<jpg->dqtMark.jpgDQTtable[r][i][j]<<" ";
    //            cout<<endl;
    //        }
    //        cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    //    }



}

void JPGRead::readSOF0()
{
    //считывание SOF0
    jpg->sof0Mark.jpgSOF0Lenght=read_u16(jpg->pFile);
    jpg->sof0Mark.jpgSOF0Precision=read_u8(jpg->pFile);
    jpg->sof0Mark.jpgSOF0Height=read_u16(jpg->pFile);
    jpg->sof0Mark.jpgSOF0Width=read_u16(jpg->pFile);
    jpg->sof0Mark.jpgSOF0WComponentValue=read_u8(jpg->pFile);

    for(int i=0;i<jpg->sof0Mark.jpgSOF0WComponentValue;i++){

       jpg->sof0Mark.jpgSOF0WComponents[i].jpgCompID=read_u8(jpg->pFile);

       int decimation=read_u8(jpg->pFile);
       jpg->sof0Mark.jpgSOF0WComponents[i].jpgHorizontalDecimation=decimation>>4;
       jpg->sof0Mark.jpgSOF0WComponents[i].jpgVerticalDecimation=decimation&15;

       jpg->sof0Mark.jpgSOF0WComponents[i].jpgQuantizationTableID=read_u8(jpg->pFile);
    }
}

void JPGRead::readFFC4()
{
    int dhtAllMarkCounter=0;

    //считывание FFC4
    do{

    JPGDHT *dhtMark=new JPGDHT;

    dhtMark->jpgDHTLenght=read_u16(jpg->pFile);
    int dhtTmp=read_u8(jpg->pFile);
    if((dhtTmp>>4)==1)
        dhtMark->jpgTableType=1;
    dhtMark->jpgTableID=dhtTmp&15;

    int dhtTmpArr[16];
    int dhtElCount=0;

    for(int i=0;i<16;i++){
        dhtTmpArr[i]=read_u8(jpg->pFile);
        dhtElCount+=dhtTmpArr[i];
    }

    dhtMark->jpgTableSize=dhtElCount;
    dhtMark->jpgDHTtable=new pair<int,string>[dhtElCount];
    int dhtTabeleCounter=0;

    TREE DHTtree;
    node *tmpNode;
    tmpNode=DHTtree.root;



    for(int i=0;i<16;i++){
        for(int j=0;j<dhtTmpArr[i];j++){
            dhtTmp=read_u8(jpg->pFile);
            dhtCode="";
            makeTreeRec(i,dhtTmp,tmpNode, &DHTtree);
            dhtMark->jpgDHTtable[dhtTabeleCounter].first=dhtTmp;
            dhtMark->jpgDHTtable[dhtTabeleCounter].second=dhtCode;
            dhtTabeleCounter++;
        }
    }


    jpg->dhtAllMark.jpgDHTtables[dhtAllMarkCounter]=dhtMark;
    jpg->dhtAllMark.jpgDHTtables[dhtAllMarkCounter]->jpgDHTtreesRoots=DHTtree.root;
    dhtAllMarkCounter++;





    lastMark=read_u16(jpg->pFile);
    }while(lastMark==65476);

    //демонстрация работоспособности считывания FFC4
//    for(int i=0;i<4;i++) {
//        for(int j=0;j<jpg->dhtAllMark.jpgDHTtables[i]->jpgTableSize;j++)
//        cout<<hex<<jpg->dhtAllMark.jpgDHTtables[i]->jpgDHTtable[j].first<<"; "<<jpg->dhtAllMark.jpgDHTtables[i]->jpgDHTtable[j].second<<endl;
//        cout<<"~~~~~~~~~~~~~~"<<endl;
//    }


}



void makeTreeRec(int i, int dhtTmp, node *tmpNode, TREE *DHTtree)
{
    if (tmpNode->Left !=0 and tmpNode->Right !=0 and tmpNode->Left->Key != -1 and tmpNode->Right->Key != -1){
    tmpNode->Key = -2;
    dhtCode.erase(dhtCode.length()-1,1);
    makeTreeRec(i,dhtTmp,tmpNode->Back,DHTtree);
    }
    if(tmpNode->Left==0){
    if(tmpNode->Lvl==i){
    tmpNode->Left=DHTtree->addNode(dhtTmp,(tmpNode->Lvl+1),tmpNode);
    dhtCode=dhtCode+"0";

    }else {
    tmpNode->Left=DHTtree->addNode(-1,(tmpNode->Lvl+1),tmpNode);
    dhtCode+="0";
    makeTreeRec(i,dhtTmp,tmpNode->Left,DHTtree);
    }
    }else if (tmpNode->Left->Key==-1) {
    dhtCode+="0";
    makeTreeRec(i,dhtTmp,tmpNode->Left,DHTtree);
    }else if(tmpNode->Right==0){
    if(tmpNode->Lvl==i){
    tmpNode->Right=DHTtree->addNode(dhtTmp,(tmpNode->Lvl+1),tmpNode);
    dhtCode+="1";
    }else {
    tmpNode->Right=DHTtree->addNode(-1,(tmpNode->Lvl+1),tmpNode);
    dhtCode+="1";
    makeTreeRec(i,dhtTmp,tmpNode->Right,DHTtree); }
    }else if (tmpNode->Right->Key==-1) {
    dhtCode+="1";
    makeTreeRec(i,dhtTmp,tmpNode->Right,DHTtree);
    }

}
