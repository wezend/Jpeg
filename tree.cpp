#include "tree.h"
#include <iostream>
using namespace std;

node *TREE::addNode(int key, int lvl,node * x){
node *tmp=new node;
tmp->Key=key;
tmp->Lvl=lvl;
tmp->Back=x;
key=-2;
return tmp;
}

TREE::~TREE()
{
//    recDelTree(this->root);
}

//void TREE::recDelTree(node* tmp){
//    if(tmp->Left)
//        recDelTree(tmp->Left);
//    if(tmp->Right)
//        recDelTree(tmp->Right);
//    delete tmp;
//}


node::node()
{
    this->Key=-2;
    this->Back=0;
    this->Left=0;
    this->Lvl=0;
    this->Right=0;
    this->weight=0;

    //return this;
}
