#ifndef GENERAL_DBG_GUARD
#define GENERAL_DBG_GUARD

#define DEBUG_ON 1
#if DEBUG_ON == 1
#define SOFT_ASS(condition) if (condition)                                                                                      \
                            {                                                                                                   \
                                printf("something go wrong at %s file, %s func, %s obj, %d line\n", FUNC_GENERAL_INFO());       \
                                print_in_logs("\nsomething go wrong at %s file, %s func, %s obj, %d line", FUNC_GENERAL_INFO());\
                                return -1;                                                                                      \
                            }      
#define SOFT_ASS_NO_RET(condition) if (condition)                                                                                   \
                            {                                                                                                       \
                                printf("something go wrong at %s file, %s func, %s obj, %d line\n", FUNC_GENERAL_INFO());           \
                            }                                                  
#else
#define SOFT_ASS(condition) 
#define SOFT_ASS_NO_RET(condition) 
#endif

#define INPUT_NO_NULL(value) 

#define FUNC_GENERAL_INFO(object)  __FILE__, __FUNCTION__, #object, __LINE__

#define DBG printf("%s:%d -- %s\n", __FILE__, __LINE__, __FUNCTION__);


#endif
