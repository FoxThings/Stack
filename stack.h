#ifdef STACK_TEMPLATE_TYPE
#include "template.h"
#include "_stack.h"

#ifdef STACK_DEBUG
#define STACK_HASHSUM_PROTECT
#define STACK_VERIFY_PROTECT
#define STACK_HASHSUM_PROTECT
#endif // STACK_DEBUG

typedef struct TEMPLATE(stack, STACK_TEMPLATE_TYPE)    TEMPLATE(stack, STACK_TEMPLATE_TYPE);

//! @file

/*!
    stackCtor_[type] initializes stack
    @param[out] stk stack pointer
    @param[in] cap capacity of stack
    @version 1.0
    @authors Zvezdin Nikita
*/
void TEMPLATE(StackCtor, STACK_TEMPLATE_TYPE) (TEMPLATE(stack, STACK_TEMPLATE_TYPE)* stk, stack_t cap);

/*!
    sackDtor_[type] destructs stack
    @param[out] stk stack pointer
    @version 1.0
    @authors Zvezdin Nikita
*/
void TEMPLATE(StackDtor, STACK_TEMPLATE_TYPE) (TEMPLATE(stack, STACK_TEMPLATE_TYPE)* stk);

/*!
    stackExpand_[type] expands stack
    @param[out] stk stack pointer
    @version 1.0
    @authors Zvezdin Nikita
*/
void TEMPLATE(StackExpand, STACK_TEMPLATE_TYPE) (TEMPLATE(stack, STACK_TEMPLATE_TYPE)* stk);

/*!
    stackPush_[type] pushes a new element in stack
    @param[out] stk stack pointer
    @param[in] val new element
    @version 1.0
    @authors Zvezdin Nikita
*/
void TEMPLATE(StackPush, STACK_TEMPLATE_TYPE) (TEMPLATE(stack, STACK_TEMPLATE_TYPE)* stk, STACK_TEMPLATE_TYPE val);

/*!
    stackPop_[type] returns last element of stack
    @param[in] stk stack pointer
    @return element of stack
    @version 1.0
    @authors Zvezdin Nikita
*/
STACK_TEMPLATE_TYPE TEMPLATE(StackPop, STACK_TEMPLATE_TYPE) (TEMPLATE(stack, STACK_TEMPLATE_TYPE)* stk);

// Generate stack with destination type
#include "stack.c"

#endif // STACK_TEMPLATE_TYPE
