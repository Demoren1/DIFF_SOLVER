#include <stdio.h>
#include <tree_funcs.h>
#include <tree_debug.h>
#include <diff_funcs.h>


int main()
{
    open_tree_logs();

    Tree tree = {};
    tree_ctor(&tree);

    Buffer buff = {};

    const char *src = "expression.txt";
    diff_handle_src(src, &buff);

    for (int i = 0; i < buff.size; i++)
    {
        printf("%c", buff.buffer[i]);
    }
    puts(" ");

    // printf("tree.root address = %p\n", tree.root);
    diff_do_tree(tree.root, &buff);

    Node *cool_node = node_copy_node(tree.root);
    TREE_DUMP(cool_node, INORDER);

    node_dtor(cool_node);
    node_dtor_calloc_data(tree.root, buff.buffer, buff.size);
    node_dtor(tree.root);
    diff_buff_dtor(&buff);
    close_tree_logs();
    return 0;
}