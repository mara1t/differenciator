#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
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
    double d_data;
    char *s_data = NULL;
    int type;
    Node *parent = NULL;
    Node *right = NULL;
    Node *left = NULL;
};

int MakeBranch(Node *root, FILE *);
int MakeTree(Node *root);
int PrintDot(Node*);
int PrintStruct(FILE*, Node*, int*);
int Dif(Node *root);
int CopyBranch(Node *root, Node *copyroot);
int Optimization(Node *);
int Latex(Node *);
int LatexIn(Node *, FILE *);