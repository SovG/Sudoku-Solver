/* File contains struct declarations and function prototypes for msvv.c */

#ifndef MSVV_H
#define MSVV_H
#include <pthread.h>

typedef struct {
    int buffer1[9][9];
    int buffer2[11];
    int totalVal;
    int completedChildren;
} SharedMemory;

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t complete;
} Locks;

void* groupOne (void*);
void* groupTwo (void*);
void* groupThree (void*);

#endif
