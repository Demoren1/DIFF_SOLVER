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

    for (int i = 0; i < buff.size; i++)
    {
        printf("%c", buff.buffer[i]);
    }
    puts(" ");

    char *str = buff.buffer;
    
    printf("str = %s\n", buff.buffer);

    tree.root = get_General(str);

    Node *cool_node = node_copy_node(tree.root);

    diff_simplify(cool_node);

    Node *diff_cool_node = diff_diff(cool_node, 'x');
        
    diff_simplify(diff_cool_node);

    node_dtor(cool_node);
    node_dtor(diff_cool_node);
    node_dtor_calloc_data(tree.root, buff.buffer, buff.size);
    node_dtor(tree.root);
    diff_buff_dtor(&buff);
    close_tree_logs();
    
    open_log_pdf();
    return 0;
}