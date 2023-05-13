#ifndef TSL_H
#define TSL_H
typedef struct Lock {
    unsigned int locked;
} Lock;

void acquire(Lock *lck);
void release(Lock *lck);
#endif