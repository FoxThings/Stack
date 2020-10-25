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
    StackCtor_int(&stk, 2);
    StackPush_int(&stk, 10);
    StackPush_int(&stk, 40);

    int val = StackPop_int(&stk);
    printf("%d\n", val);

    StackDtor_int(&stk);

    stack_double stk2 = {};
    StackCtor_double(&stk2, 5);
    StackPush_double(&stk2, 0.5);
    StackPush_double(&stk2, 58.78);

    double val2 = StackPop_double(&stk2);
    printf("%lg\n", val2);

    StackDtor_double(&stk2);

    return 0;
}
