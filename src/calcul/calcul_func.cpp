#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>
#include <tree_funcs.h>
#include <tree_debug.h>
#include <diff_funcs.h>
#include <general_debug.h>
#include <dsl.h>
#include <calcul_funcs.h>

const char* STR = NULL;

Node* get_General(const char *str)
{
    STR = str;

    Node* node = get_Expression();

    assert(*STR == '\0');

    return node;
}

Node *get_VAR()
{
    Node *node = 0;

    node = Create_VAR_node(*STR);

    while(*STR != '\0' && isalpha(*STR))
        STR++;
    
    return node;
}

Node* get_Num()
{
    int val = 0;
    const char *old_s = STR;
    printf("char = %c\n", *STR);
    while (isdigit(*STR))
    {   
        val = val*10 + *STR -'0';
        STR++;
    }

    assert(STR != old_s);

    Node *node = Create_NUM_node((double) val);
    return node;
}

Node* get_P()
{
    Node *node = 0;

    if (*STR == '(')
    {
        STR++;
        node = get_Expression();
        assert (*STR == ')');
        STR++;
    }
    else if (isalpha(*STR))
    {
        node = get_VAR();
    }
    else
    {
        node = get_Num();
    }

    return node;
}

Node* get_T()
{
    Node *node = 0;
    node = get_Degree();

    while(*STR == '*' || *STR == '/')
    {
        int op = *STR;

        STR++;
        Node* r_node = get_Degree();
        
        if (op == '*')
        {
            Node *new_node = Create_OP_node(MUL);
            node_connect(new_node, node, LEFT);
            node_connect(new_node, r_node, RIGHT);
            node = new_node;
        }
        else 
        {
            Node *new_node = Create_OP_node(DIV);
            node_connect(new_node, node, LEFT);
            node_connect(new_node, r_node, RIGHT);
            node = new_node;
        }
    }

    return node;
}

Node* get_Expression()
{
    Node* node = 0;
    node = get_T();

    while(*STR == '+' || *STR == '-')
    {
        int op = *STR;

        STR++;
        Node* r_node = get_T();
        
        if (op == '+')
        {
            Node *new_node = Create_OP_node(ADD);
            node_connect(new_node, node, LEFT);
            node_connect(new_node, r_node, RIGHT);
            node = new_node;
        }
         
        else 
        {
            Node *new_node = Create_OP_node(SUB);
            node_connect(new_node, node, LEFT);
            node_connect(new_node, r_node, RIGHT);
            node = new_node;
        }
    }

    return node;
}

Node* get_Degree()
{
    Node *node = 0;

    node = get_P();

    while(*STR == '^')
    {
        STR++;
        Node *r_node = get_P();

        {
            Node *new_node = Create_OP_node(DEGREE);
            node_connect(new_node, node, LEFT);
            node_connect(new_node, r_node, RIGHT);
            node = new_node;
        }
    }

    return node;
}
