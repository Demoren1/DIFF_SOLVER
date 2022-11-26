#ifndef DIFF_DEBUG_GUARD
#define DIFF_DEBUG_GUARD

#define MAX_LEN_OP 16

#define PRINT_DIFF_IN_LOG_IF(condition, node)       if (condition)                                          \
                                                        {                                                       \
                                                            if (node->type == OP)                               \
                                                            {                                                   \
                                                                char res[MAX_LEN_OP];                                   \
                                                                get_op(node->value.op_value, res);              \
                                                                fprintf(TREE_LOGS, " %s ", res);                  \
                                                            }                                                   \
                                                            else if(node->type == VAR)                          \
                                                            {                                                   \
                                                                fprintf(TREE_LOGS, "%c", node->value.var_value);      \
                                                            }                                                   \
                                                            else if(node->type == NUM)                          \
                                                            {                                                   \
                                                                fprintf(TREE_LOGS, "%g", node->value.dbl_value);      \
                                                            }                                                   \
                                                        }


#endif                                                        