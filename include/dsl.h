#ifndef DSL_GUARD
#define DSL_GUARD

#define NL node->l_son
#define NR node->r_son

#define NNL new_node->l_son
#define NNR new_node->r_son

#define CL node_copy_node(NL)
#define CR node_copy_node(NR)

#define DL diff_diff(NL)
#define DR diff_diff(NR)

#define LINK_WITH_OP(operation)                                                             \
                                Node *new_node = 0;                                         \
                                new_node = Create_OP_node(operation);                       \
                                node_connect(new_node, l_node, LEFT);                       \
                                node_connect(new_node, r_node, RIGHT);                      \
                                return new_node;

Node *Diff_ADD(Node *node, Operation oper_value);

Node *Diff_MUL(Node *node, Operation oper_value);

Node *Diff_DIV(Node *node, Operation oper_value);

Node *ADD_OP(Node *l_node, Node *r_node);

Node *Create_OP_node(Operation oper_value);

Node *Create_NUM_node(double dbl_value);

Node *Create_VAR_node(const char var);

#endif    