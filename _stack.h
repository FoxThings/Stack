#ifndef _STACK_H_INCLUDED
#define _STACK_H_INCLUDED

typedef unsigned long long       stack_t;
typedef long long                canary;
const canary                     CANPROTECT = 112123123412345;

enum Errors  {
                STACK_OK = 0,
                CANARY_STACK_BREAKING,
                CANARY_HEAP_BREAKING,
                HASHSUM_ALTERED,
                OVERFLOW,
                NOT_A_STACK,
                MEM_NOT_FOUND
            };

#endif // _STACK_H_INCLUDED
