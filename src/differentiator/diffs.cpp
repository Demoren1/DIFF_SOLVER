#include <stdio.h>
#include <tree_funcs.h>
#include <diff_funcs.h>
#include <dsl.h>


Node *Create_OP_node(Operation oper_value)
{                
    Node *new_node = node_ctor();                             
    new_node->value.op_value = oper_value;              
    new_node->type = OP;                                
    new_node->priority = find_op_priority(oper_value);

    return new_node;
}

Node *Create_NUM_node(double dbl_value)
{
    Node *new_node = node_ctor();                             
    new_node->value.dbl_value = dbl_value;              
    new_node->type = NUM;                                
    new_node->priority = NUM_PRIOR;

    return new_node;
}

Node *Create_VAR_node(const char var)
{
    Node *new_node = node_ctor();                             
    new_node->value.var_value = var;              
    new_node->type = VAR;                                
    new_node->priority = VAR_PRIOR;

    return new_node;
}


Node *ADD_OP(Node *l_node, Node *r_node)
{LINK_WITH_OP(ADD);}

Node *SUB_OP(Node *l_node, Node *r_node)
{LINK_WITH_OP(SUB);}

Node *MUL_OP(Node *l_node, Node *r_node)
{LINK_WITH_OP(MUL);}

Node *DIV_OP(Node *l_node, Node *r_node)
{LINK_WITH_OP(DIV);}

Node *DEGREE_OP(Node *l_node, Node *r_node)
{LINK_WITH_OP(DEGREE);}

Node *Diff_ADD(Node *node, Operation oper_value)
{
    return ADD_OP(DL, DR);
}

Node *Diff_MUL(Node *node, Operation oper_value)
{
    return ADD_OP(MUL_OP(DL, CR), MUL_OP(CL, DR));
}

Node *Diff_DIV(Node *node, Operation oper_value)
{   
    
    return DIV_OP(SUB_OP(MUL_OP(DL, CR), MUL_OP(DR, CL)), MUL_OP(CR, CR));
}

#if 0

Node *Diff_DIV(Node *node, Operation oper_value)
{
    Node *new_node = 0;

    Node *l_node = 0;                       
    Node *r_node = 0;                       
                                            
    Create_OP_node(new_node, DIV);      
    Create_OP_node(r_node, MUL);     
    l_node = Diff_MUL(node, oper_value);
                                            
    node_connect(new_node, l_node, LEFT);   
    node_connect(new_node, r_node, RIGHT);  
                                            
    Node *rr_node = CR;                     
    Node *rl_node = CR;                     
                                            
    node_connect(r_node, rl_node, LEFT);    
    node_connect(r_node, rr_node, RIGHT);

    return new_node;
}

#endif 