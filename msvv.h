/* File contains function prototypes and struct declerations for msvv.c */
/* Header gaurds as typedefing */
#ifndef MSVV_H
#define MSVV_H
#import <semaphore.h>


typedef struct Buffer1
{
    int sudokuSolution[9][9];
} Buffer1;

typedef struct Buffer2
{
    int validated[11];
    int completedChildren;
} Buffer2;

typedef struct Locks
{
    sem_t mutex;
    sem_t full;
    sem_t empty;
} Locks;


#endif
