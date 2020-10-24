#include <stdio.h>
#include <stdlib.h>

// Debug mode! Delete this line before release!!!
#define STACK_DEBUG

#define STACK_TEMPLATE_TYPE int
#include "stack.h"
#undef STACK_TEMPLATE_TYPE

#define STACK_TEMPLATE_TYPE double
#include "stack.h"
#undef STACK_TEMPLATE_TYPE

int main()
{
    stack_int stk = {};
    stackCtor_int(&stk, 2);
    stackPush_int(&stk, 10);
    stackPush_int(&stk, 40);

    int val = stackPop_int(&stk);
    printf("%d\n", val);

    stackDtor_int(&stk);

    stack_double stk2 = {};
    stackCtor_double(&stk2, 5);
    stackPush_double(&stk2, 0.5);
    stackPush_double(&stk2, 58.78);

    double val2 = stackPop_double(&stk2);
    printf("%lg\n", val2);

    stackDtor_double(&stk2);

    return 0;
}
