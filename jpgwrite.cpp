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
#include <iomanip>
#include <bitset>


using namespace std;

//please, don.t use it. Everywhere;
string dhtCode2="";

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

    o[0]=HEX_TO_DEC("FF");
    o[1]=HEX_TO_DEC("D9");
    fwrite(&o, 2,1,f1);

    cout<<"to doooo"<<endl;



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

    //вызвать тут
    makeHaffmanTree();

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

        //тут
//        for(list<char>::iterator f=jpg->picMark.jpgPicture.begin();
//            f!=jpg->picMark.jpgPicture.end();f++){
//            char c=*f;
//            cout<<c<<" ";
//        }
//        cout<<"~~~~~~~~~~~~~~~~~~~~~~~"<<endl;

        jpg->picMark.jpgPicture.clear();
        codeJpg();

//        for(list<char>::iterator f=jpg->picMark.jpgPicture.begin();
//            f!=jpg->picMark.jpgPicture.end();f++){
//            char c=*f;
//            cout<<c<<" ";
//        }
//        cout<<endl;



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

}

int JPGWrite::tableElementLenth(int originalValue){

//    cout<<value<<"#"<<endl;
    int value=originalValue;

    int lenth=0;
    int tmpBit=0;
    if(value<0)
        value*=-1;

    char curretByte=value;

    for(int i=0;i<8;i++){
        tmpBit=(curretByte >> i) & 0x01;
//        cout<<tmpBit<<" ";
        if(tmpBit==1)
                lenth=i+1;

    }
//    cout<<endl;

        return lenth;

//    cout<<dec<<lenth<<endl;
}

void JPGWrite::addInNodeVector(int value){

    node* tmpNode;
    bool newNodeTrigger=1;
//    cout<<tableElementLenth(value)<<endl;
    for(vector<node*>::iterator f=nodeWeightList.begin();f!=nodeWeightList.end();f++){
        tmpNode=*f;

        if(tmpNode->Key == tableElementLenth(value) ){
            tmpNode->weight+=1;
//            cout<<"Add in node weight "<<tmpNode->Key<<" "<<tmpNode->weight<<endl;
            return;
        }
    }

    //если нет ноды, то создаётся новая(вообще здесь if не нужен, но так нагляднее)
    if(newNodeTrigger){

//                cout<<tmpP->jpgYtable[i][0][0]<<"!"<<endl;

        node* newNode=new node;
        newNode->Key= tableElementLenth( value );
        newNode->weight=1;
        nodeWeightList.push_back(newNode);

//        cout<<"New node "<<newNode->Key<<endl;

    }
    return;
}

void JPGWrite::addInACNodeVector(int zeroCount, int value){

    int nodeKey=0;
    int tmpNodeKey=0;



//    if(zeroCount>15)
//        cout<<dec<<zeroCount<<" "<<dec<<value<<endl;



    nodeKey=zeroCount<<4 | tableElementLenth(value);
//    cout<<hex<<nodeKey<<endl;

    node* tmpNode;

    for(vector<node*>::iterator f=nodeWeightList.begin();f!=nodeWeightList.end();f++){
        tmpNode=*f;
        tmpNodeKey=tmpNode->Key;
        if(tmpNodeKey>>4 == zeroCount)
            if((tmpNodeKey&15) == tableElementLenth(value)){
            tmpNode->weight+=1;
            return;
//            cout<<"Add in node weight "<<tmpNode->Key<<" "<<tmpNode->weight<<endl;
        }
    }

    //если нет ноды, то создаётся новая

        node* newACNode=new node;
        newACNode->Key= nodeKey;
        newACNode->weight=1;
        nodeWeightList.push_back(newACNode);
//        cout<<"New node "<<newNode->Key<<endl;
    return;
}

void JPGWrite::addElInNodeLIST(node* tmpNode){
    for(list<node*>::iterator f=nodeWeightLIST.begin();f!=nodeWeightLIST.end();f++){
         node* vecNode=*f;
         if(vecNode->weight>tmpNode->weight){
             nodeWeightLIST.emplace(f,tmpNode);
             return;
         }
    }
    nodeWeightLIST.push_back(tmpNode);
}

void JPGWrite::makeTree(){

    //строим дерево!
    //итератор для прохода по вектору (теперь всё буферизируем в лист,
    //ибо в векторах ломается ).
    //основной смысл: всё дерево расположено в векторе.
    //новый элемент образуется из 2х самых маловесящих.
    //кроме первого. первый получает предка и вставляется в назад лист
    //сортировка и добавление проиходит не через sort, а с помощью метода addElInNodeVector()
    //добавляем пока вес какого либо элемента не станет равен
    //весу всех изначальных элементов.(это будет корень)

    //создаём двусвязный список nodeWeightLIST
    //обычный лист на самом деле вектор. Вот так вот((
    unsigned long int totalWeight=0;

    //заполняем его
    for(vector<node*>::iterator f=nodeWeightList.begin();f!=nodeWeightList.end();f++){
         node* tmpNode=*f;
         totalWeight+=tmpNode->weight;
         nodeWeightLIST.push_back(tmpNode);
    }

    list<node*>::iterator listIter=nodeWeightLIST.begin();

    node* firstNode=new node;
    node* tN=*listIter;

    firstNode->Left=tN;
    firstNode->weight=tN->weight;
    firstNode->Key=-2;

    addElInNodeLIST(firstNode);
    listIter++;
//    if(*listIter==firstNode)
//        return;

    unsigned long int tmpTotalWeight=0;
    while(true){

         node* tmpNodeFirst=*listIter;
         tmpTotalWeight+=tmpNodeFirst->weight;

         listIter++;
         if(listIter==nodeWeightLIST.end()){
             jpgTmpRoot=tmpNodeFirst;
             break;
         }
         node* tmpNodeSecond=*listIter;
         tmpTotalWeight+=tmpNodeSecond->weight;

         node* tmpFatherNode=new node;

         tmpFatherNode->Key=-2;
         tmpFatherNode->weight=tmpNodeFirst->weight + tmpNodeSecond->weight;
         tmpFatherNode->Left=tmpNodeFirst;
         tmpFatherNode->Right=tmpNodeSecond;
         tmpNodeFirst->Back=tmpFatherNode;
         tmpNodeSecond->Back=tmpFatherNode;

         addElInNodeLIST(tmpFatherNode);

         if(tmpFatherNode->weight==totalWeight){
             jpgTmpRoot=tmpFatherNode;
             break;
         }


         listIter++;

    }


    //вывод дерева
//    cout<<endl;
//    int c=1;
//    for(list<node*>::iterator f=nodeWeightLIST.begin();f!=nodeWeightLIST.end();f++){
//         node* tmpNode=*f;
//         __int16 itt=tmpNode->Key;
//         cout<<c<<") "<<dec<<itt<<" x "<<dec<<tmpNode->weight<<endl;
//         c++;
//    }

}

void JPGWrite::recTreeValk(node *root,int *lenth){

    //рекурсивный обход дерева с целью создания строки,
    //как в JPG чтобы можно было сформировать корректное
    //дерево Хаффмана для кодирования изображения.

    if(root->Left!=0){
        *lenth+=1;
        recTreeValk(root->Left,lenth);
    }
    if(root->Right!=0){
        *lenth+=1;
        recTreeValk(root->Right,lenth);
    }
    if(root->Key!=-2){
        pair<int,string> tmpPair;
        tmpPair.first=root->Key;
        tmpPair.second=to_string(*lenth);
        jpgTmpDHTtable.push_back(tmpPair);
    }
    *lenth-=1;

}

void JPGWrite::treeValk(node *root){

    //запуск рекурсивного обхода

    int lenth=0;
    recTreeValk(root,&lenth);

//    int c=1;
//    for(vector<pair<int,string>>::iterator f=jpgTmpDHTtable.begin();f!=jpgTmpDHTtable.end();f++){
//        pair<int,string> tmpPair=*f;
//        cout<<dec<<c<<") "<<hex<<tmpPair.first<<" "<<tmpPair.second<<endl;
//    }

}

void JPGWrite::makeTreeRec(int i, int dhtTmp, node *tmpNode, TREE *DHTtree){

    if (tmpNode->Left !=0 and tmpNode->Right !=0 and tmpNode->Left->Key != -1 and tmpNode->Right->Key != -1){
    tmpNode->Key = -2;
    dhtCode2.erase(dhtCode2.length()-1,1);
    makeTreeRec(i,dhtTmp,tmpNode->Back,DHTtree);
    }
    if(tmpNode->Left==0){
    if(tmpNode->Lvl==i){
    tmpNode->Left=DHTtree->addNode(dhtTmp,(tmpNode->Lvl+1),tmpNode);
    dhtCode2=dhtCode2+"0";

    }else {
    tmpNode->Left=DHTtree->addNode(-1,(tmpNode->Lvl+1),tmpNode);
    dhtCode2+="0";
    makeTreeRec(i,dhtTmp,tmpNode->Left,DHTtree);
    }
    }else if (tmpNode->Left->Key==-1) {
    dhtCode2+="0";
    makeTreeRec(i,dhtTmp,tmpNode->Left,DHTtree);
    }else if(tmpNode->Right==0){
    if(tmpNode->Lvl==i){
    tmpNode->Right=DHTtree->addNode(dhtTmp,(tmpNode->Lvl+1),tmpNode);
    dhtCode2+="1";
    }else {
    tmpNode->Right=DHTtree->addNode(-1,(tmpNode->Lvl+1),tmpNode);
    dhtCode2+="1";
    makeTreeRec(i,dhtTmp,tmpNode->Right,DHTtree); }
    }else if (tmpNode->Right->Key==-1) {
    dhtCode2+="1";
    makeTreeRec(i,dhtTmp,tmpNode->Right,DHTtree);
    }

}

void JPGWrite::makeHaffmanTree(){

    //основаная функция создания деревьев Хаффмана

    //DC0 и AC0 деревья для Y коэффичиентов
    //DC1 и AC1 для Cr и Cb.

    JPGALLPICTABLES *tmpP;

    cout<<"~~~~~ DC0 begin ~~~~~~"<<endl;

    for(vector<JPGALLPICTABLES*>::iterator j = jpg->picMark.jpgAllPicTables.begin();j!=jpg->picMark.jpgAllPicTables.end();j++){
        tmpP=*j;
        for(int ii=3;ii>0;ii--){
             tmpP->jpgYtable[ii][0][0]=tmpP->jpgYtable[ii][0][0] - tmpP->jpgYtable[ii-1][0][0];
        }

    }

    for(vector<JPGALLPICTABLES*>::iterator j = jpg->picMark.jpgAllPicTables.begin();j!=jpg->picMark.jpgAllPicTables.end();j++){
        tmpP=*j;

        for(int i=0;i<4;i++){
            addInNodeVector(tmpP->jpgYtable[i][0][0]);
        }

    }

    sort(nodeWeightList.begin(),nodeWeightList.end(),[](const node* a, const node* b){
        return a->weight < b->weight;
    });

    //вывод Y коэффициентов DC0 дерева Хаффмана и их вероятноестей
//    for(vector<node*>::iterator f=nodeWeightList.begin();f!=nodeWeightList.end();f++){
//         node* tmpNode=*f;
//         __int16 itt=tmpNode->Key&255;
//         cout<<dec<<itt<<" x "<<dec<<tmpNode->weight<<endl;
//    }

    makeTree();
    treeValk(jpgTmpRoot);


    int lenthTable[16];
    for(int i=0;i<16;i++){
        lenthTable[i]=0;
    }

    for(int i=0;i<16;i++){
        for(vector<pair<int,string>>::iterator f=jpgTmpDHTtable.begin();f!=jpgTmpDHTtable.end();f++){
            pair<int,string> tmpPair=*f;
            string str=tmpPair.second;
            int t=atoi(str.c_str());
            if(t==i+1){
                lenthTable[i]+=1;
            }

        }

    }

    int dhtElCount=0;

    for(int i=0;i<16;i++){
//      вывод дерева Хаффмана в виде строки как в JPG
      cout<<lenthTable[i]<<" ";
        dhtElCount+=lenthTable[i];
    }
    cout<<endl;

    vector<node*> reverseNodeVector2;
    for(vector<node*>::iterator f=nodeWeightList.begin();f!=nodeWeightList.end();f++){
        reverseNodeVector2.emplace(reverseNodeVector2.begin(),*f);
    }



    int dhtTabeleCounter=0;
    TREE DHTtree;
    node *tmpNode;
    tmpNode=DHTtree.root;
    int dhtTmp;
    int dhtAllMarkCounter=0;
    vector<node*>::iterator nodeWeightListIterator=reverseNodeVector2.begin();

    delete [] jpg->dhtAllMark.jpgDHTtables[0]->jpgDHTtable;
     jpg->dhtAllMark.jpgDHTtables[0]->jpgDHTtable=new pair<int,string>[dhtElCount];

    for(int i=0;i<16;i++){
        for(int j=0;j<lenthTable[i];j++){
            node* tNode=*nodeWeightListIterator;
            nodeWeightListIterator++;
            dhtTmp=tNode->Key;
            dhtCode2="";
            makeTreeRec(i,dhtTmp,tmpNode, &DHTtree);
            jpg->dhtAllMark.jpgDHTtables[0]->jpgDHTtable[dhtTabeleCounter].first=dhtTmp;
            jpg->dhtAllMark.jpgDHTtables[0]->jpgDHTtable[dhtTabeleCounter].second=dhtCode2;

            dhtTabeleCounter++;
        }
    }

    jpg->dhtAllMark.jpgDHTtables[dhtAllMarkCounter]->jpgDHTtreesRoots=DHTtree.root;
    dhtAllMarkCounter++;

    int cc=1;

    //конечный результат. Вывод полученного дерева Хаффмана.
    for(int j=0;j<jpg->dhtAllMark.jpgDHTtables[0]->jpgTableSize;j++){
    cout<<dec<<setw(3)<<cc<<") "<<hex<<setw(3)<<jpg->dhtAllMark.jpgDHTtables[0]->jpgDHTtable[j].first<<"; "<<jpg->dhtAllMark.jpgDHTtables[0]->jpgDHTtable[j].second<<endl;
    cc++;
    }

    cout<<"~~~~~ DC0 end ~~~~~~"<<endl;

    //очищаем для создания АС0 дерева
    nodeWeightList.clear();
    nodeWeightLIST.clear();
    jpgTmpDHTtable.clear();


    cout<<"~~~~~ AC0 begin ~~~~~~"<<endl;

    //основной алгоритм считывания элементов в nodeWeightList
    for(vector<JPGALLPICTABLES*>::iterator j = jpg->picMark.jpgAllPicTables.begin();j!=jpg->picMark.jpgAllPicTables.end();j++){
        tmpP=*j;

        for(int i=0;i<4;i++){
            int zeroCounter=0;
            bool zeroTrigger=1;
            bool lastZeroTrigger=0;

            int z=2;
            int f=2;
            int n,m;
            n=m=0;

            do{

                if (f==0){
                    if(n!=7)
                        n++;
                    else
                        m++;
                    f=1;

                    if(tmpP->jpgYtable[i][n][m]==0){
                        zeroCounter++;

                        if(zeroCounter==15 && zeroTrigger){
                            if(tmpP->jpgYtable[i][n-1][m+1]==0){
                                addInACNodeVector(zeroCounter,0);
                            zeroCounter=0;
                            zeroTrigger=0;
                            lastZeroTrigger=1;
                            }

                        }

                    }else {
                        addInACNodeVector(zeroCounter,tmpP->jpgYtable[i][n][m]);
                        zeroCounter=0;
                        if(zeroTrigger==0){
                            zeroTrigger=1;
                            lastZeroTrigger=0;
                        }
                    }

                    z++;
                    continue;
                }
                if(f==1){
                    n--;
                    m++;
                    if(n==0 or m==7)
                        {f=2;}

                    if(tmpP->jpgYtable[i][n][m]==0){
                        zeroCounter++;

                        if(zeroCounter==15 && zeroTrigger){
                            if(f==2){
                            if(m!=7){
                               if(tmpP->jpgYtable[i][n][m+1]==0){
                                addInACNodeVector(zeroCounter,0);
                            zeroCounter=0;
                            zeroTrigger=0;
                            lastZeroTrigger=1;
                               }
                            }else{
                                if(tmpP->jpgYtable[i][n+1][m]==0){
                                 addInACNodeVector(zeroCounter,0);
                             zeroCounter=0;
                             zeroTrigger=0;
                             lastZeroTrigger=1;
                                }
                            }
                            }else{
                                if(tmpP->jpgYtable[i][n-1][m+1]==0){
                                 addInACNodeVector(zeroCounter,0);
                             zeroCounter=0;
                             zeroTrigger=0;
                             lastZeroTrigger=1;
                                }
                            }
                        }

                    }else {
                        addInACNodeVector(zeroCounter,tmpP->jpgYtable[i][n][m]);
                        zeroCounter=0;
                        if(zeroTrigger==0){
                            zeroTrigger=1;
                            lastZeroTrigger=0;
                        }
                    }


                    z++;
                    continue;
                }

                if (f==2){
                    if(m!=7)
                        m++;
                    else
                        n++;

                    if(tmpP->jpgYtable[i][n][m]==0){
                        zeroCounter++;

                        if(zeroCounter==15 && zeroTrigger){
                            if(tmpP->jpgYtable[i][n+1][m-1]==0){
                                addInACNodeVector(zeroCounter,0);
                            zeroCounter=0;
                            zeroTrigger=0;
                            lastZeroTrigger=1;
                            }

                        }

                    }else {
                        addInACNodeVector(zeroCounter,tmpP->jpgYtable[i][n][m]);
                        zeroCounter=0;
                        if(zeroTrigger==0){
                            zeroTrigger=1;
                            lastZeroTrigger=0;
                        }
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


                    if(tmpP->jpgYtable[i][n][m]==0){
                        zeroCounter++;

                        if(zeroCounter==15 && zeroTrigger){
                            if(f==0){
                            if(n!=7){
                               if(tmpP->jpgYtable[i][n+1][m]==0){
                                addInACNodeVector(zeroCounter,0);
                            zeroCounter=0;
                            zeroTrigger=0;
                            lastZeroTrigger=1;
                               }
                            }else{
                                if(tmpP->jpgYtable[i][n][m+1]==0){
                                 addInACNodeVector(zeroCounter,0);
                             zeroCounter=0;
                             zeroTrigger=0;
                             lastZeroTrigger=1;
                                }
                            }
                            }else{
                                if(tmpP->jpgYtable[i][n+1][m-1]==0){
                                 addInACNodeVector(zeroCounter,0);
                             zeroCounter=0;
                             zeroTrigger=0;
                             lastZeroTrigger=1;
                                }
                            }
                        }

                    }else {
                        addInACNodeVector(zeroCounter,tmpP->jpgYtable[i][n][m]);
                        zeroCounter=0;
                        if(zeroTrigger==0){
                            zeroTrigger=1;
                            lastZeroTrigger=0;
                        }
                    }


                    z++;
                    continue;
                }



            }while(z!=65);

            if(lastZeroTrigger){
                for(vector<node*>::iterator f=nodeWeightList.begin();f!=nodeWeightList.end();f++){
                     node* tmpNode=*f;
                     if(tmpNode->Key==240)
                         tmpNode->weight-=1;
                }

            }


            if(zeroCounter!=0)
                addInACNodeVector(0,0);
        }

    }

    for(vector<node*>::iterator f=nodeWeightList.begin();f!=nodeWeightList.end();f++){
         node* tmpNode=*f;
         if(tmpNode->weight==0){
             nodeWeightList.erase(f);
             break;
         }
    }

    //сортировка по возрастанию
    sort(nodeWeightList.begin(),nodeWeightList.end(),[](const node* a, const node* b){
        return a->weight < b->weight;
    });

    //вывод отсортированного вектора
    //вывод Y коэффициентов AC0 дерева Хаффмана и их вероятноестей
//    int c=1;
//    for(vector<node*>::iterator f=nodeWeightList.begin();f!=nodeWeightList.end();f++){
//         node* tmpNode=*f;
//         __int16 itt=tmpNode->Key;
//         cout<<c<<") "<<dec<<itt<<" x "<<dec<<tmpNode->weight<<endl;
//         c++;
//    }

    makeTree();
    treeValk(jpgTmpRoot);

    for(int i=0;i<16;i++){
        lenthTable[i]=0;
    }

    for(int i=0;i<16;i++){
        for(vector<pair<int,string>>::iterator f=jpgTmpDHTtable.begin();f!=jpgTmpDHTtable.end();f++){
            pair<int,string> tmpPair=*f;
            string str=tmpPair.second;
            int t=atoi(str.c_str());
            if(t==i+1){
                lenthTable[i]+=1;
            }

        }

    }


    dhtElCount=0;
    for(int i=0;i<16;i++){
        cout<<lenthTable[i]<<" ";
        dhtElCount+=lenthTable[i];
    }
    cout<<endl;

    vector<node*> reverseNodeVector;
    for(vector<node*>::iterator f=nodeWeightList.begin();f!=nodeWeightList.end();f++){
        reverseNodeVector.emplace(reverseNodeVector.begin(),*f);
    }



    dhtTabeleCounter=0;
    TREE DHTtree2;
    tmpNode=DHTtree2.root;
    dhtTmp=0;
    nodeWeightListIterator=reverseNodeVector.begin();

    delete [] jpg->dhtAllMark.jpgDHTtables[1]->jpgDHTtable;
    jpg->dhtAllMark.jpgDHTtables[1]->jpgDHTtable=new pair<int,string>[dhtElCount];

    for(int i=0;i<16;i++){
        for(int j=0;j<lenthTable[i];j++){
            node* tNode=*nodeWeightListIterator;
            nodeWeightListIterator++;
            dhtTmp=tNode->Key;
            dhtCode2="";
            makeTreeRec(i,dhtTmp,tmpNode, &DHTtree2);
            jpg->dhtAllMark.jpgDHTtables[1]->jpgDHTtable[dhtTabeleCounter].first=dhtTmp;
            jpg->dhtAllMark.jpgDHTtables[1]->jpgDHTtable[dhtTabeleCounter].second=dhtCode2;

            dhtTabeleCounter++;
        }
    }


    jpg->dhtAllMark.jpgDHTtables[dhtAllMarkCounter]->jpgDHTtreesRoots=DHTtree2.root;
    dhtAllMarkCounter++;

    cc=1;
    //вывод конечного дерева Хаффмана
    for(int j=0;j<jpg->dhtAllMark.jpgDHTtables[1]->jpgTableSize;j++){
    cout<<dec<<setw(3)<<cc<<") "<<hex<<setw(3)<<jpg->dhtAllMark.jpgDHTtables[1]->jpgDHTtable[j].first<<"; "<<jpg->dhtAllMark.jpgDHTtables[1]->jpgDHTtable[j].second<<endl;
    cc++;
    }

    cout<<"~~~~~ AC0 end ~~~~~~"<<endl;

    //очищаем для создания DC1 дерева
    nodeWeightList.clear();
    nodeWeightLIST.clear();
    jpgTmpDHTtable.clear();


    cout<<"~~~~~ DC1 begin ~~~~~~"<<endl;

    for(vector<JPGALLPICTABLES*>::iterator j = jpg->picMark.jpgAllPicTables.begin();j!=jpg->picMark.jpgAllPicTables.end();j++){
        tmpP=*j;
        addInNodeVector(tmpP->jpgCb[0][0]);
        addInNodeVector(tmpP->jpgCr[0][0]);
    }

    sort(nodeWeightList.begin(),nodeWeightList.end(),[](const node* a, const node* b){
        return a->weight < b->weight;
    });

//    for(vector<node*>::iterator f=nodeWeightList.begin();f!=nodeWeightList.end();f++){
//         node* tmpNode=*f;
//         __int16 itt=tmpNode->Key&255;
//         cout<<dec<<itt<<" x "<<dec<<tmpNode->weight<<endl;
//    }

    makeTree();
    treeValk(jpgTmpRoot);

    for(int i=0;i<16;i++){
        lenthTable[i]=0;
    }

    for(int i=0;i<16;i++){
        for(vector<pair<int,string>>::iterator f=jpgTmpDHTtable.begin();f!=jpgTmpDHTtable.end();f++){
            pair<int,string> tmpPair=*f;
            string str=tmpPair.second;
            int t=atoi(str.c_str());
            if(t==i+1){
                lenthTable[i]+=1;
            }

        }
    }


    dhtElCount=0;
    for(int i=0;i<16;i++){
        cout<<lenthTable[i]<<" ";
        dhtElCount+=lenthTable[i];
    }
    jpg->dhtAllMark.jpgDHTtables[2]->jpgTableSize=dhtElCount;

    vector<node*> reverseNodeVector3;
    for(vector<node*>::iterator f=nodeWeightList.begin();f!=nodeWeightList.end();f++){
        reverseNodeVector3.emplace(reverseNodeVector3.begin(),*f);
    }

    dhtTabeleCounter=0;
    TREE DHTtree3;
    tmpNode=DHTtree3.root;
    dhtTmp=0;
    nodeWeightListIterator=reverseNodeVector3.begin();

    delete [] jpg->dhtAllMark.jpgDHTtables[2]->jpgDHTtable;
     jpg->dhtAllMark.jpgDHTtables[2]->jpgDHTtable=new pair<int,string>[dhtElCount];



     for(int i=0;i<16;i++){
         for(int j=0;j<lenthTable[i];j++){
             node* tNode=*nodeWeightListIterator;
             nodeWeightListIterator++;
             dhtTmp=tNode->Key;
             dhtCode2="";
             makeTreeRec(i,dhtTmp,tmpNode, &DHTtree3);
             jpg->dhtAllMark.jpgDHTtables[2]->jpgDHTtable[dhtTabeleCounter].first=dhtTmp;
             jpg->dhtAllMark.jpgDHTtables[2]->jpgDHTtable[dhtTabeleCounter].second=dhtCode2;

             dhtTabeleCounter++;
         }
     }


     jpg->dhtAllMark.jpgDHTtables[dhtAllMarkCounter]->jpgDHTtreesRoots=DHTtree3.root;
     dhtAllMarkCounter++;

     cc=1;
     for(int j=0;j<jpg->dhtAllMark.jpgDHTtables[2]->jpgTableSize;j++){
     cout<<dec<<setw(3)<<cc<<") "<<hex<<setw(3)<<jpg->dhtAllMark.jpgDHTtables[2]->jpgDHTtable[j].first<<"; "<<jpg->dhtAllMark.jpgDHTtables[2]->jpgDHTtable[j].second<<endl;
     cc++;
     }

     cout<<"~~~~~ DC1 end ~~~~~~"<<endl;

     //очищаем для создания AC1 дерева
     nodeWeightList.clear();
     nodeWeightLIST.clear();
     jpgTmpDHTtable.clear();

     cout<<"~~~~~ AC1 begin ~~~~~~"<<endl;

     //основной алгоритм считывания элементов в nodeWeightList

     //для Cr коэффициентов
     for(vector<JPGALLPICTABLES*>::iterator j = jpg->picMark.jpgAllPicTables.begin();j!=jpg->picMark.jpgAllPicTables.end();j++){
         tmpP=*j;
             int zeroCounter=0;
             bool zeroTrigger=1;
             bool lastZeroTrigger=0;

             int z=2;
             int f=2;
             int n,m;
             n=m=0;

             do{

                 if (f==0){
                     if(n!=7)
                         n++;
                     else
                         m++;
                     f=1;

                     if(tmpP->jpgCr[n][m]==0){
                         zeroCounter++;

                         if(zeroCounter==15 && zeroTrigger){
                             if(tmpP->jpgCr[n-1][m+1]==0){
                                 addInACNodeVector(zeroCounter,0);
                             zeroCounter=0;
                             zeroTrigger=0;
                             lastZeroTrigger=1;
                             }

                         }

                     }else {
                         addInACNodeVector(zeroCounter,tmpP->jpgCr[n][m]);
                         zeroCounter=0;
                         if(zeroTrigger==0){
                             zeroTrigger=1;
                             lastZeroTrigger=0;
                         }
                     }

                     z++;
                     continue;
                 }
                 if(f==1){
                     n--;
                     m++;
                     if(n==0 or m==7)
                         {f=2;}

                     if(tmpP->jpgCr[n][m]==0){
                         zeroCounter++;

                         if(zeroCounter==15 && zeroTrigger){
                             if(f==2){
                             if(m!=7){
                                if(tmpP->jpgCr[n][m+1]==0){
                                 addInACNodeVector(zeroCounter,0);
                             zeroCounter=0;
                             zeroTrigger=0;
                             lastZeroTrigger=1;
                                }
                             }else{
                                 if(tmpP->jpgCr[n+1][m]==0){
                                  addInACNodeVector(zeroCounter,0);
                              zeroCounter=0;
                              zeroTrigger=0;
                              lastZeroTrigger=1;
                                 }
                             }
                             }else{
                                 if(tmpP->jpgCr[n-1][m+1]==0){
                                  addInACNodeVector(zeroCounter,0);
                              zeroCounter=0;
                              zeroTrigger=0;
                              lastZeroTrigger=1;
                                 }
                             }
                         }

                     }else {
                         addInACNodeVector(zeroCounter,tmpP->jpgCr[n][m]);
                         zeroCounter=0;
                         if(zeroTrigger==0){
                             zeroTrigger=1;
                             lastZeroTrigger=0;
                         }
                     }


                     z++;
                     continue;
                 }

                 if (f==2){
                     if(m!=7)
                         m++;
                     else
                         n++;

                     if(tmpP->jpgCr[n][m]==0){
                         zeroCounter++;

                         if(zeroCounter==15 && zeroTrigger){
                             if(tmpP->jpgCr[n+1][m-1]==0){
                                 addInACNodeVector(zeroCounter,0);
                             zeroCounter=0;
                             zeroTrigger=0;
                             lastZeroTrigger=1;
                             }

                         }

                     }else {
                         addInACNodeVector(zeroCounter,tmpP->jpgCr[n][m]);
                         zeroCounter=0;
                         if(zeroTrigger==0){
                             zeroTrigger=1;
                             lastZeroTrigger=0;
                         }
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


                     if(tmpP->jpgCr[n][m]==0){
                         zeroCounter++;

                         if(zeroCounter==15 && zeroTrigger){
                             if(f==0){
                             if(n!=7){
                                if(tmpP->jpgCr[n+1][m]==0){
                                 addInACNodeVector(zeroCounter,0);
                             zeroCounter=0;
                             zeroTrigger=0;
                             lastZeroTrigger=1;
                                }
                             }else{
                                 if(tmpP->jpgCr[n][m+1]==0){
                                  addInACNodeVector(zeroCounter,0);
                              zeroCounter=0;
                              zeroTrigger=0;
                              lastZeroTrigger=1;
                                 }
                             }
                             }else{
                                 if(tmpP->jpgCr[n+1][m-1]==0){
                                  addInACNodeVector(zeroCounter,0);
                              zeroCounter=0;
                              zeroTrigger=0;
                              lastZeroTrigger=1;
                                 }
                             }
                         }

                     }else {
                         addInACNodeVector(zeroCounter,tmpP->jpgCr[n][m]);
                         zeroCounter=0;
                         if(zeroTrigger==0){
                             zeroTrigger=1;
                             lastZeroTrigger=0;
                         }
                     }


                     z++;
                     continue;
                 }



             }while(z!=65);

             if(lastZeroTrigger){
                 for(vector<node*>::iterator f=nodeWeightList.begin();f!=nodeWeightList.end();f++){
                      node* tmpNode=*f;
                      if(tmpNode->Key==240)
                          tmpNode->weight-=1;
                 }

             }


             if(zeroCounter!=0)
                 addInACNodeVector(0,0);


     }

     for(vector<node*>::iterator f=nodeWeightList.begin();f!=nodeWeightList.end();f++){
          node* tmpNode=*f;
          if(tmpNode->weight==0){
              nodeWeightList.erase(f);
              break;
          }
     }

     //для Cb коэффициентов
     for(vector<JPGALLPICTABLES*>::iterator j = jpg->picMark.jpgAllPicTables.begin();j!=jpg->picMark.jpgAllPicTables.end();j++){
         tmpP=*j;
             int zeroCounter=0;
             bool zeroTrigger=1;
             bool lastZeroTrigger=0;

             int z=2;
             int f=2;
             int n,m;
             n=m=0;

             do{

                 if (f==0){
                     if(n!=7)
                         n++;
                     else
                         m++;
                     f=1;

                     if(tmpP->jpgCb[n][m]==0){
                         zeroCounter++;

                         if(zeroCounter==15 && zeroTrigger){
                             if(tmpP->jpgCb[n-1][m+1]==0){
                                 addInACNodeVector(zeroCounter,0);
                             zeroCounter=0;
                             zeroTrigger=0;
                             lastZeroTrigger=1;
                             }

                         }

                     }else {
                         addInACNodeVector(zeroCounter,tmpP->jpgCb[n][m]);
                         zeroCounter=0;
                         if(zeroTrigger==0){
                             zeroTrigger=1;
                             lastZeroTrigger=0;
                         }
                     }

                     z++;
                     continue;
                 }
                 if(f==1){
                     n--;
                     m++;
                     if(n==0 or m==7)
                         {f=2;}

                     if(tmpP->jpgCb[n][m]==0){
                         zeroCounter++;

                         if(zeroCounter==15 && zeroTrigger){
                             if(f==2){
                             if(m!=7){
                                if(tmpP->jpgCb[n][m+1]==0){
                                 addInACNodeVector(zeroCounter,0);
                             zeroCounter=0;
                             zeroTrigger=0;
                             lastZeroTrigger=1;
                                }
                             }else{
                                 if(tmpP->jpgCb[n+1][m]==0){
                                  addInACNodeVector(zeroCounter,0);
                              zeroCounter=0;
                              zeroTrigger=0;
                              lastZeroTrigger=1;
                                 }
                             }
                             }else{
                                 if(tmpP->jpgCb[n-1][m+1]==0){
                                  addInACNodeVector(zeroCounter,0);
                              zeroCounter=0;
                              zeroTrigger=0;
                              lastZeroTrigger=1;
                                 }
                             }
                         }

                     }else {
                         addInACNodeVector(zeroCounter,tmpP->jpgCb[n][m]);
                         zeroCounter=0;
                         if(zeroTrigger==0){
                             zeroTrigger=1;
                             lastZeroTrigger=0;
                         }
                     }


                     z++;
                     continue;
                 }

                 if (f==2){
                     if(m!=7)
                         m++;
                     else
                         n++;

                     if(tmpP->jpgCb[n][m]==0){
                         zeroCounter++;

                         if(zeroCounter==15 && zeroTrigger){
                             if(tmpP->jpgCb[n+1][m-1]==0){
                                 addInACNodeVector(zeroCounter,0);
                             zeroCounter=0;
                             zeroTrigger=0;
                             lastZeroTrigger=1;
                             }

                         }

                     }else {
                         addInACNodeVector(zeroCounter,tmpP->jpgCb[n][m]);
                         zeroCounter=0;
                         if(zeroTrigger==0){
                             zeroTrigger=1;
                             lastZeroTrigger=0;
                         }
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


                     if(tmpP->jpgCb[n][m]==0){
                         zeroCounter++;

                         if(zeroCounter==15 && zeroTrigger){
                             if(f==0){
                             if(n!=7){
                                if(tmpP->jpgCb[n+1][m]==0){
                                 addInACNodeVector(zeroCounter,0);
                             zeroCounter=0;
                             zeroTrigger=0;
                             lastZeroTrigger=1;
                                }
                             }else{
                                 if(tmpP->jpgCb[n][m+1]==0){
                                  addInACNodeVector(zeroCounter,0);
                              zeroCounter=0;
                              zeroTrigger=0;
                              lastZeroTrigger=1;
                                 }
                             }
                             }else{
                                 if(tmpP->jpgCb[n+1][m-1]==0){
                                  addInACNodeVector(zeroCounter,0);
                              zeroCounter=0;
                              zeroTrigger=0;
                              lastZeroTrigger=1;
                                 }
                             }
                         }

                     }else {
                         addInACNodeVector(zeroCounter,tmpP->jpgCb[n][m]);
                         zeroCounter=0;
                         if(zeroTrigger==0){
                             zeroTrigger=1;
                             lastZeroTrigger=0;
                         }
                     }


                     z++;
                     continue;
                 }



             }while(z!=65);


             if(lastZeroTrigger){
                 for(vector<node*>::iterator f=nodeWeightList.begin();f!=nodeWeightList.end();f++){
                      node* tmpNode=*f;
                      if(tmpNode->Key==240)
                          tmpNode->weight-=1;
                 }

             }


             if(zeroCounter!=0)
                 addInACNodeVector(0,0);



     }

     for(vector<node*>::iterator f=nodeWeightList.begin();f!=nodeWeightList.end();f++){
          node* tmpNode=*f;
          if(tmpNode->weight==0){
              nodeWeightList.erase(f);
              break;
          }
     }



     //сортировка по возрастанию
     sort(nodeWeightList.begin(),nodeWeightList.end(),[](const node* a, const node* b){
         return a->weight < b->weight;
     });

     //вывод отсортированного вектора
     //вывод Cr и Cb коэффициентов AC1 дерева Хаффмана и их вероятноестей
//     int c=1;
//     for(vector<node*>::iterator f=nodeWeightList.begin();f!=nodeWeightList.end();f++){
//          node* tmpNode=*f;
//          __int16 itt=tmpNode->Key;
//          cout<<c<<") "<<hex<<itt<<" x "<<dec<<tmpNode->weight<<endl;
//          c++;
//     }

     makeTree();
     treeValk(jpgTmpRoot);


     lenthTable[16];
     for(int i=0;i<16;i++){
         lenthTable[i]=0;
     }

     for(int i=0;i<16;i++){
         for(vector<pair<int,string>>::iterator f=jpgTmpDHTtable.begin();f!=jpgTmpDHTtable.end();f++){
             pair<int,string> tmpPair=*f;
             string str=tmpPair.second;
             int t=atoi(str.c_str());
             if(t==i+1){
                 lenthTable[i]+=1;
             }

         }

     }

     dhtElCount=0;
     for(int i=0;i<16;i++){
         cout<<lenthTable[i]<<" ";
         dhtElCount+=lenthTable[i];
     }
     cout<<endl;

     jpg->dhtAllMark.jpgDHTtables[3]->jpgTableSize=dhtElCount;

     vector<node*> reverseNodeVector4;
     for(vector<node*>::iterator f=nodeWeightList.begin();f!=nodeWeightList.end();f++){
         reverseNodeVector4.emplace(reverseNodeVector4.begin(),*f);
     }


     dhtTabeleCounter=0;
     TREE DHTtree4;
     tmpNode=DHTtree4.root;
     dhtTmp=0;
     nodeWeightListIterator=reverseNodeVector4.begin();

     delete [] jpg->dhtAllMark.jpgDHTtables[3]->jpgDHTtable;
     jpg->dhtAllMark.jpgDHTtables[3]->jpgDHTtable=new pair<int,string>[dhtElCount];

     for(int i=0;i<16;i++){
         for(int j=0;j<lenthTable[i];j++){
             node* tNode=*nodeWeightListIterator;
             nodeWeightListIterator++;
             dhtTmp=tNode->Key;
             dhtCode2="";
             makeTreeRec(i,dhtTmp,tmpNode, &DHTtree4);
             jpg->dhtAllMark.jpgDHTtables[3]->jpgDHTtable[dhtTabeleCounter].first=dhtTmp;
             jpg->dhtAllMark.jpgDHTtables[3]->jpgDHTtable[dhtTabeleCounter].second=dhtCode2;

             dhtTabeleCounter++;
         }
     }


     jpg->dhtAllMark.jpgDHTtables[dhtAllMarkCounter]->jpgDHTtreesRoots=DHTtree4.root;
     dhtAllMarkCounter++;

     cc=1;
     //вывод конечного дерева Хаффмана
     for(int j=0;j<jpg->dhtAllMark.jpgDHTtables[3]->jpgTableSize;j++){
     cout<<dec<<setw(3)<<cc<<") "<<hex<<setw(3)<<jpg->dhtAllMark.jpgDHTtables[3]->jpgDHTtable[j].first<<"; "<<jpg->dhtAllMark.jpgDHTtables[3]->jpgDHTtable[j].second<<endl;
     cc++;
     }

     cout<<"~~~~~ AC1 end ~~~~~~"<<endl;

     //очищаем
     nodeWeightList.clear();
     nodeWeightLIST.clear();
     jpgTmpDHTtable.clear();
}

pair<int,int> JPGWrite::addPair(int zeros, int value){
    pair<int,int> tmpPair;
    tmpPair.first=zeros<<4 | tableElementLenth(value);
//    if(value<0)
//        value*=-1;
    tmpPair.second=value;
    return tmpPair;
}

vector<vector<pair<int,int>>> JPGWrite::makeCodes(JPGALLPICTABLES *tmpP){

    vector<vector<pair<int,int>>> acNumbers;
//    acNumbers.resize(6);
//    vector<vector<pair<int,int>>>::iterator concreteVectorIterator;
    vector<pair<int,int>> concreteVector;
    pair<int,int> tmpPair;
    int tmpArray[8][8];



//    concreteVectorIterator=acNumbers.begin();

        for(int i=0;i<6;i++){

            concreteVector.clear();

            int choose=0;
            if(i>4)
                choose=i;
            switch (choose) {
            case 0:
                for(int j=0;j<8;j++)
                    for(int k=0;k<8;k++)
                        tmpArray[j][k]=tmpP->jpgYtable[i][j][k];
                break;
            case 4:
                for(int j=0;j<8;j++)
                    for(int k=0;k<8;k++)
                        tmpArray[j][k]=tmpP->jpgCb[j][k];
                break;
            case 5:
                for(int j=0;j<8;j++)
                    for(int k=0;k<8;k++)
                        tmpArray[j][k]=tmpP->jpgCr[j][k];
                break;
            }
            int zeroCounter=0;
            bool zeroTrigger=1;
            bool lastZeroTrigger=0;

            int z=2;
            int f=2;
            int n,m;
            n=m=0;
            do{

                if (f==0){
                    if(n!=7)
                        n++;
                    else
                        m++;
                    f=1;

                    if(tmpArray[n][m]==0){
                        zeroCounter++;

                        if(zeroCounter==15 && zeroTrigger){
                            if(tmpArray[n-1][m+1]==0){

                                tmpPair=addPair(zeroCounter,0);
                                concreteVector.push_back(tmpPair);

//                                addInACNodeVector(zeroCounter,0);//!!!!!!!!!!!!!!!

                            zeroCounter=0;
                            zeroTrigger=0;
                            lastZeroTrigger=1;
                            }

                        }

                    }else {

                        tmpPair=addPair(zeroCounter,tmpArray[n][m]);
                        concreteVector.push_back(tmpPair);

//                        addInACNodeVector(zeroCounter,tmpArray[n][m]);//!!!!!!!!!!!!!!

                        zeroCounter=0;
                        if(zeroTrigger==0){
                            zeroTrigger=1;
                            lastZeroTrigger=0;
                        }
                    }

                    z++;
                    continue;
                }
                if(f==1){
                    n--;
                    m++;
                    if(n==0 or m==7)
                        {f=2;}

                    if(tmpArray[n][m]==0){
                        zeroCounter++;

                        if(zeroCounter==15 && zeroTrigger){
                            if(f==2){
                            if(m!=7){
                               if(tmpArray[n][m+1]==0){

                                   tmpPair=addPair(zeroCounter,0);
                                   concreteVector.push_back(tmpPair);

//                                addInACNodeVector(zeroCounter,0);//!!!!!!!!!!!!!!!!!!!!

                            zeroCounter=0;
                            zeroTrigger=0;
                            lastZeroTrigger=1;
                               }
                            }else{
                                if(tmpArray[n+1][m]==0){

                                    tmpPair=addPair(zeroCounter,0);
                                    concreteVector.push_back(tmpPair);

//                                 addInACNodeVector(zeroCounter,0);//!!!!!!!!!!!!!!!!!!!

                             zeroCounter=0;
                             zeroTrigger=0;
                             lastZeroTrigger=1;
                                }
                            }
                            }else{
                                if(tmpArray[n-1][m+1]==0){

                                    tmpPair=addPair(zeroCounter,0);
                                    concreteVector.push_back(tmpPair);

//                                 addInACNodeVector(zeroCounter,0);//!!!!!!!!!!!!!!!!

                             zeroCounter=0;
                             zeroTrigger=0;
                             lastZeroTrigger=1;
                                }
                            }
                        }

                    }else {

                        tmpPair=addPair(zeroCounter,tmpArray[n][m]);
                        concreteVector.push_back(tmpPair);

//                        addInACNodeVector(zeroCounter,tmpArray[n][m]);//!!!!!!!!!!!!!!

                        zeroCounter=0;
                        if(zeroTrigger==0){
                            zeroTrigger=1;
                            lastZeroTrigger=0;
                        }
                    }


                    z++;
                    continue;
                }

                if (f==2){
                    if(m!=7)
                        m++;
                    else
                        n++;

                    if(tmpArray[n][m]==0){
                        zeroCounter++;

                        if(zeroCounter==15 && zeroTrigger){
                            if(tmpArray[n+1][m-1]==0){

                                tmpPair=addPair(zeroCounter,0);
                                concreteVector.push_back(tmpPair);

//                                addInACNodeVector(zeroCounter,0);//!!!!!!!!!!!!!!!!!!1

                            zeroCounter=0;
                            zeroTrigger=0;
                            lastZeroTrigger=1;
                            }

                        }

                    }else {

                        tmpPair=addPair(zeroCounter,tmpArray[n][m]);
                        concreteVector.push_back(tmpPair);

//                        addInACNodeVector(zeroCounter,tmpArray[n][m]);//!!!!!!!!!!!!!!!!!!

                        zeroCounter=0;
                        if(zeroTrigger==0){
                            zeroTrigger=1;
                            lastZeroTrigger=0;
                        }
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


                    if(tmpArray[n][m]==0){
                        zeroCounter++;

                        if(zeroCounter==15 && zeroTrigger){
                            if(f==0){
                            if(n!=7){
                               if(tmpArray[n+1][m]==0){

                                   tmpPair=addPair(zeroCounter,0);
                                   concreteVector.push_back(tmpPair);

//                                addInACNodeVector(zeroCounter,0);//!!!!!!!!!!!!!!!!

                            zeroCounter=0;
                            zeroTrigger=0;
                            lastZeroTrigger=1;
                               }
                            }else{
                                if(tmpArray[n][m+1]==0){

                                    tmpPair=addPair(zeroCounter,0);
                                    concreteVector.push_back(tmpPair);

//                                 addInACNodeVector(zeroCounter,0);//!!!!!!!!!!!!!!!!!!!

                             zeroCounter=0;
                             zeroTrigger=0;
                             lastZeroTrigger=1;
                                }
                            }
                            }else{
                                if(tmpArray[n+1][m-1]==0){

                                    tmpPair=addPair(zeroCounter,0);
                                    concreteVector.push_back(tmpPair);

//                                 addInACNodeVector(zeroCounter,0);//!!!!!!!!!!!!!!!!

                             zeroCounter=0;
                             zeroTrigger=0;
                             lastZeroTrigger=1;
                                }
                            }
                        }

                    }else {

                        tmpPair=addPair(zeroCounter,tmpArray[n][m]);
                        concreteVector.push_back(tmpPair);

//                        addInACNodeVector(zeroCounter,tmpArray[n][m]);//!!!!!!!!!!!!!!!!

                        zeroCounter=0;
                        if(zeroTrigger==0){
                            zeroTrigger=1;
                            lastZeroTrigger=0;
                        }
                    }


                    z++;
                    continue;
                }



            }while(z!=65);


//            for(vector<pair<int,int>>::iterator f=concreteVector.begin();f!=concreteVector.end();f++){
//                pair<int,int> tmpPair=*f;
//                cout<<hex<<tmpPair.first<<" ";
//            }
//            cout<<endl;


            pair<int,int> endPair=*concreteVector.rbegin();
            if(endPair.first==240)
                concreteVector.erase(concreteVector.end());


            if(zeroCounter!=0){
                tmpPair=addPair(0,0);
                concreteVector.push_back(tmpPair);
            }


            acNumbers.push_back(concreteVector);
            concreteVector.clear();

        }

        return acNumbers;
    }

void testFunc(){
    int i=9;
}

__int32 JPGWrite::binCode(string sBinCode,int value){

    int buffer=0;

    int lenth=sBinCode.size();


    while (sBinCode.size()!=0) {
         buffer=buffer<<1;
        char s=*sBinCode.begin();
        if(s=='1')
            buffer|=1;
        sBinCode.erase(sBinCode.begin());

    }

//    char b1[sizeof(buffer) * 8 + 1];
//    itoa(buffer, b1, 2);
//    cout<<setw(lenth)<<setfill('0')<<b1<<" ";

//    для тестов
//    if(value==0){
//        testFunc();
//        buffer=buffer<<1;
//    }else {

    lenth=tableElementLenth(value);

        buffer=buffer<<tableElementLenth(value);

        if(value<0){
            value=~value;
        }
//    }


    int ClearVal=pow(2,tableElementLenth(value))-1;
    value=value & ClearVal;

//    char b2[sizeof(value) * 8 + 1];
//    itoa(value, b2, 2);
//    cout<<setw(lenth)<<setfill('0')<<b2<<endl;

    buffer|=value;

    //побитовый вывод
//    char b[sizeof(buffer) * 8 + 1];
//    itoa(buffer, b, 2);
//    cout<<setw(lenth)<<setfill('0')<<b<<endl;

    return buffer;

}

void JPGWrite::codeIt(vector<vector<pair<int,int>>> acNumbers, vector<int> dcNumbers){

    __int32 buffer=0;
    __int32 tmpBitCode=0;
    int bufferSize=0;
    int tmpBitCodeSize=0;
    string tmpStringBitCode;



    vector<vector<pair<int,int>>>::iterator acVectorIterator=acNumbers.begin();
    vector<int>::iterator dcVectorIterator=dcNumbers.begin();


    for(int i=0;i<6;i++){
        int tmpDCValue=*dcVectorIterator;
        dcVectorIterator++;

        if(i==1)
            testFunc();


        int tableId=0;
        if(i>3)
            tableId=2;

        for(int t=0;t<jpg->dhtAllMark.jpgDHTtables[tableId]->jpgTableSize;t++){
            pair<int,string> jpgDHTpair=jpg->dhtAllMark.jpgDHTtables[tableId]->jpgDHTtable[t];
            if(tableElementLenth(tmpDCValue)==jpgDHTpair.first){

                tmpBitCode=binCode(jpgDHTpair.second,tmpDCValue);

                tmpStringBitCode=jpgDHTpair.second;
                tmpBitCodeSize=tableElementLenth(tmpDCValue) + tmpStringBitCode.size();
//                if(tmpDCValue==0){
//                    tmpBitCodeSize+=1;
//                }

                buffer=(buffer<<tmpBitCodeSize)|tmpBitCode;
                bufferSize+=tmpBitCodeSize;

                break;
            }
        }

        while (bufferSize>=8) {
            char c=0;

            c=buffer>>(bufferSize-8);

//            cout<<c<<" ";

            jpg->picMark.jpgPicture.push_back(c);

            int obnulenie=pow(2,bufferSize-8)-1;
            buffer=buffer & obnulenie;

            bufferSize-=8;
        }

        //цикл не по всему АС а по конкретному вектору.

        vector<pair<int,int>> concreteVector=*acVectorIterator;
        acVectorIterator++;

        for(vector<pair<int,int>>::iterator concreteVectorIterator=concreteVector.begin();
            concreteVectorIterator!=concreteVector.end();concreteVectorIterator++){

            pair<int,int> tmpACValue=*concreteVectorIterator;

            int tableId=1;
            if(i>3)
                tableId=3;

            for(int t=0;t<jpg->dhtAllMark.jpgDHTtables[tableId]->jpgTableSize;t++){
                pair<int,string> jpgDHTpair=jpg->dhtAllMark.jpgDHTtables[tableId]->jpgDHTtable[t];
                if(tmpACValue.first==jpgDHTpair.first){

                    tmpBitCode=binCode(jpgDHTpair.second,tmpACValue.second);

                    tmpStringBitCode=jpgDHTpair.second;
                    tmpBitCodeSize=tableElementLenth(tmpACValue.second) + tmpStringBitCode.size();
//                    if(tmpACValue.second==0){
//                        testFunc();
//                    }

                    buffer=(buffer<<tmpBitCodeSize)|tmpBitCode;
                    bufferSize+=tmpBitCodeSize;

                    break;
                }
            }

            while (bufferSize>=8) {
                char c=0;

                c=buffer>>(bufferSize-8);

//                cout<<c<<" ";

                jpg->picMark.jpgPicture.push_back(c);

                int obnulenie=pow(2,bufferSize-8)-1;
                buffer=buffer & obnulenie;
                bufferSize-=8;
            }


        }


    }

//    cout<<"!"<<buffer<<" "<<bufferSize<<endl;
    buffer=buffer<<(8-bufferSize);
    int edinitsi=pow(2,8-bufferSize)-1;
    buffer|=edinitsi;
    char endSymb=buffer;
    jpg->picMark.jpgPicture.push_back(endSymb);


}

void JPGWrite::codeJpg(){

    vector<int> dcNumbers;
    vector<vector<pair<int,int>>> acNumbers;
    JPGALLPICTABLES *tmpP;

    for(vector<JPGALLPICTABLES*>::iterator j = jpg->picMark.jpgAllPicTables.begin();j!=jpg->picMark.jpgAllPicTables.end();j++){
        tmpP=*j;

        dcNumbers.push_back(tmpP->jpgYtable[0][0][0]);
        dcNumbers.push_back(tmpP->jpgYtable[1][0][0]);
        dcNumbers.push_back(tmpP->jpgYtable[2][0][0]);
        dcNumbers.push_back(tmpP->jpgYtable[3][0][0]);
        dcNumbers.push_back(tmpP->jpgCb[0][0]);
        dcNumbers.push_back(tmpP->jpgCr[0][0]);

//        for(vector<int>::iterator f=dcNumbers.begin();f!=dcNumbers.end();f++){
//            int t=*f;
//            cout<<dec<<t<<" ";
//        }

        acNumbers=makeCodes(tmpP);
//        вывод результатов работы функции makeCodes
//        vector<vector<pair<int,int>>>::iterator concreteVectorIterator;
//        vector<pair<int,int>> concreteVector;
//        pair<int,int> tmpPair;

//        for(concreteVectorIterator=acNumbers.begin();concreteVectorIterator!=acNumbers.end();concreteVectorIterator++){
//            concreteVector=*concreteVectorIterator;

//            for(vector<pair<int,int>>::iterator f=concreteVector.begin();f!=concreteVector.end();f++){
//                tmpPair=*f;
//                cout<<hex<<tmpPair.first<<" "<<tmpPair.second<<endl;
//            }
//            cout<<"~~~~~~~~~~~~~~~~"<<endl;
//        }



        codeIt(acNumbers,dcNumbers);






//        for(int ii=3;ii>0;ii--){
//             tmpP->jpgYtable[ii][0][0]=tmpP->jpgYtable[ii][0][0] - tmpP->jpgYtable[ii-1][0][0];
//        }

    }



}



