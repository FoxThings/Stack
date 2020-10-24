#ifdef STACK_TEMPLATE_TYPE

#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "template.h"
#include "_stack.h"

typedef struct TEMPLATE(stack, STACK_TEMPLATE_TYPE)    TEMPLATE(stack, STACK_TEMPLATE_TYPE);

#ifdef STACK_VERIFY_PROTECT
#define VERIFY_STACK(STACK_TEMPLATE_TYPE)                  \
    if (stackVerify ## _ ## STACK_TEMPLATE_TYPE (stk) )    \
    {                                                      \
        stackDamp ## _ ## STACK_TEMPLATE_TYPE (stk);       \
        assert(!"Something happened");                     \
    }

#define VERIFY(T) VERIFY_STACK(T)
#else
#define VERIFY(T)
#endif // STACK_VERIFY_PROTECT

struct TEMPLATE(stack, STACK_TEMPLATE_TYPE)
{
#ifdef STACK_CANARY_PROTECT
    canary   CANARY_START;
#endif // STACK_CANARY_PROTECT

    stack_t   capacity;
    int      current;
    STACK_TEMPLATE_TYPE* data;

#ifdef STACK_HASHSUM_PROTECT
    stack_t   hashSum;
#endif // STACK_HASHSUM_PROTECT

#ifdef STACK_CANARY_PROTECT
    canary   CANARY_END;
#endif // STACK_CANARY_PROTECT

};

// ---- Debug instruments ----

#ifdef STACK_CANARY_PROTECT
canary* TEMPLATE(getStartCanary,T) (TEMPLATE(stack, STACK_TEMPLATE_TYPE)* stk)
{
    assert(stk != NULL);
    assert(stk->data != NULL);

    char* temp = (char*) stk->data;
    temp -= sizeof(canary);
    return (canary*) temp;
}

canary* TEMPLATE(getEndCanary, STACK_TEMPLATE_TYPE) (TEMPLATE(stack, STACK_TEMPLATE_TYPE)* stk)
{
    assert(stk != NULL);
    assert(stk->data != NULL);

    char* temp = (char*) stk->data;
    temp += sizeof(T) * stk->capacity;
    return (canary*) temp;
}
#endif // STACK_CANARY_PROTECT

#ifdef STACK_HASHSUM_PROTECT
stack_t TEMPLATE(hashSum, STACK_TEMPLATE_TYPE) (TEMPLATE(stack, STACK_TEMPLATE_TYPE)* stk)
{
    assert(stk != NULL);
    assert(stk->data != NULL);

    stack_t count = sizeof(canary) * 2 + sizeof(STACK_TEMPLATE_TYPE) * stk->capacity;
    stack_t sum = 0;

    char* temp = (char*) stk->data;
    temp -= sizeof(canary);

    for (stack_t i = 1; i <= count; ++i)
    {
        sum += temp[i-1] * i;
    }

    return sum;
}
#endif // STACK_HASHSUM_PROTECT

#ifdef STACK_VERIFY_PROTECT
int TEMPLATE(stackVerify, STACK_TEMPLATE_TYPE) (TEMPLATE(stack, STACK_TEMPLATE_TYPE)* stk)
{
    if (stk == NULL)
        return NOT_A_STACK;
    else if (stk->data == NULL)
        return MEM_NOT_FOUND;
#ifdef STACK_CANARY_PROTECT
    else if (*TEMPLATE(getStartCanary,T) (stk) != CANPROTECT || *TEMPLATE(getEndCanary, STACK_TEMPLATE_TYPE) (stk) != CANPROTECT)
        return CANARY_HEAP_BREAKING;
    else if (stk->CANARY_START != CANPROTECT || stk->CANARY_END != CANPROTECT)
        return CANARY_STACK_BREAKING;
#endif // STACK_CANARY_PROTECT
    else if (stk->current < 0 || stk->current > stk->capacity)
        return OVERFLOW;
#ifdef STACK_HASHSUM_PROTECT
    else if (stk->hashSum != TEMPLATE(hashSum, STACK_TEMPLATE_TYPE) (stk))
        return HASHSUM_ALTERED;
#endif // STACK_HASHSUM_PROTECT


    return STACK_OK;
}
#endif // STACK_VERIFY_PROTECT

#ifdef STACK_VERIFY_PROTECT
void TEMPLATE(stackDamp, STACK_TEMPLATE_TYPE) (TEMPLATE(stack, STACK_TEMPLATE_TYPE)* stk)
{
    const int errCode = TEMPLATE(stackVerify, STACK_TEMPLATE_TYPE) (stk);
    char* error = "";

    switch (errCode)
    {
    case STACK_OK:
        error = "OK";
        break;
    case CANARY_STACK_BREAKING:
        error = "CANARY STACK BREAKING";
        break;
    case CANARY_HEAP_BREAKING:
        error = "CANARY HEAP BREAKING";
        break;
    case HASHSUM_ALTERED:
        error = "HASHSUM ALTERED";
        break;
    case OVERFLOW:
        error = "STACK OVERFLOW";
        break;
    case NOT_A_STACK:
        error = "NOT A STACK";
        break;
    case MEM_NOT_FOUND:
        error = "MEMORY NOT FOUND";
        break;
    default:
        error = "UNDEFINED BEHAVIOR";
        break;
    }

    printf("\nStack(%s) [%p]\n{\n", error, stk);
    printf("\tcapacity: %I64d\n\tcurrent: %d\n\tdata[%p]\n}\n\n", stk->capacity, stk->current, stk->data);
}
#endif // STACK_VERIFY_PROTECT

// ---- Stack definition ----

void TEMPLATE(stackCtor, STACK_TEMPLATE_TYPE) (TEMPLATE(stack, STACK_TEMPLATE_TYPE)* stk, stack_t cap)
{
    assert(stk != NULL);

#ifdef STACK_CANARY_PROTECT
    char* temp = (char*) calloc(cap * sizeof(T) + 2 * sizeof(canary), sizeof(char));
    if (temp == NULL) return;
    temp += sizeof(canary);
#else
    STACK_TEMPLATE_TYPE* temp = (STACK_TEMPLATE_TYPE*) calloc(cap, sizeof(STACK_TEMPLATE_TYPE));
    if (temp == NULL) return;
#endif // STACK_CANARY_PROTECT

    stk->data = (STACK_TEMPLATE_TYPE*) temp;
    stk->capacity = cap;
    stk->current = 0;

#ifdef STACK_CANARY_PROTECT
    stk->CANARY_START = CANPROTECT;
    stk->CANARY_END = CANPROTECT;

    *TEMPLATE(getStartCanary, STACK_TEMPLATE_TYPE) (stk) = CANPROTECT;
    *TEMPLATE(getEndCanary, STACK_TEMPLATE_TYPE) (stk) = CANPROTECT;
#endif // STACK_CANARY_PROTECT

#ifdef STACK_HASHSUM_PROTECT
    stk->hashSum = TEMPLATE(hashSum, STACK_TEMPLATE_TYPE) (stk);
#endif // STACK_HASHSUM_PROTECT

    VERIFY(STACK_TEMPLATE_TYPE)

    return;
}

void TEMPLATE(stackDtor, STACK_TEMPLATE_TYPE) (TEMPLATE(stack, STACK_TEMPLATE_TYPE)* stk)
{
    assert(stk != NULL);
    assert(stk->data != NULL);

    VERIFY(STACK_TEMPLATE_TYPE)

#ifdef STACK_CANARY_PROTECT
    char* temp = (char*)stk->data;
    temp -= sizeof(canary);
    memset(temp, '\0', sizeof(T) * stk->capacity + sizeof(canary) * 2);
#else
    char* temp = (char*) stk->data;
    memset(temp, '\0', sizeof(STACK_TEMPLATE_TYPE) * stk->capacity);
#endif // STACK_CANARY_PROTECT
    free(temp);

    return;
}

void TEMPLATE(stackExpand, STACK_TEMPLATE_TYPE) (TEMPLATE(stack, STACK_TEMPLATE_TYPE)* stk)
{
    assert(stk != NULL);
    assert(stk->data != NULL);

    VERIFY(STACK_TEMPLATE_TYPE)

#ifdef STACK_CANARY_PROTECT
    *TEMPLATE(getEndCanary, STACK_TEMPLATE_TYPE) (stk) = 0;

    char* temp = (char*)stk->data;
    temp -= sizeof(canary);

    stk->capacity *= 2;
    stack_t newSize = sizeof(canary) * 2 + stk->capacity * sizeof(T);
    temp = (char*) realloc(temp, newSize);

    temp += sizeof(canary);
    stk->data = (T*) temp;

    *TEMPLATE(getStartCanary, STACK_TEMPLATE_TYPE) (stk) = CANPROTECT;
    *TEMPLATE(getEndCanary, STACK_TEMPLATE_TYPE) (stk) = CANPROTECT;
#else
    stk->capacity *= 2;
    stk->data = (STACK_TEMPLATE_TYPE*) realloc(stk->data, stk->capacity * sizeof(STACK_TEMPLATE_TYPE));
#endif // STACK_CANARY_PROTECT

#ifdef STACK_HASHSUM_PROTECT
    stk->hashSum = TEMPLATE(hashSum, STACK_TEMPLATE_TYPE) (stk);
#endif // STACK_HASHSUM_PROTECT

    VERIFY(STACK_TEMPLATE_TYPE)
}

void TEMPLATE(stackPush, STACK_TEMPLATE_TYPE) (TEMPLATE(stack, STACK_TEMPLATE_TYPE)* stk, STACK_TEMPLATE_TYPE val)
{
    assert(stk != NULL);
    assert(stk->data != NULL);

    VERIFY(STACK_TEMPLATE_TYPE)

    if (stk->current == stk->capacity)
        TEMPLATE(stackExpand, STACK_TEMPLATE_TYPE) (stk);

    stk->data[stk->current++] = val;

#ifdef STACK_HASHSUM_PROTECT
    stk->hashSum = TEMPLATE(hashSum, STACK_TEMPLATE_TYPE) (stk);
#endif // STACK_HASHSUM_PROTECT

    VERIFY(STACK_TEMPLATE_TYPE)
}

STACK_TEMPLATE_TYPE TEMPLATE(stackPop, STACK_TEMPLATE_TYPE) (TEMPLATE(stack, STACK_TEMPLATE_TYPE)* stk)
{
    assert(stk != NULL);
    assert(stk->data != NULL);

    VERIFY(STACK_TEMPLATE_TYPE)

    STACK_TEMPLATE_TYPE ret = stk->data[--stk->current];

#ifdef STACK_HASHSUM_PROTECT
    stk->hashSum = TEMPLATE(hashSum, STACK_TEMPLATE_TYPE) (stk);
#endif // STACK_HASHSUM_PROTECT

    VERIFY(STACK_TEMPLATE_TYPE)

    return ret;
}

#endif // STACK_TEMPLATE_TYPE
