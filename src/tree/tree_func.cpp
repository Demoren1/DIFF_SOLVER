#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <tree_funcs.h>
#include <tree_debug.h>

int tree_ctor(Tree *tree)
{   
    tree->root = node_ctor(0);

    return 0;
}

Node* node_ctor(bool flag_on_data)
{
    Node *node = (Node*) calloc(1, sizeof(Node));
    SOFT_ASS_NO_RET(node == NULL);

    if (flag_on_data)
    {
        node->data = (char*) calloc(LEN_OF_DATA + 1, sizeof(char));
        SOFT_ASS_NO_RET(node->data == NULL);

        puts("Input data");
        fgets(node->data, LEN_OF_DATA, stdin);
    }

    return node;
}

Node *node_ctor_connect(Node *parent, Pos_of_node pos)
{   
    SOFT_ASS_NO_RET(parent == NULL);

    Node *node = node_ctor(0);
    SOFT_ASS_NO_RET(node == NULL);

    node_connect(parent, node, pos);

    return node;
}

Node* node_connect(Node *parent, Node *node, Pos_of_node pos)
{
    SOFT_ASS_NO_RET(parent == NULL);
    SOFT_ASS_NO_RET(node == NULL);

    switch (pos)       
    {
        case LEFT:
        {   
            SOFT_ASS_NO_RET(parent->l_son != NULL)
            parent->l_son = node;
            node->pos = LEFT;
            
            break;
        }
        case RIGHT:
        {
            SOFT_ASS_NO_RET(parent->r_son != NULL)
            parent->r_son = node;
            node->pos = RIGHT;

            break;
        }

        default:
            printf("You make a mistake in input of pos\n");
            break;
    }
    
    node->parent = parent;
    
    return node;
}

int node_dtor(Node* node)
{
    SOFT_ASS(node == NULL);

    if (node->l_son != NULL)
    {
        node_dtor(node->l_son);
        node->l_son = NULL;
    }

    if (node->r_son != NULL)
    {
        node_dtor(node->r_son);
        node->r_son = NULL;
    }
    
    node->data = NULL;
    free(node);
    node = NULL;

    return 0;
}

int node_dtor_calloc_data(Node *node, const char* buffer, int size)
{
    if (node->l_son != NULL)
    {
        node_dtor_calloc_data(node->l_son, buffer, size);
    }

    if (node->r_son != NULL)                                        //todo 1 traverse on tree
    {
        node_dtor_calloc_data(node->r_son, buffer, size);
    }

    if (fabs(buffer - (node->data)) > size)
    {
        free(node->data);
    }

    return 0;
}