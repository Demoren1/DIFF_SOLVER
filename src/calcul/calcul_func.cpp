#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <calcul_funcs.h>
#include <general_debug.h>
const char* s = NULL;

int get_General(const char *str)
{
    s = str;

    int val = get_Expression();

    assert(*s == '\0');

    return val;
} 

int get_Num()
{
    int val = 0;
    const char *old_s = s;

    while (isdigit(*s))
    {   
        val = val*10 + *s -'0';
        s++;
    }

    assert(s != old_s);

    return val;
}

int get_P()
{
    int val = 0;

    if (*s == '(')
    {
        s++;
        val = get_Expression();
        assert (*s == ')');
        s++;
    }
    else
    {
        val = get_Num();
    }

    return val;
}

int get_T()
{
    int val = 0;
    val = get_Degree();

    while(*s == '*' || *s == '/')
    {
        int op = *s;

        s++;
        int val2 = get_Degree();
        
        if (op == '*')
            val *= val2;
        else 
            val /= val2;
    }

    return val;
}

int get_Expression()
{
    int val = 0;
    val = get_T();

    while(*s == '+' || *s == '-')
    {
        int op = *s;

        s++;
        int val2 = get_T();
        
        if (op == '+')
            val += val2;
         
        else 
            val -= val2;
    }

    return val;
}

int get_Degree()
{
    int val = 0;

    val = get_P();

    while(*s == '^')
    {
        s++;
        int val2 = get_P();

        val = pow(val, val2);
    }

    return val;
}
