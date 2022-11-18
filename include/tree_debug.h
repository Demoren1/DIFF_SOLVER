#ifndef DEBUG_GUARD
#define DEBUG_GUARD

#define TREE_CHECK_ON_ERROR(condition, code) if(condition)                                  \
                                        {                                                   \
                                            tree_error_decoder(code);                       \
                                            tree_dump(tree, INORDER, FUNC_GENERAL_INFO());  \
                                        }


#define TREE_PRINT_ERROR(testing_var, code_of_error) if (testing_var & code_of_error)                           \
                                                    {                                                           \
                                                        fprintf(TREE_LOGS, "%s\n", #code_of_error);         \
                                                        fprintf(TREE_GRAPH_LOGS, "%s\n", #code_of_error); \
                                                    }                                                           \
                                                    else  0;

#define PRINT_IN_LOG_IF(condition, str, ...)   if (condition)                                            \
                                                        {                                               \
                                                            fprintf(TREE_LOGS, str, __VA_ARGS__);       \
                                                        }

#define TREE_DUMP(node, mode)   tree_dump(node, mode, FUNC_GENERAL_INFO());   \
                                tree_graph_dump(node, mode, FUNC_GENERAL_INFO());

#define TREE_LOGS tree_logs

#define HTM_LOGS htm_logs

#define TREE_GRAPH_LOGS tree_graph_logs

typedef enum 
{
    PREORDER  = 1,
    INORDER   = 2, 
    POSTORDER = 3
} Mode_of_print;

int open_tree_logs();

int close_tree_logs();

int tree_print(const Node *node, const Mode_of_print mode);

int tree_dump(Node *node, Mode_of_print mode,  const char* name_function, const char* name_file, const char* name_variable, int num_line);

int tree_graph_dump(Node *node, Mode_of_print mode,  const char* name_function, const char* name_file, const char* name_variable, int num_line);

int tree_error_decoder(int code);

int print_in_logs(const char *str,...);

enum Tree_Errors
{
    TREE_ERROR_REWRITE_NODE                     = 1 << 0,

};

#endif