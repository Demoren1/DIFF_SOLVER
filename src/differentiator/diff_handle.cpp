#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>
#include <tree_funcs.h>
#include <tree_debug.h>
#include <diff_funcs.h>
#include <general_debug.h>
#include <dsl.h>

static int del_new_line_sym(Buffer *buffer);

static int input_val_type(Node *node, Buffer *buff);

static int create_node_if_need(char cur_sym, Node* node, Buffer *buff);

static int exit_node_if_need(char cur_sym, Buffer *buff);

static int switch_sons_if_need(Node *node);

static Node *diff_node_ctor(Type_of_expression type, const void *value);

static Operation get_operation(const char * buffer);

static int len_op(const char *operation);

static FILE* SRC = 0;

int diff_handle_src(const char* path_to_file, Buffer *buff)
{   
    SOFT_ASS(path_to_file == NULL);
    SOFT_ASS(buff == NULL);
    
    SRC = fopen(path_to_file, "rw+");

    SOFT_ASS(SRC == NULL);

    buff->size = diff_size_for_buffer(SRC, path_to_file);

    buff->buffer = (char*) calloc(buff->size, sizeof(char));        
    SOFT_ASS(buff->buffer == NULL);

    int test_fread = 0;
    test_fread = fread(buff->buffer, sizeof(char), buff->size, SRC);      
    SOFT_ASS(test_fread == 0);  

    del_new_line_sym(buff);

    buff->buffer[buff->size] = '\0';
    buff->curr_index = 0;

    return 0;
}

int diff_size_for_buffer(FILE *SRC, const char *path_to_file)
{
    SOFT_ASS(SRC == NULL);
    SOFT_ASS(path_to_file == NULL);

    struct stat buf ={};
    stat(path_to_file, &buf);

    return buf.st_size + 1;
}

static int del_new_line_sym(Buffer* buff)
{
    SOFT_ASS(buff == NULL);
    SOFT_ASS(buff->buffer == NULL);

    for (int i = 0; i < buff->size; i++)
    {
        if (buff->buffer[i] == '\n')
        {
            buff->buffer[i] = '\0';
        }
    }

    return 0;
}

int diff_buff_dtor(Buffer *buffer)
{   
    if (SRC != NULL)
    {
        fclose(SRC);
        SRC = NULL;
    }
    if (buffer->buffer != NULL)
    {
        free(buffer->buffer);
        buffer->buffer = NULL;
    }  

    return 0;
}

int diff_do_tree(Node *node, Buffer *buff)
{   
    char cur_sym = buff->buffer[buff->curr_index];
    int cur_pos = buff->curr_index;

    if (node->parent == NULL)
    {
        input_val_type(node, buff);
        create_node_if_need(cur_sym, node, buff);
    }

    if(node->parent != NULL)
        create_node_if_need(cur_sym, node, buff);

    input_val_type(node, buff);

    cur_sym = buff->buffer[buff->curr_index];
    cur_pos = buff->curr_index;

    if (exit_node_if_need(cur_sym, buff))
    {
        switch_sons_if_need(node);
        return 0;
    }
    
    create_node_if_need(cur_sym, node, buff);

    input_val_type(node, buff);

    cur_sym = buff->buffer[buff->curr_index];
    cur_pos = buff->curr_index;

    if (exit_node_if_need(cur_sym, buff))
    {
        switch_sons_if_need(node);
        return 0;
    }

    switch_sons_if_need(node);
    return 0;
}

static int input_val_type(Node *node, Buffer *buff)
{
    int cur_sym = buff->buffer[buff->curr_index];
    int cur_pos = buff->curr_index;
                                                                                                //todo dint check twice
    if (isalpha(cur_sym) && (NOT_OP == get_operation(buff->buffer + cur_pos)))
    {   
        node->value.var_value = cur_sym;
        node->priority = VAR_PRIOR;
        
        buff->curr_index++;
        node->type = VAR;
    }
    else if(isdigit(cur_sym))
    {
        int shift = 0;
        node->type = NUM;
        node->priority = NUM_PRIOR;

        sscanf(buff->buffer + cur_pos, "%lf%n", &(node->value.dbl_value), &shift);
    
        buff->curr_index += shift;
    }
    else if (NOT_OP != get_operation(buff->buffer + cur_pos))
    {
        node->type = OP;
        
        node->value.op_value = get_operation(buff->buffer + cur_pos);
        node->priority = find_op_priority(node->value.op_value);
        int len = len_op(buff->buffer + cur_pos);

        buff->curr_index += len;
    }

    return 0;
}

static int create_node_if_need(char cur_sym, Node* node, Buffer *buff)
{
    if (cur_sym == '(')
    {
        buff->curr_index++;
        Node *new_node1 = node_ctor();
        diff_connect_node(node, new_node1);
        diff_do_tree(new_node1, buff);
    }

    return 0;
}

static int exit_node_if_need(char cur_sym, Buffer *buff)
{
    if (cur_sym == ')')
    {
        buff->curr_index++;
        return 1;
    }

    return 0;
}


Node *diff_connect_node(Node *parent, Node *new_node)
{
    new_node->parent = parent;
    
    if (parent->l_son == NULL)                 
    {                 
        parent->l_son = new_node;       
    }                                   
    else if(parent->r_son == NULL)             
    {                                
        parent->r_son = new_node;       
    }                                   
    else    SOFT_ASS_NO_RET(1);

    return new_node;
}
#if 0
Node *diff_node_ctor(Type_of_expression type, const void *v_value)
{   
    if (type == OP)
    {                                                
        Operation *value = (Operation*) value;     
        return _diff_node_ctor(type, NAN, *value, ' ');
    }     
    else if (type == VAR)                                           
    {
        char *value = (char*) v_value;
        return _diff_node_ctor(type, NAN, NOT_OP, *value);       
    }
    else if (type == NUM)                                           
     {   
        double *value = (double*) v_value; 
        return _diff_node_ctor(type, *value, NOT_OP, ' ');       
     }

     return 0;
}


Node *_diff_node_ctor(Type_of_expression type, double dbl_value, Operation op_value, char var_value)
{
    Node *node = node_ctor();
    SOFT_ASS_NO_RET(node == NULL);
    node->type = type;

    if (dbl_value != NAN)
    {
        node->priority = NUM_PRIOR;
        node->value.dbl_value = dbl_value;
    }
    else if (op_value != NOT_OP)
    {
        node->priority = find_op_priority(op_value);
        node->value.op_value = op_value;
    }
    else if(var_value != ' ')
    {
        node->priority = VAR_PRIOR;
        node->value.var_value = var_value;
    }
    else SOFT_ASS_NO_RET(1);

    return node;   
}

#endif

Node *diff_diff(Node *node)
{
    Type_of_expression type = node->type;
    Node *new_node = 0;
    switch(type)
    {
        case NUM:
        {   
            double value = 0;
            return Create_NUM_node(value);
        }
        case VAR:
        {   
            double value = 1;
            return Create_NUM_node(value);
        }
        case OP:
        {
            
            switch (node->value.op_value)
            {
                case ADD:
                {   
                    return Diff_ADD(node, ADD);
                }
                case SUB:
                {   
                    return Diff_ADD(node, SUB);;
                }
                case MUL:
                {   
                    return Diff_MUL(node, ADD);
                }
                case DIV:
                {   
                    return Diff_DIV(node, SUB); 
                }
            }
            break;
        }
    }
    return 0;
}

Priorities find_op_priority(Operation operation)
{   
    switch(operation)
    {
        case ADD:
            return ADD_PRIOR;
        case SUB:
            return SUB_PRIOR;
        case MUL:
            return MUL_PRIOR;
        case DIV:
            return DIV_PRIOR;
        case DEGREE:
            return DEGREE_PRIOR;
        case LN:
        case COS:
        case SIN:
        case TG:
            return UNAR_OP_PRIOR;
        default:    
        {
            SOFT_ASS_NO_RET(1);
            return ERROR_PRIOR;
        }
    }

    return ERROR_PRIOR;
}

static Operation get_operation(const char *buff)
{
    char op_sym = buff[0];

    if (strstr(buff, "ln") == buff)
        return LN;
    else if (strstr(buff, "sin") == buff)
        return SIN;
    else if (strstr(buff, "cos") == buff)
        return COS;
    else if (strstr(buff, "tg") == buff)
        return TG;


    switch (op_sym)
    {
        case '+':
            return ADD;
        case '-':
            return SUB;
        case '*':
            return MUL;
        case '/':
            return DIV;
        case '^':
            return DEGREE;   
    }
    
    return NOT_OP;
}

static int len_op(const char *operation)
{
    int index = 0;
    
    if (!isalpha(*operation))
        return 1;
    else
    {
        while (isalpha(*(operation + index)))
            index++;
    }

    return index;
}

static int switch_sons_if_need(Node *node)
{
    if (node->type != OP)
        return 0;

    switch (node->value.op_value)
    {
        case LN:
        case COS:
        case SIN:
        case TG:
        {
            node_switch_sons(node);
            DBG;
        }
    }

    return 0;
}