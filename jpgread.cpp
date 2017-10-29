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

//please, don.t use it. Everywhere;
string dhtCode="";

JPGRead::JPGRead()
{

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
    if (lastMark==65504 || lastMark==65534)
        {
        readComment();
    }

    if (lastMark==65504 || lastMark==65534)
        {
        readComment();
    }

   //тест на наличие таблиц квантования;

    if (lastMark!=65499)
        {
        cout<<"Error! It is not DQT tables! "<<lastMark<<endl;
        return;
    }else{
        readDqt();
    }

    //тест на наличие SOF0
    if (lastMark!=65472)
        {
            cout<<"Error! It is not SOF0 marker!"<<endl;
            return;
    }else {
        readSOF0();
    }

    //тест на наличие FFC4
    lastMark=read_u16(jpg->pFile);
    if (lastMark!=65476)
        {
            cout<<"Error! It is not FFC4 marker!"<<endl;
            return;
    }else {
        readFFC4();
    }

    //тест на наличие FFDA
    if (lastMark!=65498)
        {
            cout<<"Error! It is not SOS marker!"<<lastMark<<endl;
            return;
    }else {
        readSOS();
    }



    //я ненавижу эту срань. если кто то попросит
    //объяснить, что написано ниже,
    //я ему в лицо ударю.
    //Я НЕ ПОНИМАЮ КАК ЭТО РАБОТЕТ, ПОТОМУ ЧТО ЭТО ДИЧЬ.(но я это писал)
    //приятного просмотра

    node* noda;
    int bitNumber=7;
    char jpgC;
    int concreteBit=0;
    int index=0;



    int x[3][8][8];
    int f=2;
    int n,m;
    n=m=0;
    int i=0,j=0;








    for(int n=0;n<8;n++){
        for(int m=0;m<8;m++)
            for(int tableType=0;tableType<3;tableType++ ){

            noda=isOK(&bitNumber,&jpgC,&concreteBit, index);
            if(noda->Key==-1)
                break;

            if(tableType==0){
                if(noda->Key==0)
                    addInTable(i, j, x,tableType,noda->Key, f);

                else {
                    addInTable(&i, &j, &x,tableType,koefficient(&bitNumber,&jpgC,&concreteBit,noda->Key),&f);
                     }

                }

            }
            cout<<endl;
        }






//        node* tmpNode;
//        tmpNode=jpg->dhtAllMark.jpgDHTtables[0]->jpgDHTtreesRoots;
//        while (true) {

//            jpgC=readPicChar();
//            if(jpgC==-1)
//                return;

//            int bitNumber=7;

//            while (bitNumber!=-1) {
//                concreteBit=(jpgC >> bitNumber)& 0x01;

//               //cout<<concreteBit<<"    ";

//                if(concreteBit){
//                     tmpNode=tmpNode->Right;
//                }else {
//                    tmpNode=tmpNode->Left;
//                }
//                if(tmpNode->Key!=-1)
//                    break;
//                bitNumber--;
//            }

//            //cout<<tmpNode->Key<<endl;

            


//            break;

//        }






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
//        for(int r=0;r<jpg->dqtMark.jpgDQTmaxTableID;r++){
//            for(int i=0; i<8;i++){
//                for(int j=0;j<8;j++)
//                    cout<<jpg->dqtMark.jpgDQTtable[r][i][j]<<" ";
//                cout<<endl;
//            }
//            cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
//        }



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

void JPGRead::readSOS()
{
    int sosTmp=0;
    jpg->sosMark.jpgSosLenght=read_u16(jpg->pFile);
    jpg->sosMark.jpgSosComponentValue=read_u8(jpg->pFile);
    jpg->sosMark.jpgComponentsTable=new pair<int,int>[jpg->sosMark.jpgSosComponentValue];
    for(int i=0;i<jpg->sosMark.jpgSosComponentValue;i++){
        sosTmp=read_u8(jpg->pFile);
        jpg->sosMark.jpgComponentsTable[i].first=sosTmp>>4;
        jpg->sosMark.jpgComponentsTable[i].second=sosTmp&15;
    }
    jpg->sosMark.nonameBite1=read_u16(jpg->pFile);
    jpg->sosMark.nonameBite2=read_u16(jpg->pFile);
    jpg->sosMark.nonameBite3=read_u16(jpg->pFile);
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

node* JPGRead::isOK(int *bitNumber,char *jpgC,int *concreteBit,int index){

    node* tmpNode;

    //пофиксить [0] на конкретную таблицу в зависимости от кэфов
    tmpNode=jpg->dhtAllMark.jpgDHTtables[index]->jpgDHTtreesRoots;

    while (true) {
        *jpgC=read_u8(jpg->pFile);
        if(*jpgC==255){
            *jpgC=read_u8(jpg->pFile);
                if(*jpgC==217){
                    tmpNode->Key=-1;
                    return tmpNode;
                    }
                }

        if(*bitNumber==-1)
            *bitNumber=7;

        while (*bitNumber!=-1) {
            *concreteBit=(*jpgC >> *bitNumber)& 0x01;

           //cout<<concreteBit<<"    ";


            if(*concreteBit){
                 tmpNode=tmpNode->Right;
            }else {
                tmpNode=tmpNode->Left;
            }

            *bitNumber=*bitNumber-1;
            if(tmpNode->Key!=-1)
                return tmpNode;
        }
    }
}

int JPGRead::koefficient(int *bitNumber,char *jpgC,int *concreteBit, int countOfBits){

    int kef=0;
    bool firstBIT=0;
    bool bitVAL;
    int lenth=countOfBits;

    while (true) {

        if(*bitNumber==-1){
        *jpgC=read_u8(jpg->pFile);
        if(*jpgC==255){
            *jpgC=read_u8(jpg->pFile);
                if(*jpgC==217){
                    return -1;
                    }
                }
        }

        if(*bitNumber==-1)
            *bitNumber=7;


    while(countOfBits||(*bitNumber>-1)){
        *concreteBit=( *jpgC >> *bitNumber) & 0x01;

        if(firstBIT==0){
            firstBIT=1;
            bitVAL=concreteBit;
        }



        kef=kef|*concreteBit;
        kef=kef<<1;
        *bitNumber=*bitNumber-1;
        countOfBits--;

        if(countOfBits)
            if(bitVAL==1)
            return kef;
            else {
                kef=kef-pow(2,lenth)+1;
                return kef;
            }
        }
    }
}

void JPGRead::addInTable(int &i, int &j, int *table, int index, int key,int *f){

    while((i<8)&&(j<8)){
        if (*f==0){

            *table[index][i][j]=key;

            if(i!=7)
                i+=1;
            else
                j+=1;

            if(index==0)
                *f=1;
            continue;
        }
        if(*f==1){

            *table[index][i][j]=key;

            i--;
            j++;
            if(i==0 or j==7){
                if(index==0)
                *f=2;
            }
            continue;
        }
        if (*f==2){

            *table[index][i][j]=key;

            if(j!=7)
                j++;
            else
                i++;

            if(index==0)
            *f=3;

            continue;
        }
        if(*f==3){

            *table[index][i][j]=key;

            i++;
            j--;

            if(j==0 or i==7)

                if(index==0)
                *f=0;

            continue;
        }
    }

    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++)
            cout<<*table[index][i][j]<<" ";
        cout<<endl;
    }
    cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;


}

