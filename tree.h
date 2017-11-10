#ifndef TREE_H
#define TREE_H

struct node
{
int Lvl=0;
int Key=-1;
node *Left=0;
node *Right=0;
node *Back=0;
unsigned long int weight=0;

node();

bool operator < (const node& a) const{
          return weight < a.weight;
        }

};


class TREE{
public:
node *root;
TREE(){root=new node;}
node *addNode(int key, int lvl,node * x);

~TREE();
void recDelTree(node* tmp);
};



#endif // TREE_H
