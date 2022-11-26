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

#define MAX_VARS 128

int diff_handle_src(const char* path_to_file, Buffer *buff);

int diff_size_for_buffer(FILE *BASE, const char *path_to_file);

int diff_buff_dtor(Buffer *buffer);

int diff_do_tree(Node *node, Buffer *buff);

Node *_diff_node_ctor(Type_of_expression type, double dbl_value, Operation op_value, char var_value);

Node *diff_connect_node(Node *parent, Node *new_node);

Node *diff_diff(Node *node, char var_for_diff);

int diff_simplify(Node *node);

Operation diff_get_operation(const char *buffer);

Priorities find_op_priority(Operation operation);

int diff_ctor_var_arr(Node *node, Var *var_arr, int cur_index);

double diff_calc_tree(Node *node, Var var_arr[], int n_vars);

#endif