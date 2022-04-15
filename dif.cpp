#include "dif.h"

int Latex(Node *root)
{
    assert(root != NULL);
    FILE *f = fopen("lat.txt", "w");
    fprintf(f, "\\documentclass{article}\n \\begin{document}\n\n\\[\n");

    LatexIn(root, f);

    fprintf(f, "\n\\]\n\n\\end{document}");
    fclose(f);
    return 0;
}

int LatexIn(Node *root, FILE *f) 
{
    if (root->type == OPER) {
        if (strcmp(root->s_data, "/") == 0) {
            fprintf(f, "\\frac{");
            LatexIn(root->left, f);
            fprintf(f, "}{");
            LatexIn(root->right, f);
            fprintf(f, "}");
        }
        else if (strcmp(root->s_data, "*") == 0) {
            
            LatexIn(root->left, f);
            fprintf(f, " \\cdot ");
            LatexIn(root->right, f);
            
        }
        else if (strcmp(root->s_data, "sin") == 0 || strcmp(root->s_data, "ln") == 0 || strcmp(root->s_data, "cos") == 0) {
            fprintf(f, "sin(");
            LatexIn(root->left, f);
            fprintf(f, ")");
        }
        else {
            LatexIn(root->left, f);
            fprintf(f, "%s", root->s_data);
            LatexIn(root->right, f);

        }
    }
    else if (root->type == VARIOUS) {
        fprintf(f, "x");
    }
    else if (root->type == CONSTANT) {
        fprintf(f, "%lg", root->d_data);
    }

    return 0;
}

int Optimization(Node *root)
{
    assert(root != NULL);

    
    if (root->type == OPER) {
        
        if (strcmp(root->s_data, "cos") == 0 || strcmp(root->s_data, "sin") == 0) {
            $$$
            Optimization(root->left);
            if (root->left->type == CONSTANT) {
                if (strcmp(root->s_data, "sin") == 0) {
                    root->d_data = sin(root->left->d_data);
                                    
                    root->type = CONSTANT;

                    delete[] root->left;
                    root->left = NULL;
                }
                else if (strcmp(root->s_data, "cos") == 0) {
                    root->d_data = cos(root->left->d_data);
                                    
                    root->type = CONSTANT;

                    delete[] root->left;
                    root->left = NULL;
                }    
            }
            $$$
        }
        else if (strcmp(root->s_data, "+") == 0 || strcmp(root->s_data, "-") == 0) {
            
            
            Optimization(root->left);
            Optimization(root->right);

            if (root->left->type == CONSTANT && root->left->d_data == 0) {
                root->d_data = root->right->d_data;
                strcpy(root->s_data, root->right->s_data);
                root->type = root->right->type;
                root->left = root->right->left;
                if (root->left != NULL)
                    root->left->parent = root;
                root->right = root->right->right;
                if (root->right != NULL)
                    root->right->parent = root;
            }
            else if (root->right->type == CONSTANT && root->right->d_data == 0 && strcmp(root->s_data, "+") == 0) {
                root->d_data = root->left->d_data; 
                strcpy(root->s_data, root->left->s_data);
                root->type = root->left->type;
                root->right = root->left->right;
                if (root->right != NULL)
                    root->right->parent = root;
                root->left = root->left->left;
                if (root->left != NULL)
                    root->left->parent = root;
            }
            else if (strcmp(root->s_data, "+") == 0) {
                
                if (root->left->type == CONSTANT && root->right->type == CONSTANT) {
                    
                    root->d_data = root->left->d_data + root->right->d_data;
                    root->type = CONSTANT;
                    delete[] root->left;
                    delete[] root->right;
                    root->left = NULL;
                    root->right = NULL;
                }
                else if (root->left->type == VARIOUS && root->right->type == VARIOUS) {
                    
                    strcpy(root->s_data, "*");
                    root->right->d_data = 2;
                    root->right->type = CONSTANT;
                }
            }
            else if (strcmp(root->s_data, "-") == 0) {
                
                if (root->left->type == root->right->type == CONSTANT) {
                    root->d_data = root->left->d_data - root->right->d_data;
                    root->type = CONSTANT;
                    delete[] root->left;
                    delete[] root->right;
                    root->left = NULL;
                    root->right = NULL;
                }
            }
        }
        else if (strcmp(root->s_data, "*") == 0) {
            $$$
            Optimization(root->left);
            Optimization(root->right);
            if (root->left->type == CONSTANT && root->right->type == CONSTANT) {
                root->d_data = root->left->d_data * root->right->d_data;
                root->type = CONSTANT;
                delete[] root->left;
                delete[] root->right;
                root->left = NULL;
                root->right = NULL;
            }
            else if ((root->right->type == CONSTANT && root->right->d_data == 0) || (root->left->type == CONSTANT && root->left->d_data == 0)) {
                root->type = CONSTANT;
                root->d_data = 0;
                if (root->left != NULL) {
                    delete[] root->left;
                    root->left = NULL;
                }
                if (root->right != NULL) {
                    delete[] root->right;
                    root->right = NULL;
                }
            }
            else if (root->right->type == CONSTANT && root->right->d_data == 1) {

                root->d_data = root->left->d_data;
                strcpy(root->s_data, root->left->s_data);
                root->type = root->left->type;
                root->right = root->left->right;
                if (root->right != NULL)
                    root->right->parent = root;
                root->left = root->left->left;
                if (root->left != NULL)
                    root->left->parent = root;
            }
            else if (root->left->type == CONSTANT && root->left->d_data == 1) {
                root->d_data = root->right->d_data;
                strcpy(root->s_data, root->right->s_data);
                root->type = root->right->type;
                root->right = root->left->right;
                if (root->right != NULL)
                    root->right->parent = root;
                root->left = root->left->left;
                if (root->left != NULL)
                    root->left->parent = root;
            }
            $$$
        }
        else if (strcmp(root->s_data, "/") == 0) {
            Optimization(root->left);
            Optimization(root->right);
            if (root->left->type == root->right->type == CONSTANT) {
                root->d_data = root->left->d_data / root->right->d_data;
                root->type = CONSTANT;
                delete[] root->left;
                delete[] root->right;
                root->left = NULL;
                root->right = NULL;
            }
            else if (root->left->type == CONSTANT && root->left->d_data == 0) {
                root->type = CONSTANT;
                root->d_data = 0;
                if (root->left != NULL) {
                    delete[] root->left;
                    root->left = NULL;
                }
                if (root->right != NULL) {
                    delete[] root->right;
                    root->right = NULL;
                }
            }
            else if (root->right->type == CONSTANT && root->right->d_data == 1) {
                root->d_data = root->left->d_data;
                strcpy(root->s_data, root->left->s_data);
                root->type = root->left->type;
                root->right = root->left->right;
                if (root->right != NULL)
                    root->right->parent = root;
                root->left = root->left->left;
                if (root->left != NULL)
                    root->left->parent = root;
            }
        }
        else if (strcmp(root->s_data, "^") == 0) {
        
            Optimization(root->left);
            Optimization(root->right);

            if (root->right->type == CONSTANT && root->right->d_data == 0) {

                root->type = CONSTANT;
                root->d_data = 1;
                if (root->left != NULL) {
                    delete[] root->left;
                    root->left = NULL;
                }
                if (root->right != NULL) {
                    delete[] root->right;
                    root->right = NULL;
                }
            }
            else if (root->right->type == CONSTANT && root->right->d_data == 1) {
                root->type = root->left->type;
                root->d_data = root->left->d_data;
                strcpy(root->s_data, root->left->s_data);
                root->right = root->left->right;
                if (root->right != NULL)
                    root->right->parent = root;
                root->left = root->left->left;
                if (root->left != NULL)
                    root->left->parent = root;
                
            }
            else if (root->right->type == CONSTANT && root->left->type == CONSTANT) {
                root->d_data = pow(root->left->d_data, root->right->d_data);
                root->type = CONSTANT;
                delete[] root->left;
                delete[] root->right;
                root->left = NULL;
                root->right = NULL;

            }
        }
    }
    
    return 0;
}

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

            Node *r_copyroot = new Node;
            r_copyroot->s_data = new char[30];
            CopyBranch(root->right, r_copyroot);

            Node *l_dif = new Node;
            l_dif->s_data = new char[30];
            CopyBranch(root->left, l_dif);
            Dif(l_dif);

            Node *r_dif = new Node;
            r_dif->s_data = new char[30];
            CopyBranch(root->right, r_dif);
            Dif(r_dif);


            Node *new_l = new Node;
            new_l->s_data = new char[30];
            new_l->type = OPER;
            Node *new_r = new Node;
            new_r->s_data = new char[30];
            new_r->type = OPER;

            strcpy(root->s_data, "+");
            strcpy(new_r->s_data, "*");
            strcpy(new_l->s_data, "*");

            root->left = new_l;
            root->right = new_r;

            new_l->left = l_copyroot;
            new_l->left->parent = new_l;

            new_l->right = r_dif;
            new_l->right->parent = new_l;

            new_r->left = r_copyroot;
            new_r->left->parent = new_r;

            new_r->right = l_dif;
            new_r->right->parent = new_r;

        }
        else if (strcmp(root->s_data, "/") == 0) {
            
            Node *l_copyroot = new Node;
            l_copyroot->s_data = new char[30];
            CopyBranch(root->left, l_copyroot);

            Node *r_copyroot = new Node;
            r_copyroot->s_data = new char[30];
            CopyBranch(root->right, r_copyroot);

            Node *l_dif = new Node;
            l_dif->s_data = new char[30];
            CopyBranch(root->left, l_dif);
            Dif(l_dif);

            Node *r_dif = new Node;
            r_dif->s_data = new char[30];
            CopyBranch(root->right, r_dif);
            Dif(r_dif);

            Node *new_l = new Node;
            new_l->s_data = new char[30];
            new_l->type = OPER;

            Node *new_r = new Node;
            new_r->s_data = new char[30];
            new_r->type = OPER;

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


            new_l->left->left = l_dif;
            new_l->left->left->parent = new_l->left;

            new_l->left->right = r_copyroot;
            new_l->left->right->parent = new_l->left;

            new_l->right->left = r_dif;
            new_l->right->left->parent = new_l->right;

            new_l->right->right = l_copyroot;
            new_l->right->right->parent = new_l->right;

            new_r->left = r_copyroot;
            new_r->left->parent = new_r;

            new_r->right = new Node;
            new_r->right->s_data = new char[30];

            new_r->right->type = CONSTANT;
            new_r->right->d_data = 2;

            root->left = new_l;
            root->left->parent = root;

            root->right = new_r;
            root->right->parent = root;

        }
        else if (strcmp(root->s_data, "^") == 0) {

            if (root->left->type == CONSTANT) {
            
                root->d_data = 0;
                delete[] root->left;
                root->left = NULL;
                delete[] root->right;
                root->right = NULL;
                root->type = CONSTANT;
            }
            else if (root->left->type == VARIOUS || root->left->type == OPER) {
                
                strcpy(root->s_data, "*");

                Node *new_l = new Node;
                new_l->s_data = new char[30];
                Node *new_r = new Node;
                new_r->s_data = new char[30];

                strcpy(new_l->s_data, "^");
        
                strcpy(new_r->s_data, "*");

                Node *l_copyroot = new Node;
                l_copyroot->s_data = new char[30];
                CopyBranch(root->left, l_copyroot);

                Node *r_copyroot = new Node;
                r_copyroot->s_data = new char[30];
                CopyBranch(root->right, r_copyroot);

                Node *l_dif = new Node;
                l_dif->s_data = new char[30];
                CopyBranch(root->left, l_dif);
                Dif(l_dif);

                new_l->left = l_copyroot;
                new_l->left->parent = new_l;

                new_l->right = new Node;
                new_l->type = OPER;
                new_l->right->s_data = new char[30];
                new_l->right->d_data = root->right->d_data - 1;
                new_l->right->type = CONSTANT;
                new_l->right->parent = new_l;

                new_r->left = l_dif;
                new_r->left->parent = new_r;
                new_r->type = OPER;
                new_r->right = r_copyroot;
                new_r->right->parent = new_r;

                root->left = new_l;
                root->left->parent = root;
                root->right = new_r;
                root->right->parent = root;
            }
            else if (root->left->type == VARIOUS) {
                ;
            }
            else {
                ;
            }
            
            
        }
        else if (strcmp(root->s_data, "sin") == 0) {
            
            Node *l_dif = new Node;
            l_dif->s_data = new char[30];
            CopyBranch(root->left, l_dif);
            Dif(l_dif);

            Node *new_l = new Node;
            new_l->s_data = new char[30];
            new_l->type = OPER;

            strcpy(root->s_data, "*");

            strcpy(new_l->s_data, "cos");
            new_l->left = root->left;
            new_l->left->parent = new_l;
            

            root->left = new_l;
            root->left->parent = root;
            root->right = l_dif;
            root->right->parent = root;
        }
        else if (strcmp(root->s_data, "cos") == 0) {
            Node *l_dif = new Node;
            l_dif->s_data = new char[30];
            CopyBranch(root->left, l_dif);
            Dif(l_dif);

            Node *new_l = new Node;
            new_l->s_data = new char[30];
            new_l->type = OPER;

            Node *new_r = new Node;
            new_r->s_data = new char[30];
            new_r->type = OPER;
            strcpy(new_r->s_data, "*");

            new_r->right = new Node;
            new_r->right->s_data = new char[30];
            new_r->right->type = CONSTANT;
            new_r->right->parent = new_r;
            

            strcpy(root->s_data, "*");

            strcpy(new_l->s_data, "sin");
            new_l->left = root->left;
            new_l->left->parent = new_l;
            
            new_r->left = l_dif;
            new_r->left->parent = new_r;
            
            new_r->right->d_data = -1;

            root->left = new_l;
            root->left->parent = root;
            
            root->right = new_r;
            root->right->parent = root;
        }
        else if (strcmp(root->s_data, "ln") == 0) {
            Node *l_dif = new Node;
            l_dif->s_data = new char[30];
            CopyBranch(root->left, l_dif);
            Dif(l_dif);

            strcpy(root->s_data, "/");

            root->right = root->left;
            root->right->parent = root;
            root->left = l_dif;
            root->left->parent = root;

        }
        else {
            printf("ERROR on line %d\n", __LINE__);
        }
    }
    else if (root->type == VARIOUS) {
        
        root->type = CONSTANT;
        
        root->d_data = 1;

        //root->left = root->right = NULL;
        
        return 0;
    }
    else if (root->type == CONSTANT) {
        root->type = CONSTANT;
        root->d_data = 0;
        //root->left = root->right = NULL;
        
        return 0;
    }
    else {
        printf("ERROR on line %d\n", __LINE__);
    }

    return 0;
}

int CopyBranch(Node *root, Node *copyroot)
{
    assert(root != NULL);
    assert(copyroot != NULL);

    copyroot->type = root->type;
    copyroot->parent = root->parent;

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

    fclose(file);
    return 0;
}

int MakeBranch(Node *root, FILE *file)
{
    
    char tmp_symb = fgetc(file);
    double tmp_num = 0;

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
        
    }
    else if (tmp_symb == 'x') {
        
        strcpy(root->left->s_data, "x");
        root->left->type = VARIOUS;
    }
    else if (isdigit(tmp_symb)) {
        ungetc(tmp_symb, file);
        fscanf(file, "%lg", &tmp_num);
        root->left->d_data = tmp_num;
        root->left->type = CONSTANT;
    }
    else if (isalpha(tmp_symb)) {
        if (tmp_symb == 's') {
            
            strcpy(root->left->s_data, "sin");
        }
        else if (tmp_symb == 'c') {
            
            strcpy(root->left->s_data, "cos");
        }
        else if (tmp_symb == 'l') {
            
            strcpy(root->left->s_data, "ln");
        }


        root->left->left = new Node;
        root->left->left->parent = root->left;
        root->left->left->s_data = new char[30];
            
        root->left->type = OPER;

        while(tmp_symb != '(') {
            tmp_symb = fgetc(file);
        }
        
        MakeBranch(root->left->left, file);
        
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
    else if (tmp_symb == ')') {
    
        root->right = root->left->right;
        root->type = root->left->type;
        root->d_data = root->left->d_data;
        strcpy(root->s_data, root->left->s_data);
        root->left = root->left->left;
            
        return 0;
    }
    else {
        printf("ERROR on line %d\n", __LINE__);
        return -1;
    }

    tmp_symb = fgetc(file);

    while(isspace(tmp_symb)) {
        tmp_symb = fgetc(file);
    }

    if (tmp_symb == '(') {
        MakeBranch(root->right, file);
        
    }
    else if (tmp_symb == 'x') {
        strcpy(root->right->s_data, "x");
        root->right->type = VARIOUS;
    }
    else if (isdigit(tmp_symb)) {
        ungetc(tmp_symb, file);
        fscanf(file, "%lg", &tmp_num);
        root->right->d_data = tmp_num;
        root->right->type = CONSTANT;
    }
    else if (isalpha(tmp_symb)) {
        
        if (tmp_symb == 's') {
            
            strcpy(root->right->s_data, "sin");
        }
        else if (tmp_symb == 'c') {
            
            strcpy(root->right->s_data, "cos");
        }
        else if (tmp_symb == 'l') {
            
            strcpy(root->right->s_data, "ln");
        }

        root->right->left = new Node;
        root->right->left->parent = root->right;
        root->right->left->s_data = new char[30];
        
            
        root->right->type = OPER;

        while(tmp_symb != '(') {
            tmp_symb = fgetc(file);
        }

        MakeBranch(root->right->left, file);
        
        tmp_symb = fgetc(file);
    
    }

    tmp_symb = fgetc(file);

    while(isspace(tmp_symb)) {
        tmp_symb = fgetc(file);
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
    if (node == NULL) {
        printf("HERE root == NULL\n");
        return 0;

    }
    
    int tmp_counter = *global_counter;

    if (node->type == VARIOUS || node->type == OPER) {
        fprintf(graph, "\tstruct%d [shape=record,label=\"{<f0>%s | %p | %p}\" ];\n", //fprintf(graph, "\tstruct%d [shape=record,label=\"{<f0>%s | {<f1> left | <f2> right} }\" ];\n", 
        *global_counter, node->s_data, node, node->parent);
        
    }
    else {
        fprintf(graph, "\tstruct%d [shape=record,label=\"{<f0>%lg | %p | %p}\" ];\n", 
        *global_counter, node->d_data, node, node->parent);
    }
    if (node->left == NULL && node->right == NULL) {
        //(*global_counter)++;
        return 0;
    }
    
    

    
    fprintf(graph, "\tstruct%d->struct%d;\n", tmp_counter, tmp_counter + 1);   
    (*global_counter)++;
    
    PrintStruct(graph, node->left, global_counter);
    
    (*global_counter)++;
    
    if (strcmp(node->s_data, "sin") != 0 && strcmp(node->s_data, "cos") != 0 && strcmp(node->s_data, "ln") != 0) {
        
        fprintf(graph, "\tstruct%d->struct%d;\n", tmp_counter, *global_counter );
        
        PrintStruct(graph, node->right, global_counter);
    }

    
    return 0;
}
