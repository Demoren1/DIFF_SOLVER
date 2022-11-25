#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <tree_funcs.h>
#include <tree_debug.h>
#include <general_debug.h>

static int connect_with_parent(Node *parent, Node *new_node);

int tree_ctor(Tree *tree)
{   
    tree->root = node_ctor();

    return 0;
}

Node* node_ctor()
{
    Node *node = (Node*) calloc(1, sizeof(Node));
    SOFT_ASS_NO_RET(node == NULL);

    node->value.dbl_value = NAN;
    node->value.op_value = NOT_OP;
    node->value.var_value = ' ';

    return node;
}

Node *node_ctor_connect(Node *parent, Pos_of_node pos)
{   
    SOFT_ASS_NO_RET(parent == NULL);

    Node *node = node_ctor();
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

Node *node_disconnect(Node *parent, Pos_of_node pos, int dtor_flag)
{
    SOFT_ASS_NO_RET(parent == NULL);

     switch (pos)       
    {
        case LEFT:
        {   
            if (parent->l_son == NULL)
                break;
            
            parent->l_son->parent = NULL;
            if (dtor_flag) node_dtor(parent->l_son);
            parent->l_son = NULL;            
            break;
        }
        case RIGHT:
        {   
            if (parent->r_son == NULL)
                break;

            parent->r_son->parent = NULL;
            if (dtor_flag) node_dtor(parent->r_son);
            parent->r_son = NULL;            

            break;
        }

        default:
            printf("You make a mistake in input of pos\n");
            break;
    }

    return parent;
}

int node_dtor(Node *node)
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

    if (node->r_son != NULL)                                      
    {
        node_dtor_calloc_data(node->r_son, buffer, size);
    }
    
    #if 0
    if (fabs(buffer - (node->data)) > size)
    {
        free(node->data);
    }
    #endif

    return 0;
}

Node *node_copy_node(Node *node)
{   
    if (!node)
    {
        return 0;
    }

    Node *new_node = node_ctor();
    node_copy_data(new_node, node);
    
    return new_node;
}

static int connect_with_parent(Node *parent, Node *new_node)
{
    if (parent)
    {
        new_node->parent = parent;
    }

    return 0;
}

int node_copy_data(Node *new_node, Node *old_node)
{
    new_node->value = old_node->value;

    new_node->priority = old_node->priority;
    new_node->type = old_node->type;
    new_node->pos = old_node->pos;
    

    if (old_node->l_son)
    {
        node_ctor_connect(new_node, LEFT);
        node_copy_data(new_node->l_son, old_node->l_son);
    }
    if (old_node->r_son)
    {
        node_ctor_connect(new_node, RIGHT);
        node_copy_data(new_node->r_son, old_node->r_son);
    }

    return 0;
}

int node_switch_sons(Node *node)
{
    if (node == NULL)
        return 0;

    Node *tmp_node = 0;

    tmp_node = node->l_son;
    node->l_son = node->r_son;
    node->r_son = tmp_node;

    return 0;
}