#ifndef TREE_GUARD
#define TREE_GUARD
#include <math.h>

const int LEN_OF_DATA = 256;

#define FUNC_GENERAL_INFO(object)  __FILE__, __FUNCTION__, #object, __LINE__

#define TREE_CTOR(tree) tree_ctor(&list, FUNC_GENERAL_INFO(tree))

#define POISON "POISON"

typedef enum 
{   
    NOT_SIDE    = -1,
    LEFT        = 1,
    RIGHT       = 2
}Pos_of_node;

typedef enum 
{
    OP  = 1,
    VAR = 2,
    NUM = 3
} Type_of_expression;

typedef enum 
{   
    NOT_OP  = 0,
    ADD     = 1,
    SUB     = 2,
    MUL     = 3,
    DIV     = 4,
    DEGREE  = 5,
    LN      = 6,
    SIN     = 7,
    COS     = 8,
    TG      = 9
} Operation;

union Value
{
    double dbl_value;
    Operation op_value;
    char var_value;
};

typedef enum 
{   
    ERROR_PRIOR     = -999,
    NUM_PRIOR       = 1,
    VAR_PRIOR       = 1,
    ADD_PRIOR       = 1,
    SUB_PRIOR       = 1,
    MUL_PRIOR       = 2,
    DIV_PRIOR       = 3,
    DEGREE_PRIOR    = 5,
    UNAR_OP_PRIOR   = 10
} Priorities;

typedef struct Node_t
{    
    Type_of_expression type;

    Value value;

    Node_t *parent;
    Node_t *l_son;
    Node_t *r_son;
    Pos_of_node pos;

    Priorities priority;

} Node;

typedef struct Tree_t
{
    Node* root;
} Tree;

int tree_ctor(Tree *tree);

Node *node_ctor();

Node* node_connect(Node *parent, Node *node, Pos_of_node pos);

Node *node_disconnect (Node *parent, Pos_of_node pos, int dtor_flag);

Node* node_ctor_connect(Node *parent, Pos_of_node pos);

int node_dtor(Node *node);

int node_dtor_calloc_data(Node *node, const char* buffer, int size);

int tree_show_graph();

Node* node_copy_node(Node *node);

int node_copy_data(Node *new_node, Node *old_node);

int node_switch_sons(Node *node);


#endif



