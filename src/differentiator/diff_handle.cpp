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

static int del_new_line_sym(Buffer *buffer);

static int input_val_type(Node *node, Buffer *buff);

static int create_node_if_need(char cur_sym, Node* node, Buffer *buff);

static int exit_node_if_need(char cur_sym, Buffer *buff);

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
        return 0;
    }
    
    create_node_if_need(cur_sym, node, buff);

    input_val_type(node, buff);

    cur_sym = buff->buffer[buff->curr_index];
    cur_pos = buff->curr_index;

    if (exit_node_if_need(cur_sym, buff))
    {
        return 0;
    }

    return 0;
}

static int input_val_type(Node *node, Buffer *buff)
{
    int cur_sym = buff->buffer[buff->curr_index];
    int cur_pos = buff->curr_index;
    printf("after cur_sym = %c\n", cur_sym);

    if (isalpha(cur_sym))
    {
        node->value.var_value = cur_sym;
        
        buff->curr_index++;
        node->type = VAR;
    }
    else if(isdigit(cur_sym))
    {
        int shift = 0;
        node->type = NUM;

        sscanf(buff->buffer + cur_pos, "%lf%n", &(node->value.dbl_value), &shift);
    
        buff->curr_index += shift;
    }
    else
    {
        node->type = OP;

        int shift = 0;
        if (cur_sym == '+')
        {
            node->value.op_value = ADD;
        }
        else if (cur_sym == '-')
        {
            node->value.op_value = SUB;
        }
        else if (cur_sym == '*')
        {
            node->value.op_value = MUL;
        }
        else if (cur_sym == '/')
        {
            node->value.op_value = DIV;
        }
        
        buff->curr_index++;
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
        //DBG;                  
        parent->l_son = new_node;       
    }                                   
    else if(parent->r_son == NULL)             
    {       
        //DBG;                            
        parent->r_son = new_node;       
    }                                   
    else    SOFT_ASS_NO_RET(1);

    return new_node;
}

Node *_diff_node_ctor(Type_of_expression type, double dbl_value, Operation op_value, char var_value)
{
    Node *node = {};
    node = node_ctor();
    SOFT_ASS_NO_RET(node == NULL);

    if (dbl_value != NAN)
    {
        node->value.dbl_value = dbl_value;
    }
    else if (op_value != NOT_OP)
    {
        node->value.op_value = op_value;
    }
    else if(var_value != ' ')
    {
        node->value.var_value = var_value;
    }
    else SOFT_ASS_NO_RET(1);

    return node;   
}


