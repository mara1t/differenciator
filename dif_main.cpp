#include "dif.h"

int main() 
{
    Node *root = new Node;
    root->s_data = new char[30];
    MakeTree(root);

    Dif(root);
    Optimization(root);
    PrintDot(root);

    return 0;
}