#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#define $$$ printf("on line=%d\n", __LINE__);

enum TYPES {
    VARIOUS,
    CONSTANT,
    OPER,
    SIN,
    COS,
    POW,
    ADD,
    SUB,
    MUL,
    DIV,
    NOT
};


struct Node {
    int d_data;
    char *s_data;
    int type;
    Node *parent;
    Node *right;
    Node *left;
};

int MakeBranch(Node *root, FILE *);
int MakeTree(Node *root);
int PrintDot(Node*);
int PrintStruct(FILE*, Node*, int*);
int Dif(Node *root);
int CopyBranch(Node *root, Node *copyroot);
int Optimization(Node *);