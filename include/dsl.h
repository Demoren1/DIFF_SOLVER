#ifndef DSL_GUARD
#define DSL_GUARD

#define NL node->l_son
#define NR node->r_son

#define NLV node->l_son->value
#define NRV node->r_son->value

#define NNL new_node->l_son
#define NNR new_node->r_son

#define CL node_copy_node(NL)
#define CR node_copy_node(NR)

#define DL diff_diff(NL)
#define DR diff_diff(NR)

#define LINK_WITH_OP(operation)                                                             \
                                Node *new_node = 0;                                         \
                                new_node = Create_OP_node(operation);                       \
                                if (l_node != NULL)                                         \
                                    node_connect(new_node, l_node, LEFT);                   \
                                if (r_node != NULL)                                         \
                                    node_connect(new_node, r_node, RIGHT);                  \
                                return new_node;

Node *Diff_ADD(Node *node, Operation oper_value);

Node *Diff_MUL(Node *node, Operation oper_value);

Node *Diff_DIV(Node *node, Operation oper_value);

Node *Diff_DEGREE(Node *node);

Node *Diff_LN(Node *node);

Node *Diff_SIN(Node *node);

Node *Diff_COS(Node *node);

Node *Diff_TG(Node *node);

Node *Create_OP_node(Operation oper_value);

Node *Create_NUM_node(double dbl_value);

Node *Create_VAR_node(const char var);

Node *ADD_OP(Node *l_node, Node *r_node);

Node *SUB_OP(Node *l_node, Node *r_node);

Node *MUL_OP(Node *l_node, Node *r_node);

Node *DIV_OP(Node *l_node, Node *r_node);

Node *DEGREE_OP(Node *l_node, Node *r_node);

Node *LN_OP(Node *l_node, Node *r_node);

Node *COS_OP(Node *l_node, Node *r_node);

Node *SIN_OP(Node *l_node, Node *r_node);

Node *TG_OP(Node *l_node, Node *r_node);

#endif    