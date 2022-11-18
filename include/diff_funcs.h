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

#define diff_node_ctor(type, value, new_node) if (type == OP)                                 \
                                                new_node = _diff_node_ctor(type, NAN, value, ' ');       \
                                            else if (type == VAR)                           \
                                                new_node = _diff_node_ctor(type, NAN, NOT_OP, value);  \
                                            else if (type == NUM)                           \
                                                new_node = _diff_node_ctor(type, value, NOT_OP, ' ');


int diff_handle_src(const char* path_to_file, Buffer *buff);

int diff_size_for_buffer(FILE *BASE, const char *path_to_file);

int diff_buff_dtor(Buffer *buffer);

int diff_do_tree(Node *node, Buffer *buff);

Node *_diff_node_ctor(Type_of_expression type, double dbl_value, Operation op_value, char var_value);

Node *diff_connect_node(Node *parent, Node *new_node);

#endif