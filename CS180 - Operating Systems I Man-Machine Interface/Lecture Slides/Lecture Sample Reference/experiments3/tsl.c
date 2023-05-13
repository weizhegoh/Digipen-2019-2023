#include "tsl.h"



static inline unsigned int TSL(volatile unsigned int* ptr)
{
/*
   xchgl result, *ptr
   xchgl exchanges the values of its two operands, while
   locking the memory bus to exclude other operations.
   You are not required to understand the assembly inlining.
   the xchgl assembly code performs the swap function
   in a single instruction. Swapping between the register
   containing result and the address "ptr" in 
   one atomic and indivisable instruction.
*/

    int result;
    asm volatile("lock;"
                 "xchgl %0, %1;"
                 : "=r"(result), "=m"(*ptr)
                 : "0"(1), "m"(*ptr)
                 : "memory");
    return result;
}

void acquire(Lock *lck) {
    while(TSL(&(lck->locked)) != 0)
    ;
}

void release(Lock *lck) {
    lck->locked = 0;
}