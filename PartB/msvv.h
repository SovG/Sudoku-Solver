/* File contains struct declarations and function prototypes for msvv.c */

#ifndef MSVV_H
#define MSVV_H

typedef struct {
    int buffer1[9][9];
    int buffer2[11];
    int totalVal;
    int completedChildren;
} SharedMemory;

#endif
