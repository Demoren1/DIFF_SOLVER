#ifndef DIFF_DEBUG_GUARD
#define DIFF_DEBUG_GUARD

#define PRINT_DIFF_IN_LOG_IF(condition, node)       if (condition)                                          \
                                                        {                                                       \
                                                            if (node->type == OP)                               \
                                                            {                                                   \
                                                                fprintf(TREE_LOGS, "%c", get_op(node->value.op_value));  \
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