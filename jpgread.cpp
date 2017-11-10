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
    }
    else
        {
        readDqt();
    }

    //тест на наличие SOF0
    if (lastMark!=65472)
        {
            cout<<"Error! It is not SOF0 marker!"<<endl;
            return;
    }
    else
        {
        readSOF0();
    }

    //тест на наличие FFC4
    lastMark=read_u16(jpg->pFile);
    if (lastMark!=65476)
        {
            cout<<"Error! It is not FFC4 marker!"<<endl;
            return;
    }
    else
        {
        readFFC4();
    }

    //тест на наличие FFDA
    if (lastMark!=65498)
        {
            cout<<"Error! It is not SOS marker!"<<lastMark<<endl;
            return;
    }
    else
        {
        readSOS();
    }

//ВВОД изображения
    readPic();

//дешифровка изображения
    decodTables();

//    cout<<"to doooo"<<endl;

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

//    демонстрация работоспособности считывания FFC4
    for(int i=0;i<4;i++) {
        for(int j=0;j<jpg->dhtAllMark.jpgDHTtables[i]->jpgTableSize;j++)
        cout<<hex<<jpg->dhtAllMark.jpgDHTtables[i]->jpgDHTtable[j].first<<"; "<<jpg->dhtAllMark.jpgDHTtables[i]->jpgDHTtable[j].second<<endl;
        cout<<"~~~~~~~~~~~~~~"<<endl;
    }


}

void JPGRead::readSOS()
{
    int sosTmp=0;
    jpg->sosMark.jpgSosLenght=read_u16(jpg->pFile);
    jpg->sosMark.jpgSosComponentValue=read_u8(jpg->pFile);
    jpg->sosMark.jpgComponentsTable=new pair<int,int>[jpg->sosMark.jpgSosComponentValue];
    for(int i=0;i<jpg->sosMark.jpgSosComponentValue;i++){
        sosTmp=read_u8(jpg->pFile);
        sosTmp=read_u8(jpg->pFile);
        jpg->sosMark.jpgComponentsTable[i].first=sosTmp>>4;
        jpg->sosMark.jpgComponentsTable[i].second=sosTmp&15;
    }
    jpg->sosMark.nonameBite1=read_u8(jpg->pFile);
    jpg->sosMark.nonameBite2=read_u8(jpg->pFile);
    jpg->sosMark.nonameBite3=read_u8(jpg->pFile);
}

void JPGRead::readPic(){
    int cTmp;
    while(true){
        cTmp=read_u8(jpg->pFile);
        if(cTmp==255){
            cTmp=read_u8(jpg->pFile);
            if(cTmp==217)
                break;
            else {
                cTmp=255;
            }
        }
        jpg->picMark.jpgPicture.push_back(cTmp);
//        cout<<cTmp<<endl;
    }

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


char JPGRead::getNewByte(){
    char tmp= *jpgPicIterator;
    jpgPicIterator++;
    bitCounter=7;
    return tmp;
}

int JPGRead::getNewBit(){

     if(bitCounter<0){
         tmpPicByte=getNewByte();
     }

     int concreteBit=( tmpPicByte >> bitCounter) & 0x01;
//     cout<<"Bit= "<<concreteBit<<"; BitNumber="<<bitCounter<<endl;
     bitCounter--;

     //    побитовый гуляка
         //    int s;
         //    for(int i=0;i<8;i++){
         //        s=getNewBit();
         //        cout<<s<<"  ";
         //    }

     return concreteBit;
}

node* JPGRead::findNode(node *root){
    node* tmpNode;
    tmpNode=root;
    int tmpPicBit=0;
    while (true)
        {

        tmpPicBit=getNewBit();

        if(tmpPicBit){
             tmpNode=tmpNode->Right;
        }else {
            tmpNode=tmpNode->Left;
        }

        if(tmpNode->Key>=0)
            return tmpNode;


    }
}

int JPGRead::DCtableAlgorithm(node* tmpNode){
    int lenth=tmpNode->Key;
    int value=0;
    if(lenth!=0){
        bool isFirst=1;
        int firstBit=0;
        int concreteBit;
        while (lenth>0) {
            concreteBit=getNewBit();
            if(isFirst){
                firstBit=concreteBit;
                isFirst=0;
            }
            value=value<<1;
            value=value|concreteBit;
            lenth--;
        }

        if(firstBit==0){
            value=value-pow(2,tmpNode->Key)+1;
        }
    }
     return value;
}

int JPGRead::ACtableAlgorithm(int OriginalLenth){

    int lenth=OriginalLenth;
    int value=0;

    if(lenth!=0){
        bool isFirst=1;
        int firstBit=0;
        int concreteBit;
        while (lenth>0) {

            concreteBit=getNewBit();
            if(isFirst){
                firstBit=concreteBit;
                isFirst=0;
            }
            value=value<<1;
            value=value|concreteBit;
            lenth--;
        }

        if(firstBit==0){
            value=value-pow(2,OriginalLenth)+1;
        }
    }
     return value;
}

void JPGRead::addACInTable(node* originalRoot){

    node* root=originalRoot;
    do{

    node* tmpNode=findNode(root);
    int lenth=tmpNode->Key & 15;
    int zeroCounter=tmpNode->Key >> 4;

    if(tmpNode->Key==0){
        do{
            addInACTable(0);
        }while(readTmpTable.z!=65);

    }else{
    for(int i=0;i<zeroCounter;i++){
        addInACTable(0);
    }
    addInACTable(ACtableAlgorithm(lenth));
    }

    }while(readTmpTable.z!=65);
}

void JPGRead::addInACTable(int value){



        if (readTmpTable.f==0){
            if(readTmpTable.n!=7)
                readTmpTable.n+=1;
            else
                readTmpTable.m+=1;
            readTmpTable.f=1;

            readTmpTable.x[readTmpTable.n][readTmpTable.m]=value;
//            cout<<"f= "<<readTmpTable.f<<"; x["<<readTmpTable.n<<"]["<<readTmpTable.m<<"]= "<<value<<endl;


            readTmpTable.z+=1;

            return;
        }
        if(readTmpTable.f==1){
            readTmpTable.n-=1;
            readTmpTable.m+=1;
            if(readTmpTable.n==0 or readTmpTable.m==7)
                {readTmpTable.f=2;}

            readTmpTable.x[readTmpTable.n][readTmpTable.m]=value;
//            cout<<"f= "<<readTmpTable.f<<"; x["<<readTmpTable.n<<"]["<<readTmpTable.m<<"]= "<<value<<endl;


            readTmpTable.z+=1;
            return;
        }

        if (readTmpTable.f==2){
            if(readTmpTable.m!=7)
                readTmpTable.m+=1;
            else
                readTmpTable.n+=1;

            readTmpTable.x[readTmpTable.n][readTmpTable.m]=value;
//            cout<<"f= "<<readTmpTable.f<<"; x["<<readTmpTable.n<<"]["<<readTmpTable.m<<"]= "<<value<<endl;


            readTmpTable.z+=1;
            readTmpTable.f=3;
            return;
        }
        if(readTmpTable.f==3){
            readTmpTable.n+=1;
            readTmpTable.m-=1;
            if(readTmpTable.m==0 or readTmpTable.n==7)
                readTmpTable.f=0;

            readTmpTable.x[readTmpTable.n][readTmpTable.m]=value;
//            cout<<"f= "<<readTmpTable.f<<"; x["<<readTmpTable.n<<"]["<<readTmpTable.m<<"]= "<<value<<endl;

            readTmpTable.z+=1;
            return;
        }
}


void JPGRead::decodTables(){
    jpgPicIterator=jpg->picMark.jpgPicture.begin();

    while (jpgPicIterator!=jpg->picMark.jpgPicture.end()) {

    JPGALLPICTABLES *tmpPicTables=new JPGALLPICTABLES;

    for(int ii=0;ii<4;ii++){
    int indexID=0;

    readTmpTable.x[0][0]=DCtableAlgorithm(findNode(jpg->dhtAllMark.jpgDHTtables[indexID]->jpgDHTtreesRoots));
    indexID++;

    readTmpTable.z=2;
    readTmpTable.f=2;
    readTmpTable.n=0;
    readTmpTable.m=0;

    addACInTable(jpg->dhtAllMark.jpgDHTtables[indexID]->jpgDHTtreesRoots);

    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            tmpPicTables->jpgYtable[ii][i][j]=readTmpTable.x[i][j];
            if(ii>0 && i+j==0)
                tmpPicTables->jpgYtable[ii][i][j]=tmpPicTables->jpgYtable[ii][i][j] + tmpPicTables->jpgYtable[ii-1][i][j];

            readTmpTable.x[i][j]=0;
        }
     }
    }

    for(int ii=0;ii<1;ii++){
        int indexID=2;

        readTmpTable.x[0][0]=DCtableAlgorithm(findNode(jpg->dhtAllMark.jpgDHTtables[indexID]->jpgDHTtreesRoots));
        indexID++;

        readTmpTable.z=2;
        readTmpTable.f=2;
        readTmpTable.n=0;
        readTmpTable.m=0;

        addACInTable(jpg->dhtAllMark.jpgDHTtables[indexID]->jpgDHTtreesRoots);
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                tmpPicTables->jpgCb[i][j]=readTmpTable.x[i][j];
                readTmpTable.x[i][j]=0;
            }
        }
    }

    for(int ii=0;ii<1;ii++){
        int indexID=2;

        readTmpTable.x[0][0]=DCtableAlgorithm(findNode(jpg->dhtAllMark.jpgDHTtables[indexID]->jpgDHTtreesRoots));
        indexID++;

        readTmpTable.z=2;
        readTmpTable.f=2;
        readTmpTable.n=0;
        readTmpTable.m=0;

        addACInTable(jpg->dhtAllMark.jpgDHTtables[indexID]->jpgDHTtreesRoots);

        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                tmpPicTables->jpgCr[i][j]=readTmpTable.x[i][j];
                readTmpTable.x[i][j]=0;
            }
        }
    }

    jpg->picMark.jpgAllPicTables.push_back(tmpPicTables);


    }

    //    вывод таблицы
    //    JPGALLPICTABLES *tmpP;
    //    tmpP=*jpg->picMark.jpgAllPicTables.begin();
    //    for(int i=0;i<8;i++){
    //        for(int j=0;j<8;j++)
    //           cout<<setw(3)<<tmpP->jpgCb[i][j];
    //        cout<<endl;
    //    }


    //    большой вывод всех таблиц

    JPGALLPICTABLES *tmpP;
    int ccounter=1;
        for(vector<JPGALLPICTABLES*>::iterator j = jpg->picMark.jpgAllPicTables.begin();j!=jpg->picMark.jpgAllPicTables.end();j++){

            tmpP=*j;
            cout<<"!!!!!!!!!!~ TABLES # "<<dec<<ccounter<<" ~!!!!!!!!!!!"<<endl;
            ccounter++;
            for(int ii=0;ii<4;ii++){
                cout<<"Y-table # "<<ii<<endl;
                for(int i=0;i<8;i++){
                    for(int jj=0;jj<8;jj++){}
//                        cout<<dec<<setw(3)<<tmpP->jpgYtable[ii][i][jj];
//                    cout<<endl/*<<setw(3)<<tmpP->jpgYtable[ii][0][0]*/;
                }
                cout<<setw(3)<<tmpP->jpgYtable[ii][0][0]<<endl;
//                cout<<endl;
            }

//                cout<<"Cb-table"<<endl;
//                for(int i=0;i<8;i++){
//                    for(int jj=0;jj<8;jj++)
//                        cout<<dec<<setw(3)<<tmpP->jpgCb[i][jj];
//                cout<<endl;
//                }

//                cout<<"Cr-table"<<endl;
//                for(int i=0;i<8;i++){
//                    for(int jj=0;jj<8;jj++)
//                        cout<<dec<<setw(3)<<tmpP->jpgCr[i][jj];
//                cout<<endl;
//                }

        }

}
