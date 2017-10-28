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
