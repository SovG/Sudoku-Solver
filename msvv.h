/* File contains function prototypes and struct declerations for msvv.c */
/* Header gaurds as typedefing */
#ifndef MSVV_H
#define MSVV_H
#include <semaphore.h>


typedef struct
{
    int sudokuSolution[9][9];
} Buffer1;

typedef struct
{
    int validated[11];
    int completedChildren;
} Buffer2;

typedef struct
{
    sem_t mutex;
    sem_t full;
    sem_t empty;
} Locks;


#endif
