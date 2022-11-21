#include <stdio.h>
#include <tree_funcs.h>
#include <diff_funcs.h>
#include <dsl.h>

Node *Diff_ADD(Node *node, Operation oper_value)
{
    Node *new_node = 0;
    Create_OP_new_node(new_node, oper_value);               
    NNL = DL;                                        
    NNR = DR;                                        
    NNL->parent = new_node;                          
    NNR->parent = new_node;

    return new_node;
}

Node *Diff_MUL(Node *node, Operation oper_value)
{
    // ADD( MUL(DIFF(LEFT), RIGHT), MUL(DIFF(RIGHT), LEFT) );
    Node *new_node = 0;

    Create_OP_new_node(new_node, oper_value);  
                                
    Node *l_node = 0;                   
    Node *r_node = 0;                   
                                        
    Create_OP_new_node(l_node, MUL);    
    Create_OP_new_node(r_node, MUL);    
                                        
    node_connect(new_node, l_node, LEFT);   
    node_connect(new_node, r_node, RIGHT);  
                                            
    Node *lr_node = CR;                 
    Node *rl_node = CL;                 
    Node *ll_node = DL;                 
    Node *rr_node = DR;                 
                                        
    node_connect(l_node, ll_node, LEFT);    
    node_connect(l_node, lr_node, RIGHT);   
                                            
    node_connect(r_node, rl_node, LEFT);    
    node_connect(r_node, rr_node, RIGHT);

    return new_node;
}

Node *Diff_DIV(Node *node, Operation oper_value)
{
    Node *new_node = 0;

    Node *l_node = 0;                       
    Node *r_node = 0;                       
                                            
    Create_OP_new_node(new_node, DIV);      
    Create_OP_new_node(r_node, MUL);     
    l_node = Diff_MUL(node, oper_value);
                                            
    node_connect(new_node, l_node, LEFT);   
    node_connect(new_node, r_node, RIGHT);  
                                            
    Node *rr_node = CR;                     
    Node *rl_node = CR;                     
                                            
    node_connect(r_node, rl_node, LEFT);    
    node_connect(r_node, rr_node, RIGHT);

    return new_node;
}

#if 0

#define Diff_DIV(new_node)                                      \
                        Node *l_node = 0;                       \
                        Node *r_node = 0;                       \
                                                                \
                        Create_OP_new_node(new_node, DIV);      \
                        Create_OP_new_node(r_node, MUL);        \
                        Diff_MUL(l_node, SUB)                   \
                                                                \
                        node_connect(new_node, l_node, LEFT);   \
                        node_connect(new_node, r_node, RIGHT);  \
                                                                \
                        Node *rr_node = CR;                     \
                        Node *rl_node = CR;                     \
                                                                \
                        node_connect(r_node, rl_node, LEFT);    \
                        node_connect(r_node, rr_node, RIGHT);   


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



#define Diff_ADD(new_node)                               \
        Create_OP_new_node(new_node, ADD);               \
        NNL = DL;                                        \
        NNR = DR;                                        \
        NNL->parent = new_node;                          \
        NNR->parent = new_node;

#endif