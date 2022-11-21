#ifndef DSL_GUARD
#define DSL_GUARD

#define NL node->l_son
#define NR node->r_son

#define NNL new_node->l_son
#define NNR new_node->r_son

#define CL node_copy_node(NL)
#define CR node_copy_node(NR)

#define DL diff_diff(NL);
#define DR diff_diff(NR);

Node *Diff_ADD(Node *node, Operation oper_value);

Node *Diff_MUL(Node *node, Operation oper_value);

Node *Diff_DIV(Node *node, Operation oper_value);

#define Create_OP_new_node(new_node, oper_value)                \
            new_node = node_ctor();                             \
            new_node->value.op_value = oper_value;              \
            new_node->type = OP;                                \
            new_node->priority = find_op_priority(oper_value);

#endif    