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

static int wrap_equivalents(Node *node);

static Pos_of_node find_zero_side(Node *node);

static Pos_of_node find_one_side(Node *node);

static int kill_zero_side(Pos_of_node zero_side, Node *node);

static int kill_one_side(Pos_of_node zero_side, Node *node);

static int del_new_line_sym(Buffer *buffer);

static int input_val_type(Node *node, Buffer *buff);

static int create_node_if_need(char cur_sym, Node* node, Buffer *buff);

static int exit_node_if_need(char cur_sym, Buffer *buff);

static int switch_sons_if_need(Node *node);

static double count_num(Node *node, Node *l_son, Node *r_son);

static Node *diff_node_ctor(Type_of_expression type, const void *value);

static Operation get_operation(const char * buffer);

static int len_op(const char *operation);

static int compute_constants(Node *node);

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

int diff_simplify(Node *node)
{
    int changes = 1;

    while(changes > 0)
    {       
        TREE_DUMP(node, INORDER);
        changes = 0;
        changes += compute_constants(node);
        changes += wrap_equivalents(node);
        TREE_DUMP(node, INORDER);
        // changes = 0;
    }

    return changes;
}

int wrap_equivalents(Node *node)
{
    int changes = 0;
    if(!node)
        return changes;

    Pos_of_node zero_side = find_zero_side(node);    

    if (zero_side != NOT_SIDE)
    {
        changes++;
        kill_zero_side(zero_side, node);
    }
    
    Pos_of_node one_side = find_one_side(node);
    if (one_side != NOT_SIDE)
    {   
        changes += kill_one_side(one_side, node);
    }

    if (node->l_son && node->l_son->type == OP)
    {
        changes += wrap_equivalents(node->l_son);
    }

    if (node->r_son && node->r_son->type == OP)
    {   
        changes += wrap_equivalents(node->r_son);
    }

    return changes;
}

static int kill_one_side(Pos_of_node one_side, Node *node)
{  
    Node *one_node = 0;
    Node *conjugate_node = 0;
    Node *parent = node->parent;
    Node *tmp_node = node;
    int changes = 0;
    int fake_parent_flag = 0;

    if (one_side == LEFT)
    {
        one_node = NL;
        conjugate_node = NR;
    }
    else
    {
        one_node = NR;
        conjugate_node = NL;
    }

    if (conjugate_node == NULL)
        return changes;

    Operation oper = node->value.op_value;    
    
    if (!node->parent)
        fake_parent_flag = 1;
    
    if (!fake_parent_flag && node->pos > 0)
    {   
        node_disconnect(parent, node->pos, 0);
    }
    
    if  (oper == MUL || 
        (oper == DEGREE && one_side == RIGHT) ||
        (oper == DIV && one_side == RIGHT))
    {   
        changes++;        
        if (!fake_parent_flag)
        {
            
            node_connect(parent, node_copy_node(conjugate_node), node->pos);
            
        }
        else
        {
            tmp_node = node_copy_node(conjugate_node);
            node_disconnect(node, LEFT, 1);
            node_disconnect(node, RIGHT, 1);
            node_copy_data(node, tmp_node);
            
            node_dtor(tmp_node);
            return changes;
        }
    }
    else if(oper == DEGREE && one_side == LEFT)
    {
        changes++;
        if (!fake_parent_flag)
        {
            
            node_connect(parent, Create_NUM_node(1), node->pos);
        }
        else
        {
            tmp_node = Create_NUM_node(1);
            node_disconnect(node, LEFT, 1);
            node_disconnect(node, RIGHT, 1);
            node_copy_data(node, tmp_node);
            
            node_dtor(tmp_node);
            return changes;
        }
    }
    
    if (!fake_parent_flag && changes != 0)
    {
        node_dtor(node);
    }
    else if (node->pos > 0 && parent)
    {
        node_connect(parent, node, node->pos);   
    }
    
    return changes;    
}

static int kill_zero_side(Pos_of_node zero_side, Node *node)
{  
     
    Node *zero_node = 0;
    Node *conjugate_node = 0;
    Node *parent = node->parent;
    Node *tmp_node = node;
    int changes = 0;
    int fake_parent_flag = 0;

    if (zero_side == LEFT)
    {
        zero_node = NL;
        conjugate_node = NR;
    }
    else
    {
        zero_node = NR;
        conjugate_node = NL;
    }

    if (conjugate_node == NULL)
        return 0;
    
    Operation oper = node->value.op_value;    
    
    if (!node->parent)
        fake_parent_flag = 1;

    if (!fake_parent_flag && node->pos > 0)
    {   
        node_disconnect(parent, node->pos, 0);
    }

    if (oper == ADD || oper == SUB)
    {   
        changes++;
        if (!fake_parent_flag)
            node_connect(parent, node_copy_node(conjugate_node), node->pos);
        else
        {
            tmp_node = node_copy_node(conjugate_node);
            node_disconnect(node, LEFT, 1);
            node_disconnect(node, RIGHT, 1);
            
            node_copy_data(node, tmp_node);
            
            node_dtor(tmp_node);
            return changes;
        }
    }

    else if (oper == MUL || (zero_side == LEFT && oper == DEGREE) ||(zero_side == LEFT && oper == DIV))
    {   
        changes++;
        if (!fake_parent_flag)
            node_connect(parent, Create_NUM_node(0), node->pos);
        else
        {
            node_disconnect(node, LEFT, 1);
            node_disconnect(node, RIGHT, 1);

            node->type = NUM;
            node->value.op_value = NOT_OP;
            node->value.dbl_value = 0;
            node->priority = NUM_PRIOR;
        }
    }

    else if (zero_side == RIGHT && oper == DEGREE)
    {   
        changes++;
        if (!fake_parent_flag)
            node_connect(parent, Create_NUM_node(0), node->pos);
        else
        {
            node_disconnect(node, LEFT, 1);
            node_disconnect(node, RIGHT, 1);

            node->type = NUM;
            node->value.op_value = NOT_OP;
            node->value.dbl_value = 1;
            node->priority = NUM_PRIOR;
        }   
    }


    if (!fake_parent_flag && changes != 0)
    {
        node_dtor(node);
    }
    else if (node->pos > 0 && parent)
    {
        node_connect(parent, node, node->pos);
    }
    
    return changes++;    
}


static Pos_of_node find_one_side(Node *node)
{
    if (NL && NR && NL->type == NUM && NLV.dbl_value == 1)
        return LEFT;

    if (NR && NL && NR->type == NUM && NRV.dbl_value == 1)
    {   
        return RIGHT;
    }
    

    return NOT_SIDE; 
}

static Pos_of_node find_zero_side(Node *node)
{
    if (NL && NL->type == NUM && NLV.dbl_value == 0)
        return LEFT;

    if (NR && NR->type == NUM && NRV.dbl_value == 0)
        return RIGHT;

        
    return NOT_SIDE;
}

static int compute_constants(Node *node)
{   
    int changes = 0;
    if (node == NULL)
        return changes;

    double result = 0;
    
    if (!node->l_son && node->r_son && node->r_son->type == NUM)
    {
        count_num(node, NULL, node->r_son);
        changes++;
    }

    else if (node->l_son && node->r_son && node->l_son->type == NUM && node->r_son->type == NUM)
    {
        count_num(node, node->l_son, node->r_son);
        changes++;
    }

    if (node->l_son)
        changes += compute_constants(node->l_son);

    if (node ->r_son)
        changes += compute_constants(node->r_son);

    return changes;
}

static double count_num(Node *node, Node *l_son, Node *r_son)
{   
    double result = 0;


    if (l_son == NULL)
    {
        double value = r_son->value.dbl_value;

        switch(node->value.op_value)
        {
            case LN:
                result = log(value);
                break;
            case COS:
                result = cos(value);
                break;
            case SIN:
                result = sin(value);
                break;
            case TG:
                result = tan(value);
                break;
        }
    }
    
    else
    {   
        double l_value = l_son->value.dbl_value;
        double r_value = r_son->value.dbl_value;
        
        switch(node->value.op_value)
        {
            case ADD:
                result = l_value + r_value;
                break;
            case SUB:
                result = l_value - r_value;
                break;
            case MUL:  
                result = l_value * r_value;
                break;
            case DIV:
                if (r_value == 0)   break;
                result = l_value / r_value;
                break;
            case DEGREE:
                result = pow(l_value, r_value);
                break;
            
        }
    }

    if (node->r_son != NULL)
    {
        free(node->r_son);
        node->r_son = NULL;
    }
    
    if (node->l_son != NULL)
    {
        free(l_son);
        node->l_son = NULL;
    }

    node->type = NUM;
    node->value.op_value = NOT_OP;
    node->value.dbl_value = result;
    node->priority = NUM_PRIOR;

    return result;
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
        }
    }

    return 0;
}

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
                    return Diff_ADD(node, ADD);
                case SUB:
                    return Diff_ADD(node, SUB);;
                case MUL:
                    return Diff_MUL(node, ADD);
                case DIV:       
                    return Diff_DIV(node, SUB); 
                case DEGREE:
                    return Diff_DEGREE(node);
                case LN:
                    return Diff_LN(node);
                case COS:
                    return Diff_COS(node);
                case SIN:
                    return Diff_SIN(node);
                case TG:
                    return Diff_TG(node);
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