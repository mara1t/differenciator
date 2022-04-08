#include "dif.h"

int Dif(Node *root) 
{
    
    if (root->type == OPER) {
        
        if (strcmp(root->s_data, "+") == 0 || strcmp(root->s_data, "-") == 0) {
            Dif(root->left);
            Dif(root->right);
        }
        else if (strcmp(root->s_data, "*") == 0) {
            
            Node *l_copyroot = new Node;
            l_copyroot->s_data = new char[30];
            CopyBranch(root->left, l_copyroot);
            Dif(l_copyroot);

            Node *r_copyroot = new Node;
            r_copyroot->s_data = new char[30];
            CopyBranch(root->right, r_copyroot);
            Dif(r_copyroot);

            Node *new_l = new Node;
            new_l->s_data = new char[30];
            Node *new_r = new Node;
            new_r->s_data = new char[30];

            strcpy(root->s_data, "+");
            strcpy(new_r->s_data, "*");
            strcpy(new_l->s_data, "*");

            new_l->left = r_copyroot;
            new_l->right = root->left;
            
            new_r->left = l_copyroot;
            new_r->right = root->right;

            root->left = new_l;
            new_l->parent = root;
            root->right = new_r;
            new_r->parent = root;
        }
        else if (strcmp(root->s_data, "/") == 0) {
            
            Node *l_copyroot = new Node;
            l_copyroot->s_data = new char[30];
            CopyBranch(root->left, l_copyroot);
            Dif(l_copyroot);

            Node *r_copyroot = new Node;
            r_copyroot->s_data = new char[30];
            CopyBranch(root->right, r_copyroot);
            Dif(r_copyroot);

            Node *new_l = new Node;
            new_l->s_data = new char[30];
            Node *new_r = new Node;
            new_r->s_data = new char[30];

            strcpy(new_r->s_data, "^");
            strcpy(new_l->s_data, "-");

            new_l->left = new Node;
            new_l->right = new Node;
            
            new_l->left->s_data = new char[30];
            new_l->right->s_data = new char[30];

            new_l->left->type = OPER;
            new_l->right->type = OPER;

            strcpy(new_l->right->s_data, "*");
            strcpy(new_l->left->s_data, "*");


            new_l->left->left = l_copyroot;
            new_l->left->right = root->right;

            new_l->right->left = r_copyroot;
            new_l->right->right = root->left;


            
            
            new_r->left = root->right;

            new_r->right = new Node;
            new_r->right->s_data = new char[30];

            new_r->right->type = CONSTANT;
            new_r->right->d_data = 2;

            root->left = new_l;
            new_l->parent = root;
            root->right = new_r;
            new_r->parent = root;
        }
        else if (strcmp(root->s_data, "^") == 0) {
            ;
        }
        else if (strcmp(root->s_data, "sin") == 0) {
            ;
        }
        else if (strcmp(root->s_data, "cos") == 0) {
            ;
        }
        else {
            printf("oshibka here\n");
        }
    }
    else if (root->type == VARIOUS) {
        root->type = CONSTANT;
        root->d_data = 1;
        return 0;
    }
    else if (root->type == CONSTANT) {
        root->type = CONSTANT;
        root->d_data = 0;
        return 0;
    }
    else {
        printf("error in dif");
    }

    return 0;
}

int CopyBranch(Node *root, Node *copyroot)
{
    copyroot->type = root->type;

    if (copyroot->type == CONSTANT) {
        copyroot->d_data = root->d_data;    
    }
    else {
        strcpy(copyroot->s_data, root->s_data);
    }


    if (root->left != nullptr) {
        copyroot->left = new Node;
        copyroot->left->s_data = new char[30];
        copyroot->left->parent = copyroot;
        CopyBranch(root->left, copyroot->left);
    }
    else {
        ;
    }
    if (root->right != nullptr) {
        copyroot->right = new Node;
        copyroot->right->s_data = new char[30];
        copyroot->right->parent = copyroot;
        CopyBranch(root->right, copyroot->right);
    }
    else {
        ;
    }
    
    
    return 0;
}


int MakeTree(Node *root)
{
    FILE *file = fopen("1.txt", "r");

    char tmp_symb = fgetc(file);

    while(isspace(tmp_symb)) {
        tmp_symb = fgetc(file);
    }

    MakeBranch(root, file);

    tmp_symb = fgetc(file);

    while(isspace(tmp_symb)) {
        tmp_symb = fgetc(file);
    }

    fclose(file);
    return 0;
}

int MakeBranch(Node *root, FILE *file)
{
    
    char tmp_symb = fgetc(file);
    int tmp_num = 0;

    root->left = new Node;
    root->left->parent = root;
    root->left->s_data = new char[30];

    root->right = new Node;
    root->right->parent = root;
    root->right->s_data = new char[30];


    while(isspace(tmp_symb)) {
        tmp_symb = fgetc(file);
    }

    if (tmp_symb == '(') {
        
        MakeBranch(root->left, file);

        tmp_symb = fgetc(file);

        while(isspace(tmp_symb)) {
            tmp_symb = fgetc(file);
        }

        tmp_symb = fgetc(file);

        while(isspace(tmp_symb)) {
            tmp_symb = fgetc(file);
        }

        root->type = OPER;

        if      (tmp_symb == '+') strcpy(root->s_data, "+");
        else if (tmp_symb == '-') strcpy(root->s_data, "-");
        else if (tmp_symb == '*') strcpy(root->s_data, "*");
        else if (tmp_symb == '/') strcpy(root->s_data, "/");
        else if (tmp_symb == '^') strcpy(root->s_data, "^");
        else {
            printf("ERROR\n");
            return -1;
        }

        tmp_symb = fgetc(file);

        while(isspace(tmp_symb)) {
            tmp_symb = fgetc(file);
        }

        MakeBranch(root->right, file);  

        tmp_symb = fgetc(file);

        while(isspace(tmp_symb)) {
            tmp_symb = fgetc(file);
        } 
    }
    else if (tmp_symb == 'x' || isdigit(tmp_symb)) {
        
        if (tmp_symb == 'x') {
            strcpy(root->left->s_data, "x");
            root->left->type = VARIOUS;
        }
        else if (isdigit(tmp_symb)) {
            ungetc(tmp_symb, file);
            fscanf(file, "%d", &tmp_num);
            root->left->d_data = tmp_num;
            root->left->type = CONSTANT;
        }
        else {
            printf("error\n");
        }

        tmp_symb = fgetc(file);

        while(isspace(tmp_symb)) {
            tmp_symb = fgetc(file);
        }

        root->type = OPER;

        if      (tmp_symb == '+') strcpy(root->s_data, "+");
        else if (tmp_symb == '-') strcpy(root->s_data, "-");
        else if (tmp_symb == '*') strcpy(root->s_data, "*");
        else if (tmp_symb == '/') strcpy(root->s_data, "/");
        else if (tmp_symb == '^') strcpy(root->s_data, "^");
        else {
            printf("%cERROR\n", tmp_symb);
            return -1;
        }

        tmp_symb = fgetc(file);

        while(isspace(tmp_symb)) {
            tmp_symb = fgetc(file);
        }


        if (tmp_symb == 'x') {
            strcpy(root->right->s_data, "x");
            root->right->type = VARIOUS;
        }
        else if (isdigit(tmp_symb)) {
            
            ungetc(tmp_symb, file);
            fscanf(file, "%d", &tmp_num);
            root->right->d_data = tmp_num;
            root->right->type = CONSTANT;
        }

    }
    else if (isalpha(tmp_symb)) {
        if (tmp_symb == 's') {
            strcpy(root->s_data, "sin");
        }
        if (tmp_symb == 'c') {
            strcpy(root->s_data, "cos");
        }

        delete[] root->right;   
            
        root->type = OPER;

        while(tmp_symb != '(') {
            tmp_symb = fgetc(file);
        }

        while(isspace(tmp_symb)) {
            tmp_symb = fgetc(file);
        }

        MakeBranch(root->left, file);

        tmp_symb = fgetc(file);

        while(isspace(tmp_symb)) {
            tmp_symb = fgetc(file);
        }
        
    }
    

    return 0;
}

int PrintDot(Node *root)
{
    assert(root != NULL);

    FILE *graph = fopen("graph.txt", "w");

    int global_counter = 0;

    fprintf(graph, "digraph G{\n");
    PrintStruct(graph, root, &global_counter);
    fprintf(graph, "}");

    fclose(graph);

    return 0;
}

int PrintStruct(FILE *graph, Node *node, int *global_counter)
{
    if (node == NULL)
        return 0;
    
    int tmp_counter = *global_counter;

    if (node->type == VARIOUS || node->type == OPER) {
        fprintf(graph, "\tstruct%d [shape=record,label=\"{<f0>%s | {<f1> left | <f2> right} }\" ];\n", 
        *global_counter, node->s_data);
    }
    else {
        fprintf(graph, "\tstruct%d [shape=record,label=\"{<f0>%d | {<f1> left | <f2> right} }\" ];\n", 
        *global_counter, node->d_data);
    }
    if (node->left == NULL && node->right == NULL) 
        return 0;
    
    fprintf(graph, "\tstruct%d->struct%d;\n", tmp_counter, tmp_counter + 1);

    (*global_counter)++;
    
    PrintStruct(graph, node->left, global_counter);
    
    (*global_counter)++;

    if (strcmp(node->s_data, "sin") != 0 && strcmp(node->s_data, "cos") != 0) {
        fprintf(graph, "\tstruct%d->struct%d;\n", tmp_counter, *global_counter );
        
        PrintStruct(graph, node->right, global_counter);
    }
    return 0;
}
