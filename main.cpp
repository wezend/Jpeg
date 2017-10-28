#include <iostream>
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

//FFFE
typedef struct{
    unsigned int jpgCommentLenght=0;
    string jpgCommentContent[15];
} JPGCOMMENT;

//FFDB
typedef struct
{
    unsigned int jpgDQTLenght[15];
    unsigned int jpgDQTLVT[15];
    unsigned int jpgDQTmaxTableID=0;
    unsigned int jpgDQTtable[15][7][7];

} JPGDQT;

//SOF0
typedef struct
{
    unsigned int jpgCompID=0;
    unsigned int jpgHorizontalDecimation=0;
    unsigned int jpgVerticalDecimation=0;
    unsigned int jpgQuantizationTableID=0;
} JPGCOMPONENT;

typedef struct
{
    unsigned int jpgSOF0Lenght=0;
    unsigned int jpgSOF0Precision=0;
    unsigned int jpgSOF0Height=0;
    unsigned int jpgSOF0Width=0;
    unsigned int jpgSOF0WComponentValue=0;
    JPGCOMPONENT jpgSOF0WComponents[15];

} JPGSOF0;

typedef struct
{
    unsigned int jpgDHTLenght=0;
    unsigned int jpgTableType=0;
    unsigned int jpgTableID=0;
    unsigned int jpgTableSize=0;
    pair<int,string> *jpgDHTtable;


} JPGDHT;

typedef struct
{
    JPGDHT  *jpgDHTtables[4];
} JPGDHTALL;

typedef struct
{
    unsigned int jpgSosLenght=0;
    unsigned int jpgSosComponentValue=0;
    pair<int,int> *jpgComponentsTable;

    unsigned int nonameBite1=0;
    unsigned int nonameBite2=0;
    unsigned int nonameBite3=0;

} JPGSOS;

static unsigned short read_u8(FILE *fp);
static unsigned short read_u16(FILE *fp);
static unsigned int   read_u32(FILE *fp);
static int            read_s32(FILE *fp);

    string dhtCode="";

    //рекурсивное добавление нового элемента в дерево Хаффмена.
    void makeTreeRec(int i, int dhtTmp,node* tmpNode,TREE *DHTtree){

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
    string DEC_TO_HEX(int st,int a)
    {
        int s;
        string str1,str2;
        while( st / 16){
            if(st % 16 > 10){
                switch (st % 16)
                        {
                        case 10:str1+='A'; break;
                        case 11:str1+='B'; break;
                        case 12:str1+='C'; break;
                        case 13:str1+='D'; break;
                        case 14:str1+='E'; break;
                        case 15:str1+='F'; break;
                }
                st=st/16;
                continue;
        }
            str1+=to_string(st % 16);

            st=st/16;

    }
        str1+=to_string(st % 16);
        while(str1.length() != a)
        {
         str1+="0";
        }
        for(int i = str1.length()-1; i >= 0; i--){
            str2+=str1[i];
        }

    return str2;
}

    int HEX_TO_DEC(string st)
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
    int* del(int a,int n){
        string str1=DEC_TO_HEX(a,n);
        string str2=str2+str1[0]+str1[1];
        cout<<"3";
        int *i=new int[2];
        i[0]=atoi(str2.c_str());
        str2=str2+str1[2]+str1[3];
        i[1]=atoi(str2.c_str());
        cout<<i[0]<<i[1];
        return i;
    }
    void write12(JPGDQT dqtMark,JPGSOF0 sof0Mark,JPGDHTALL dhtAllMark,JPGSOS sosMark){
        ofstream facin;
        facin.open("C:\\Users\\Wezend\\Downloads\\JPEG3\\JPEG\\test1.jpg");
        int * i= new int[2];
        i=del(dqtMark.jpgDQTLenght[0],4);
        //cout<<i[1];
        facin << "FFD8FFDB" << dqtMark.jpgDQTLVT <<dqtMark.jpgDQTmaxTableID <<dqtMark.jpgDQTtable ;


    }

int main()
{
    // "rb" побайтовое считывание
    FILE * pFile = fopen("C:\\Users\\Wezend\\Downloads\\JPEG3\\JPEG\\test.jpg", "rb");
    int lastMark;

    //тест на формат файла jpeg
    lastMark=read_u16(pFile);
    if (lastMark!=65496)
        {
            cout<<"error0"<<endl;
        }

    JPGCOMMENT comMark;
    //тест на наличие комментария
    lastMark=read_u16(pFile);
    if (lastMark!=65504)
        {
            cout<<"error1"<<endl;
        }

       //считывание комментариев
        int t=0;
        string sTmp;
        do{
    int s=0;
    comMark.jpgCommentLenght= read_u16(pFile);
    for(int i=0;i<comMark.jpgCommentLenght-2;i++)
    {
        s=read_u8(pFile);
       // cout<<hex<<s<<endl;
        sTmp+=s;

    }
    comMark.jpgCommentContent[t]=sTmp;
    t++;
    lastMark=read_u16(pFile);
        }while(lastMark==65504+t);

  //считывание таблицы квантования
   JPGDQT dqtMark;
   t=0;
   do{
    dqtMark.jpgDQTLenght[t]=read_u16(pFile);
    dqtMark.jpgDQTmaxTableID=t;

   int LVS=read_u8(pFile);
   if((LVS>>4)==1)
        dqtMark.jpgDQTLVT[t]=2;
   else
        dqtMark.jpgDQTLVT[t]=1;

    int x[8][8];
    int z=2;
    int k=1;
    int f=2;
    int n,m;
    n=m=0;
    x[n][m]=read_u8(pFile);

    do{
        if (f==0){
            if(n!=7)
                n++;
            else
                m++;
            f=1;
            x[n][m]=read_u8(pFile);
            z++;
            continue;
        }
        if(f==1){
            n--;
            m++;
            if(n==0 or m==7)
                {f=2;}
            x[n][m]=read_u8(pFile);
            z++;
            continue;
        }

        if (f==2){
            if(m!=7)
                m++;
            else
                n++;
            x[n][m]=read_u8(pFile);
            z++;
            f=3;
            continue;
        }
        if(f==3){
            n++;
            m--;
            if(m==0 or n==7)
                f=0;
            x[n][m]=read_u8(pFile);
            z++;
            continue;
        }
    }while(z!=65);

    t++;
    lastMark=read_u16(pFile);
   }while(lastMark==65499);




    //тест на наличие SOF0
    if (lastMark!=65472)
        {
            cout<<"error2"<<endl;
        }

    JPGSOF0 sof0Mark;

    //считывание SOF0
    sof0Mark.jpgSOF0Lenght=read_u16(pFile);
    sof0Mark.jpgSOF0Precision=read_u8(pFile);
    sof0Mark.jpgSOF0Height=read_u16(pFile);
    sof0Mark.jpgSOF0Width=read_u16(pFile);
    sof0Mark.jpgSOF0WComponentValue=read_u8(pFile);

    for(int i=0;i<sof0Mark.jpgSOF0WComponentValue;i++){

       sof0Mark.jpgSOF0WComponents[i].jpgCompID=read_u8(pFile);

       int decimation=read_u8(pFile);
       sof0Mark.jpgSOF0WComponents[i].jpgHorizontalDecimation=decimation>>4;
       sof0Mark.jpgSOF0WComponents[i].jpgVerticalDecimation=decimation&15;

       sof0Mark.jpgSOF0WComponents[i].jpgQuantizationTableID=read_u8(pFile);
    }

    //тест на наличие FFC4
    lastMark=read_u16(pFile);
    if (lastMark!=65476)
        {
            cout<<"error3"<<endl;
        }

    JPGDHTALL dhtAllMark;
    int dhtAllMarkCounter=0;
    //считывание FFC4
    do{

    JPGDHT *dhtMark=new JPGDHT;

    dhtMark->jpgDHTLenght=read_u16(pFile);
    int dhtTmp=read_u8(pFile);
    if((dhtTmp>>4)==1)
        dhtMark->jpgTableType=1;
    dhtMark->jpgTableID=dhtTmp&15;

    int dhtTmpArr[15];
    int dhtElCount=0;

    for(int i=0;i<16;i++){
        dhtTmpArr[i]=read_u8(pFile);
        dhtElCount+=dhtTmpArr[i];
    }

    dhtMark->jpgTableSize=dhtElCount;
    dhtMark->jpgDHTtable=new pair<int,string>[dhtElCount];
    int dhtTabeleCounter=0;

    TREE DHTtree;
    node* tmpNode;
    tmpNode=DHTtree.root;

    for(int i=0;i<16;i++){
        for(int j=0;j<dhtTmpArr[i];j++){
            dhtTmp=read_u8(pFile);
            dhtCode="";
            makeTreeRec(i,dhtTmp,tmpNode, &DHTtree);
            dhtMark->jpgDHTtable[dhtTabeleCounter].first=dhtTmp;
            dhtMark->jpgDHTtable[dhtTabeleCounter].second=dhtCode;
            dhtTabeleCounter++;
        }
    }

    dhtAllMark.jpgDHTtables[dhtAllMarkCounter]=dhtMark;
    dhtAllMarkCounter++;


    lastMark=read_u16(pFile);
    }while(lastMark==65476);

//     демонстрация работоспособности считывания FFC4
   /* for(int i=0;i<4;i++) {
        for(int j=0;j<dhtAllMark.jpgDHTtables[i]->jpgTableSize;j++)
        cout<<hex<<dhtAllMark.jpgDHTtables[i]->jpgDHTtable[j].first<<"; "<<dhtAllMark.jpgDHTtables[i]->jpgDHTtable[j].second<<endl;

        cout<<"~~~~~~~~~~~~~~"<<endl;
    }*/


    //тест на наличие FFDA
    if (lastMark!=65498)
        {
            cout<<"error4"<<endl;
        }

    JPGSOS sosMark;

    int sosTmp=0;
    sosMark.jpgSosLenght=read_u16(pFile);
    sosMark.jpgSosComponentValue=read_u8(pFile);
    sosMark.jpgComponentsTable=new pair<int,int>[sosMark.jpgSosComponentValue];
    for(int i=0;i<sosMark.jpgSosComponentValue;i++){
        sosTmp=read_u8(pFile);
        sosMark.jpgComponentsTable[i].first=sosTmp>>4;
        sosMark.jpgComponentsTable[i].second=sosTmp&15;
    }
    sosMark.nonameBite1=read_u16(pFile);
    sosMark.nonameBite2=read_u16(pFile);
    sosMark.nonameBite3=read_u16(pFile);

    //cout<<hex<<read_u16(pFile);




    write12(dqtMark,sof0Mark,dhtAllMark,sosMark);


  return 0;
}


static unsigned short read_u8(FILE *fp)
{
    unsigned char b0;

    b0 = getc(fp);

    return b0;
}


static unsigned short read_u16(FILE *fp)
{
    unsigned char b0, b1;

    b1 = getc(fp);
    b0 = getc(fp);


    return ( (b1 << 8)|b0);
}


static unsigned int read_u32(FILE *fp)
{
    unsigned char b0, b1, b2, b3;

    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);

    return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}


static int read_s32(FILE *fp)
{
    unsigned char b0, b1, b2, b3;

    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);

    return ((int)(((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}


/*
    while((i<8)&&(j<8)){
        if (f==0){
            if(i!=7)
                i+=1;
            else
                j+=1;
            x[i][j]=z;
            z++;
            f=1;
        }
        if(f==1){

            i--;
            j++;
            if(i==0 or j==7)
                {f=2;}
            x[i][j]=z;
            z++;
        }
        if (f==2){
            if(j!=7)
                j++;
            else
                i++;
            x[i][j]=z;
            z++;
            f=3;
        }
        if(f==3){
            i++;
            j--;
            if(j==0 or i==7)
                f=0;
            x[i][j]=z;
            z++;
        }
    }
     for(int i=0;i<8;i++){
    for(int j=0;j<8;j++){
        cout << x[i][j] <<" ";
    }
    cout<<endl;
    }

       // cout<<hex<<read_u16(pFile);

    t++;*/

   //The length of the values in the table(LVT)



    /*tableID
    dqtMark.jpgDQTtableID=getc(pFile);

    int x[8][8];
    int t=1;
    int k=1;
    for(int i=0;i<8;i++){
    for(int j=0;j<8;j++){
        x[i][j]=t;
        t++;
        cout << x[i][j] <<" ";
    }
    cout<<endl;
    }
    cout<<"20!8"<<endl;
    int i,j;
    i=j=0;
    t=2;
    int f=2;
    x[i][j]=1;
    while((i<8)&&(j<8)){
        if (f==0){
            if(i!=7)
                i+=1;
            else
                j+=1;
            x[i][j]=t;
            t++;
            f=1;
        }
        if(f==1){

            i--;
            j++;
            if(i==0 or j==7)
                {f=2;}
            x[i][j]=t;
            t++;
        }
        if (f==2){
            if(j!=7)
                j++;
            else
                i++;
            x[i][j]=t;
            t++;
            f=3;
        }
        if(f==3){
            i++;
            j--;
            if(j==0 or i==7)
                f=0;
            x[i][j]=t;
            t++;
        }
    }


    //считать матрицу 8 на 8
*/




     // char tttt=getc(pFile);
     // cout<<tttt<<"!123456"<<endl;
     //int st=0;
     //st=read_u16(pFile);
     //cout<<hex<<st<<endl;
     //st=read_u8(pFile);
     //cout<<hex<<st<<endl;
     //cout<<hex<<read_u16(pFile)<<endl;
     //cout<<hex<<read_u16(pFile)<<endl;
     //cout<<hex<<read_u16(pFile)<<endl;
     //cout<<hex<<read_u16(pFile)<<endl;
     //st=read_u16(pFile);
     //cout<<hex<<st;
     //st=read_u16(pFile);
     //cout<<hex<<st;
    // if (read_u16(pFile)!=65496)


