#ifndef DIFF_FUNCS_GUARD
#define DIFF_FUNCS_GUARD
#include <stdio.h>
#include <tree_funcs.h>
#include <general_debug.h>

typedef struct Buffer_t
{
    char *buffer;
    size_t size;
    int curr_index;
} Buffer;

int diff_handle_src(const char* path_to_file, Buffer *buff);

int diff_size_for_buffer(FILE *BASE, const char *path_to_file);

int diff_buff_dtor(Buffer *buffer);

int diff_do_tree(Node *node, Buffer *buff);

Node *_diff_node_ctor(Type_of_expression type, double dbl_value, Operation op_value, char var_value);

Node *diff_connect_node(Node *parent, Node *new_node);

Node *diff_diff(Node *node);

#define NL node->l_son
#define NR node->r_son

#define NNL new_node->l_son
#define NNR new_node->r_son

#define CL node_copy_node(NL)
#define CR node_copy_node(NR)

#define DL diff_diff(NL);
#define DR diff_diff(NR);

#define Create_OP_new_node(new_node, oper_value)        \
            new_node = node_ctor();                     \
            new_node->value.op_value = oper_value;      \
            new_node->type = OP;                        \

#define Diff_ADD(new_node)                               \
        Create_OP_new_node(new_node, ADD);               \
        NNL = DL;                                        \
        NNR = DR;                                        \
        NNL->parent = new_node;                          \
        NNR->parent = new_node;

#define Diff_SUB(new_node)                               \
        Create_OP_new_node(new_node, SUB);               \
        NNL = DL;                                        \
        NNR = DR;                                        \
        NNL->parent = new_node;                          \
        NNR->parent = new_node;

#define Diff_MUL(new_node, oper_val)                   \
                    Create_OP_new_node(new_node, oper_val);  \
                                                       \
                    Node *__l_node = 0;                   \
                    Node *__r_node = 0;                   \
                                                        \
                    Create_OP_new_node(__l_node, MUL);    \
                    Create_OP_new_node(__r_node, MUL);    \
                                                        \
                    node_connect(new_node, __l_node, LEFT);   \
                    node_connect(new_node, __r_node, RIGHT);  \
                                                            \
                    Node *__lr_node = CR;                 \
                    Node *__rl_node = CL;                 \
                    Node *__ll_node = DL;                 \
                    Node *__rr_node = DR;                 \
                                                        \
                    node_connect(__l_node, __ll_node, LEFT);    \
                    node_connect(__l_node, __lr_node, RIGHT);   \
                                                            \
                    node_connect(__r_node, __rl_node, LEFT);    \
                    node_connect(__r_node, __rr_node, RIGHT);   

#endif