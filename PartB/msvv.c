/* File contains main function for msvv program as required for COMP2006
 * Assignment. This file is for part B, so it validates a Sudoku Soltion using
 * multiple-threads.
 *
 * Created by Nehal Ghuman
 * 2nd May, 2017                                                              */

#include "msvv.h"
#include "fileIO.h"
#include "validate.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
/* Global Variable shared between threads as used by textbook */
SharedMemory* shareMem;
Locks* locks;
int completedRows;

int main (int argc, char* argv[])
{
    if (argc != 3)
    {
        printf ("Please use program in following format\n");
        printf ("msvv INFILE MAXDELAY\n");
    }
    else
    {
        int i;
        pthread_t threadArray[11]; /*So we can Join threads after they've completed execution */
        shareMem = (SharedMemory*)malloc(sizeof(SharedMemory));
        locks = (Locks*)malloc(sizeof(Locks));
        /* Initializing default values for variables and Mutex/Condition */
        pthread_mutex_init(&locks->mutex, NULL);
        pthread_cond_init(&locks->complete, NULL);
        completedRows = 0;
        shareMem->totalVal = 0;
        shareMem->completedChildren = 0;

        readFile(shareMem, argv[1]);

        /* Use a FOR Loop to create the 11 Child threads. If it is the first
         * 9 threads being created send it to groupOne, 10th thread goes to
         * groupTwo, and the last thread goes to groupThree                 */
        for (i = 0; i < 11; i++)
        {
            if (i < 9)
            {
                pthread_create(&threadArray[i], NULL, &groupOne, NULL);
            }
            else if (i == 9)
            {
                pthread_create(&threadArray[i], NULL, &groupTwo, NULL);
            }
            else
            {
                pthread_create(&threadArray[i], NULL, &groupThree, NULL);
            }
        }
        /* Waits Parent Thread using pthread_cond_wait() until its signaled
         * by one of its Child threads                                      */
        pthread_cond_wait(&locks->complete, &locks->mutex);
        printResults(shareMem);
        /* Terminates all the Child Threads after all validation has occured */
        for (i = 0; i < 11; i++)
        {
            pthread_join(threadArray[i], NULL);
        }
        /* Free Malloc'd Memory and destroy locks */
        pthread_mutex_destroy(&locks->mutex);
        pthread_cond_destroy(&locks->complete);
        free(locks);
        free(shareMem);
    }
    return 0;
}

/* Each thread assigned to groupOne does a single row of the sudoku solution,
 * uses global variabe completedRows to keep track of which thread is doing which
 * row, and access to that variable is done only via mutex and is assigned a local
 * variable to prevent synchronization issues                                   */
void* groupOne (void* threadNum)
{
    int row, valid;

    pthread_mutex_lock(&locks->mutex);
    completedRows++;
    row = completedRows;
    pthread_mutex_unlock(&locks->mutex);
    valid = validateRow(row, shareMem);

    /* As accessing shared data using Mutex Locks to solve CSP */
    pthread_mutex_lock(&locks->mutex);
    shareMem->buffer2[row-1] = valid;
    shareMem->totalVal += valid;
    shareMem->completedChildren++;
    /* If all children have been completed signal parent thread to continue */
    if (shareMem->completedChildren == 11)
    {
        pthread_cond_signal(&locks->complete);
    }
    pthread_mutex_unlock(&locks->mutex);

    pthread_exit(NULL);
}

/* Thread assigned to this function validates all the columns for the Sudoku solution.*/
void* groupTwo (void* nothing)
{
    int valid;
    valid = validateAllCols(shareMem);

    /* As accessing shared data using Mutex Locks to solve CSP */
    pthread_mutex_lock(&locks->mutex);
    shareMem->buffer2[9] = valid;
    shareMem->totalVal += valid;
    shareMem->completedChildren++;
    /* If all children have been completed signal parent thread to continue */
    if (shareMem->completedChildren == 11)
    {
        pthread_cond_signal(&locks->complete);
    }
    pthread_mutex_unlock(&locks->mutex);

    pthread_exit(NULL);
}
/* Threads assigned to this function validates every subgrid of the Sudoku Solution */
void* groupThree (void* nothing)
{
    int valid;
    valid = validateAllGrids(shareMem);

    /* As accessing shared data using Mutex Locks to solve CSP */
    pthread_mutex_lock(&locks->mutex);
    shareMem->buffer2[10] = valid;
    shareMem->totalVal += valid;
    shareMem->completedChildren++;
    /* If all children have been completed signal parent thread to continue */
    if (shareMem->completedChildren == 11)
    {
        pthread_cond_signal(&locks->complete);
    }
    pthread_mutex_unlock(&locks->mutex);

    pthread_exit(NULL);
}
