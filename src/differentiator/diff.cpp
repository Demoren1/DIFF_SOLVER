#include <stdio.h>
#include <tree_funcs.h>
#include <tree_debug.h>
#include <diff_funcs.h>
#include <calcul_funcs.h>


int main()
{
    open_tree_logs();

    Tree tree = {};
    Buffer buff = {};

    const char *src = "expression.txt";
    diff_handle_src(src, &buff);

    char *str = buff.buffer;
    
    printf("str = %s\n", buff.buffer);

    tree.root = get_General(str);

    Node *cool_node = node_copy_node(tree.root);
    diff_simplify(cool_node);

    Var var_arr[MAX_VARS] = {};
    int n_vars = diff_ctor_var_arr(cool_node, var_arr, 0);
    double result = 0;
    result = diff_calc_tree(cool_node, var_arr, n_vars);    
    double tailor_row_result = diff_tailor_one_var(cool_node, 5, 'x', 0.5, 0);

    printf("vars = %d, result = %g\n",n_vars, result);
    printf("Tailor row result = %g\n", tailor_row_result);
    Node *diff_cool_node = diff_diff(cool_node, 'y');
    diff_simplify(diff_cool_node);

    node_dtor(cool_node);
    node_dtor(diff_cool_node);
    node_dtor_calloc_data(tree.root, buff.buffer, buff.size);
    node_dtor(tree.root);
    diff_buff_dtor(&buff);
    open_log_pdf();
    close_tree_logs();
    
    return 0;
}