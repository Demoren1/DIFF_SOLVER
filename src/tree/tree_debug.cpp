#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <tree_funcs.h>
#include <tree_debug.h>

static FILE *TREE_LOGS = 0;
static FILE *HTM_LOGS  = 0;
static FILE *TREE_GRAPH_LOGS = 0;

int open_tree_logs()
{
    TREE_LOGS = fopen("tree_logs.txt", "w");

    if (NULL == TREE_LOGS)
    {
        printf("Cant open logs");
        return -1;
    }

    HTM_LOGS = fopen("graph_log.htm", "w");

    if (NULL == HTM_LOGS)
    {
        printf("Cant open graph logs for browser");
        return -1;
    }

    fprintf(HTM_LOGS, "<pre>\n\n");

    return 0;
}

int close_tree_logs()
{
    fclose(TREE_LOGS);
    fclose(HTM_LOGS);
    
    return 0;
}




int tree_dump(Node *node, Mode_of_print mode,  const char* name_function, const char* name_file, const char* name_variable, int num_line)
{
    fprintf(TREE_LOGS,  "Dump called from %s file, in %s func, in %d line, name of variable = %s\n\n",
                         name_file, name_function, num_line, name_variable);    
    fprintf(HTM_LOGS,   "Dump called from %s file, in %s func, in %d line, name of variable = %s\n\n",
                        name_file, name_function, num_line, name_variable);

    tree_print(node, mode);

    return 0;

}


int tree_print(const Node *node, const Mode_of_print mode)
{
    if (!node) 
    {
        return 0;
    }

    fprintf(TREE_LOGS," ( ");
    PRINT_IN_LOG_IF(mode == PREORDER, "%s", node->data);

    if(node->l_son)
        {   
            tree_print(node->l_son, mode);
        }
    else    fprintf(TREE_LOGS, " (VOID) ");

    PRINT_IN_LOG_IF(mode == INORDER, "%s", node->data);

    if (node->r_son)
    {
        tree_print(node->r_son, mode);
    }
    
    else    fprintf(TREE_LOGS, " (VOID) ");

    PRINT_IN_LOG_IF(mode == POSTORDER, "%s", node->data);
    
    fprintf(TREE_LOGS," ) ");

    fflush(TREE_LOGS);
    return 0;
}

int tree_print_graph(const Node *node);

static int num_of_node = 1;
static int num_of_pic = 1;

int tree_graph_dump(Node *node, Mode_of_print mode,  const char* name_function, const char* name_file, const char* name_variable, int num_line)
{   
    TREE_GRAPH_LOGS = fopen("graph_log.html", "w");

    if (NULL == TREE_GRAPH_LOGS)
    {
        printf("Cant open graph logs");
        return -1;
    }

    fputs("digraph lala{\n", TREE_GRAPH_LOGS);
    fputs("rankdir = HR;\n", TREE_GRAPH_LOGS);
    fputs("bgcolor = grey;\n", TREE_GRAPH_LOGS);

    fputs("graph [splines=polyline];\n", TREE_GRAPH_LOGS);
    fputs("node [shape = \"plaintext\", style = \"solid\"];\n", TREE_GRAPH_LOGS);
    
    tree_print_graph(node);
    
    fputs("\n}", TREE_GRAPH_LOGS);

    fprintf(HTM_LOGS, "DUMP #%d \n", num_of_pic);
    fprintf(HTM_LOGS, "<img src = graph_dumps/dump_%d.jpeg>\n", num_of_pic);

    char command[128] = {};   
    sprintf(command, "dot -Tjpeg graph_log.html > graph_dumps/dump_%d.jpeg", num_of_pic++);
    
    fclose(TREE_GRAPH_LOGS);
    system(command);
    return 0;
}

int tree_show_graph()
{
    char command[256] = {};
    int cur_num_pic = num_of_pic - 1;

    sprintf(command, "xdg-open graph_dumps/dump_%d.jpeg", cur_num_pic);

    system(command);

    return 0;
}



int tree_print_graph(const Node *node)
{
    if (!node)  return 0;

    int cur_node = num_of_node;

    fprintf(TREE_GRAPH_LOGS, "node%d [\n"
                        "label=<\n"
                        
                        "<table border=\"0\" cellborder=\"1\" cellspacing=\"0\">\n"
                        
                        "    <tr><td bgcolor=\"#ad8abf\"><font color=\"#061f2b\">parent = %p</font></td></tr>\n" 
                        
                        "    <tr><td bgcolor=\"lightblue\"><font color=\"#0000ff\">data = %s</font></td></tr>\n"
                        
                        "    <tr><td bgcolor=\"lightgreen\"><font color=\"black\">cur_address = %p</font></td></tr>\n"
                        
                        "    <tr>\n"
                        "    <td>\n"
                        
                        "        <table border=\"0\" cellborder=\"1\">\n"
                        
                        "        <tr>\n"
                        
                        "            <td bgcolor=\"#70de9f\" port = \"L%d\"> Left = %p</td> \n"
                        
                        "            <td bgcolor = \"#c8A2c8\" port = \"R%d\"> Right =  %p </td>\n"
                        
                        "        </tr> \n"
                        
                        "        </table> \n"
                        
                        "    </td>\n"
                        
                        "    </tr>\n" 
                        
                        "</table>>\n"
                        "]\n\n", num_of_node, node->parent, node->data, node, num_of_node, node->l_son, num_of_node, node->r_son);
    num_of_node++;   

    if(node->l_son)
        {   
            fprintf(TREE_GRAPH_LOGS, "node%d: L%d -> node%d; \n", cur_node, cur_node, num_of_node);
            tree_print_graph(node->l_son);
        }

    if (node->r_son)
    {   
        fprintf(TREE_GRAPH_LOGS, "node%d: R%d -> node%d; \n", cur_node, cur_node, num_of_node);
        tree_print_graph(node->r_son);
    }

    return 0;
}

int tree_error_decoder(int code)
{
    TREE_PRINT_ERROR(code, TREE_ERROR_REWRITE_NODE);


    return 0;
}

int print_in_logs(const char *str,...)
{   
    if (TREE_LOGS != NULL && str != NULL)
    {
        va_list args;
        
        va_start(args, str);
        vfprintf(TREE_LOGS, str, args);
        va_end(args);
        
        fflush(TREE_LOGS);
    }

    return 0;
}