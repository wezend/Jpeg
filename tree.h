#ifndef TREE_H
#define TREE_H

struct node
{
int Lvl=0;
int Key=-1;
node *Left=0;
node *Right=0;
node *Back=0;
};


class TREE{
public:
node *root;
TREE(){root=new node;}
node *addNode(int key, int lvl,node * x);
};


#endif // TREE_H
